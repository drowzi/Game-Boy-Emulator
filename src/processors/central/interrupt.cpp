#include "common/stdafx.h"
#include "processors/central/cpu.h"
#include "memory/mmu.h"

#define IBIT_VBLANK 0x01U
#define IBIT_LCD	0x02U
#define IBIT_TIME	0x04U
#define IBIT_SERIAL 0x08U
#define IBIT_JOYPAD 0x10U

void Cpu::interrupt(__uint16 isr, __uint8 id_bit)
{
	MACHINE_CYCLES(3);
	interrupt_master_enable = false;
	mem->write(0xff0f, mem->read(0xff0f) & ~(id_bit));
	mem->write(0xffff, mem->read(0xffff) & ~(id_bit));
	push(&PC);
	jp(isr);
}

// Each ISR should take 5 machine cycles (2 NOPs, 2 for Push, 1 for writing isr address)
void Cpu::probe_interrupts()
{
	__uint8 _if, _ie;

	// (todo): Add support for interrupt timeouts. I can't imagine VBLANK/HBLANK interrupts are meant to persist.
	if (interrupt_master_enable)
	{
		_if = *(mmu->IF);
		_ie = *(mmu->IE);

		// Lowest bits get the highest priority
		if (_if & IBIT_VBLANK)
		{
			if (_ie & IBIT_VBLANK)
			{
				std::cout << "VBlank interrupt" << std::endl;
				return interrupt(mmu->isr_vblank, IBIT_VBLANK);
			}

		}
		if (_if & IBIT_LCD)
		{
			if (_ie & IBIT_LCD)
			{
				std::cout << "LCD interrupt" << std::endl;
				return interrupt(mmu->isr_lcdc, IBIT_LCD);
			}

		}
		if (_if & IBIT_TIME)
		{
			if (_ie & IBIT_TIME)
			{
				std::cout << "Time interrupt" << std::endl;
				return interrupt(mmu->isr_time, IBIT_TIME);
			}

		}
		if (_if & IBIT_SERIAL)
		{
			if (_ie & IBIT_SERIAL)
			{
				std::cout << "Serial interrupt" << std::endl;
				return interrupt(mmu->isr_serial, IBIT_SERIAL);
			}

		}
		if (_if & IBIT_JOYPAD)
		{
			if (_ie & IBIT_JOYPAD)
			{
				std::cout << "Joypad interrupt" << std::endl;
				return interrupt(mmu->isr_joypad, IBIT_JOYPAD);
			}

		}
		/* Alternate structure
		for (__uint8 i = 0, bitmask = 0x01U; bitmask <= 0x10U; i++, bitmask <<= 1)
		{
			if ((req_flag & bitmask) && ((*mmu->IE) & bitmask))
			{
				interrupt(idt[i], i);
			}
		}
		*/
	}
}