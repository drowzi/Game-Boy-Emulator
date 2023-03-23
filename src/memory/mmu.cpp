#include "common/stdafx.h"
#include "memory/mmu.h"
#include "memory/memory.h"

Mmu::Mmu(Memory *mem)
{
	this->mem = mem;

	dma_active = false;
	dma_writes = 0;
	dma_address = 0x00;

	isr_vblank = 0x0040;
	isr_lcdc = 0x0048;
	isr_time = 0x0050;
	isr_serial = 0x0058;
	isr_joypad = 0x0060;

	P1 = &mem->memory[0xff00];
	SB = &mem->memory[0xff01];
	SC = &mem->memory[0xff02];
	DIV = &mem->memory[0xff04];
	TIMA = &mem->memory[0xff05];
	TMA = &mem->memory[0xff06];
	TAC = &mem->memory[0xff07];
	IF = &mem->memory[0xff0f];
	NR10 = &mem->memory[0xff10];
	NR11 = &mem->memory[0xff11];
	NR12 = &mem->memory[0xff12];
	NR13 = &mem->memory[0xff13];
	NR14 = &mem->memory[0xff14];
	NR21 = &mem->memory[0xff16];
	NR22 = &mem->memory[0xff17];
	NR23 = &mem->memory[0xff18];
	NR24 = &mem->memory[0xff19];
	NR30 = &mem->memory[0xff1a];
	NR31 = &mem->memory[0xff1b];
	NR32 = &mem->memory[0xff1c];
	NR33 = &mem->memory[0xff1d];
	NR34 = &mem->memory[0xff1e];
	NR41 = &mem->memory[0xff20];
	NR42 = &mem->memory[0xff21];
	NR43 = &mem->memory[0xff22];
	NR44 = &mem->memory[0xff23];
	NR50 = &mem->memory[0xff24];
	NR51 = &mem->memory[0xff25];
	NR52 = &mem->memory[0xff26];
	WPM = &mem->memory[0xff30];
	LCDC = &mem->memory[0xff40];
	STAT = &mem->memory[0xff41];
	SCY = &mem->memory[0xff42];
	SCX = &mem->memory[0xff43];
	LY = &mem->memory[0xff44];
	LYC = &mem->memory[0xff45];
	DMA = &mem->memory[0xff46];
	BGP = &mem->memory[0xff47];
	OBP0 = &mem->memory[0xff48];
	OBP1 = &mem->memory[0xff49];
	WY = &mem->memory[0xff4a];
	WX = &mem->memory[0xff4b];
	DMG_ROM = &mem->memory[0xff50];
	IE = &mem->memory[0xffff];
}

Mmu::~Mmu()
{
}

__uint8 Mmu::read(__uint16 address)
{
	__uint8 ppu_mode;
	
	ppu_mode = (*STAT) & 0x03;

	// (todo): decide if it's a good idea to return 0xff for instruction fetches when the system is in DMA mode.

	if (address >= 0xfe00 && address < 0xfea0)
	{
		/*
		if (ppu_mode >= 0x02)
		{
			return 0xff;
		}*/
		return mem->read(address);
	}
	else if (address >= 0x8000 && address < 0xa000)
	{
		/*
		if (ppu_mode == 0x03)
		{
			return 0xff;
		}*/
		return mem->read(address);
	}
	else if (address >= 0xe000 && address < 0xfdff)
	{
		// Echo ram
		return mem->read(0xc000 + (address - 0xe000));
	}
	// if hardware register: bypass?
	return mem->read(address);
}

void Mmu::write(__uint16 address, __uint8 data)
{
	__uint8 ppu_mode;

	ppu_mode = (*STAT) & 0x03;

	if (address >= 0xfe00 && address < 0xfea0)
	{
		/*
		if (ppu_mode >= 0x02)
		{
			return;
		}
		*/
	}
	else if (address >= 0x8000 && address < 0xa000)
	{
		/*
		if (ppu_mode == 0x03)
		{
			return;
		}
		*/
	}
	else if (address >= 0xe000 && address < 0xfdff)
	{
		// Echo ram
		return; 
	}

	// if hardware register: bypass?
	mem->write(address, data);
}

void Mmu::check_dma()
{
	if (*DMA)
	{
		dma_active = true;
		dma_writes = 0;
		dma_address = (*DMA) << 8;
		*DMA = 0x00;
	}
}

void Mmu::update_dma(unsigned int cycles)
{
	check_dma();
	if (dma_active)
	{
		unsigned int target = dma_writes + (cycles / 4);
		for (; dma_writes < target && dma_writes < 160; dma_writes++)
		{
			write(0xfe00 + dma_writes, read(dma_address + dma_writes));
		}
		if (dma_writes == 160) dma_active = false;
	}
}

void Mmu::set_interrupt_flag_bit(char bit)
{
	mem->write(0xff0f, mem->read(0xff0f) | (1 << bit));
}

void Mmu::reset_interrupt_flag_bit(char bit)
{
	mem->write(0xff0f, mem->read(0xff0f) & ~(1U << bit));
}
