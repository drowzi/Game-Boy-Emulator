
#include "common/stdafx.h"
#include "gameboy.h"

GameBoy::GameBoy(gb_opts *opts)
{
	this->rom = (char *)opts->rom_path.data();
	this->mem = new Memory();
	this->mmu = new Mmu(this->mem);
	this->cpu = new Cpu(this->mmu);
	this->timer = new Timer(this->mmu);
	this->renderer = init_renderer(opts->renderer.data(), opts->height, opts->width);
	this->display = new Display(this->renderer);
	this->ppu = new Ppu(this->mmu, this->display);
}

GameBoy::~GameBoy()
{
	delete(mem);
	delete(mmu);
	delete(cpu);
	delete(ppu);
	delete(timer);
	delete(renderer);
}

bool GameBoy::on()
{
	return (renderer->exit() == false);
}

void GameBoy::run()
{
	unsigned int cycles;

	test_cpu();
	cpu->page_game(rom);
	memcpy(title, "GameBoy - ", 10);
	memcpy(&title[10], (char *)&mem->memory[0x134], 16);
	renderer->set_window_title(title);

	while (on())
	{
		if (mem->memory[0xff50])
			cpu->unpage_boot(rom);
		cpu->execute();
		cpu->probe_interrupts();
		cycles = cpu->offload_instruction_cycles();
		mmu->update_dma(cycles);
		timer->update(cycles);
		ppu->update(cycles);
	}
}


