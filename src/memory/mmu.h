#pragma once
#include "common/stdafx.h"
#include "memory/memory.h"

class Mmu
{
public:
	Mmu(Memory *mem);
	~Mmu();

	__uint8 Mmu::read(__uint16 address);
	void Mmu::write(__uint16 address, __uint8 data);

	// Might be a good idea to make a seperate dma "chip" for this functionality
	void Mmu::check_dma();
	void Mmu::update_dma(unsigned int cycles);
	bool dma_active;
	unsigned int dma_writes;
	__uint16 dma_address;

	void Mmu::set_interrupt_flag_bit(char bit);
	void Mmu::reset_interrupt_flag_bit(char bit);

	// This is public for old test compatibility for the moment
	Memory *mem;

	// Hardware register direct paths for associated IO devices.
	__uint8 *P1;
	__uint8 *SB;
	__uint8 *SC;
	__uint8 *DIV;
	__uint8 *TIMA;
	__uint8 *TMA;
	__uint8 *TAC;
	__uint8 *IF;
	__uint8 *NR10;
	__uint8 *NR11;
	__uint8 *NR12;
	__uint8 *NR13;
	__uint8 *NR14;
	__uint8 *NR21;
	__uint8 *NR22;
	__uint8 *NR23;
	__uint8 *NR24;
	__uint8 *NR30;
	__uint8 *NR31;
	__uint8 *NR32;
	__uint8 *NR33;
	__uint8 *NR34;
	__uint8 *NR41;
	__uint8 *NR42;
	__uint8 *NR43;
	__uint8 *NR44;
	__uint8 *NR50;
	__uint8 *NR51;
	__uint8 *NR52;
	__uint8 *WPM;
	__uint8 *LCDC;
	__uint8 *STAT;
	__uint8 *SCY;
	__uint8 *SCX;
	__uint8 *LY;
	__uint8 *LYC;
	__uint8 *DMA;
	__uint8 *BGP;
	__uint8 *OBP0;
	__uint8 *OBP1;
	__uint8 *WY;
	__uint8 *WX;
	__uint8 *IE;
	__uint8 *DMG_ROM;

	__uint16 isr_vblank;
	__uint16 isr_lcdc;
	__uint16 isr_time;
	__uint16 isr_serial;
	__uint16 isr_joypad;

	// Defined for personal reference for now
	__uint16 ROM_Bank_00 = 0x0000;
	__uint16 ROM_Bank_01 = 0x4000;
	__uint16 VRAM = 0x8000;
	__uint16 BG_Map_Data_0 = 0x9800;
	__uint16 BG_Map_Data_1 = 0x9C00;
	__uint16 External_RAM = 0xA000;
	__uint16 Work_RAM_Bank_0 = 0xC000;
	__uint16 Work_RAM_Bank_1 = 0xD000;
	__uint16 Echo_RAM = 0xE000;
	__uint16 Sprite_attribute_table = 0xFE00;
	__uint16 Unusable = 0xFEA0;
	__uint16 IO_registers = 0xFF00;
	__uint16 High_RAM = 0xFF80;
	__uint16 IE_register = 0xFFFF;

};

