#pragma once

#include "cli.h"
#include "render.h"
#include "display/display.h"
#include "memory/memory.h"
#include "memory/mmu.h"
#include "processors/central/cpu.h"
#include "processors/pixel/ppu.h"
#include "processors/central/interrupt.h"
#include "time/timer.h"

#define DISABLE_BOOTLOAD 0
#define ENABLE_BOOTLOAD 1

class GameBoy
{
public:
	GameBoy::GameBoy(gb_opts *opts);
	GameBoy::~GameBoy();
	void GameBoy::run();
	bool GameBoy::on();
private:
	char *rom, title[26];

	Memory *mem;
	Mmu *mmu;
	Cpu *cpu;
	Ppu *ppu;
	Timer *timer;
	Renderer *renderer;
	Display *display;
};

