
#include "common/stdafx.h"
#include "time/timer.h"


Timer::Timer(Mmu *mmu)
{
	this->mmu = mmu;
	this->mem = mmu->mem;
	this->set_frequency();
}


Timer::~Timer()
{
}

void Timer::update_div()
{
	// I believe this is only probed by software
	*(mmu->DIV)++;
	internal_div_counter = 0;
}

void Timer::update_tima()
{
	*(mmu->TIMA)++;
	if (!(*mmu->TIMA))
	{
		//std::cout << "TIMA overflow" << std::endl;
		mmu->set_interrupt_flag_bit(2);
		mem->write(0xff05, mem->read(0xff06));
	}
	internal_tima_counter = 0;
}

void Timer::update(unsigned int cycles)
{
	if ((*mmu->TAC) & 0x04)
	{
		internal_tima_counter += cycles;
		internal_div_counter += cycles;
		if (internal_tima_counter >= inc_frequency)
		{
			//std::cout << "tima tick" << std::endl;
			update_tima();
		}
		if (internal_div_counter >= 256)
		{
			update_div();
		}
	}
}

void Timer::set_frequency()
{
	__uint8 input_clock_select = mem->read(0xff07);

	switch (input_clock_select & 0x03)
	{
		case 0: frequency = 4096;   break;
		case 1: frequency = 262144; break;
		case 2: frequency = 65536;  break;
		case 3: frequency = 16384;  break;
	}
	inc_frequency = cpu_speed / frequency;
}

