#pragma once

#include "common/stdafx.h"
#include "memory/mmu.h"

class Timer
{
public:
	Timer(Mmu *mmu);
	~Timer();
	void Timer::update(unsigned int cycles);
private:
	void Timer::update_div();
	void Timer::update_tima();
	bool Timer::timer_enabled() { return mem->read(0xff07) & 0x04; }
	void Timer::set_frequency();
	bool Timer::div_tick() { return (internal_div_counter >= (cpu_speed / frequency)); }

	// Default
	unsigned int frequency = 4096; 
	unsigned int inc_frequency;

	unsigned int cpu_speed = 4194304;

	unsigned int internal_tima_counter = 1024;
	unsigned int internal_div_counter;
	Mmu *mmu;

	// Used to bypass some hardware register functionalities
	Memory *mem;
};

