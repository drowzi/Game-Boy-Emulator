#include "common/stdafx.h"
#include "processors/central/cpu.h"

void Cpu::stress_test()
{
	// Check flags
	clear_volatile();
	set_cf();
	assert(get_cf() == 1);
	clear_cf();
	assert(get_cf() == 0);

	PC.set(0xffff);
	PC.set(PC.get() + 1);
	assert(PC.get() == 0);

	PC.set(0xff00);
	PC.set(PC.get() + 0xffff);
	assert(PC.get() == 0xfeff);

	A.set(0xff);
	F.set(0xff);
	assert(AF.get() == 0xffff);
	A.set(A.get() + 1);
	assert(AF.get() == 0x00ff);
	F.set(F.get() + 1);
	assert(AF.get() == 0x0000);

	clear_volatile();

	// Load BC, 0xffff; Load DE, 0xffff
	__uint8 code[] = { 0x01, 0xff, 0xff, 0x11, 0xff, 0xff };
	int pc_shadow = 0;
	while (pc_shadow < (sizeof(code) / sizeof(code[0])))
	{
		mem->write(pc_shadow, code[pc_shadow]);
		assert(mem->read(pc_shadow) == code[pc_shadow]);
		pc_shadow++;
	}
	decode(fetch8());
	decode(fetch8());
	assert(BC.get() == 0xffff);
	assert(DE.get() == 0xffff);
	clear_volatile();

	// NOP
	mem->write(0x00, 0x00);
	decode(fetch8());
	assert(instruction_cycles == 4);
	clear_volatile();

	// Load r16, d16
	mem->write(0x00, 0x01);
	mem->write(0x01, 0xff);
	mem->write(0x02, 0x01);
	decode(fetch8());
	assert(BC.get() == 0x01ff);
	assert(instruction_cycles == 12);
	clear_volatile();

	// Load (r16), r8
	mem->write(0x00, 0x02);
	A.set(0x50);
	BC.set(0x10);
	decode(fetch8());
	assert(mem->read(0x10) == 0x50);
	assert(instruction_cycles == 8);
	clear_volatile();

	// Inc r16
	mem->write(0x00, 0x03);
	decode(fetch8());
	assert(BC.get() == 0x01);
	assert(instruction_cycles == 8);
	clear_volatile();

	// Inc/Dec r8
	mem->write(0x00, 0x04);
	decode(fetch8());
	assert(B.get() == 0x01);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0x05);
	decode(fetch8());
	assert(B.get() == 0);
	assert(get_zf() == 1);
	mem->write(0x02, 0x05);
	decode(fetch8());
	//assert(get_hf() == 1);
	assert(B.get() == 0xff);
	clear_volatile();

	// Load r8, d8
	mem->write(0x00, 0x06);
	mem->write(0x01, 0x80);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(B.get() == 0x80);
	clear_volatile();

	// RLCA
	A.set(0xff);
	mem->write(0x00, 0x07);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(instruction_cycles == 4);
	assert(A.get() == (__uint8)((0xff << 1) | 1));
	clear_volatile();

	// Load (a16), SP
	mem->write(0xfffd, 0x01);
	SP.set(0x05);
	mem->write(0x00, 0x08);
	mem->write(0x01, 0xfd);
	mem->write(0x02, 0xff);
	decode(fetch8());
	assert(mem->read(0xfffd) == 0x05);
	assert(instruction_cycles == 20);
	clear_volatile();

	// Add r16, r16
	mem->write(0x00, 0x09);
	HL.set(0xff);
	BC.set(0xff);
	decode(fetch8());
	assert(HL.get() == (0xff + 0xff));
	assert(instruction_cycles == 8);
	clear_volatile();

	// Load r8, (r16)
	mem->write(0x00, 0x0a);
	BC.set(0x100);
	mem->write(0x100, 0x50);
	decode(fetch8());
	assert(A.get() == 0x50);
	assert(instruction_cycles == 8);
	clear_volatile();

	// Dec r16
	mem->write(0x00, 0x0b);
	decode(fetch8());
	assert(BC.get() == 0xffff);
	assert(instruction_cycles == 8);
	clear_volatile();

	// RRCA
	A.set(0x01);
	mem->write(0x00, 0x0f);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(instruction_cycles == 4);
	assert(A.get() == 0x80);
	clear_volatile();

	// Stop

	// RLA
	A.set(0x01);
	set_cf();
	mem->write(0x00, 0x17);
	decode(fetch8());
	assert(instruction_cycles == 4);
	assert(A.get() == ((0x01 << 1) | 0x01));
	clear_cf();
	A.set(0x01);
	mem->write(0x01, 0x17);
	decode(fetch8());
	assert(A.get() == 0x01 << 1);
	assert(instruction_cycles == 8);
	clear_volatile();

	// JR d8
	mem->write(0x00, 0x18);
	mem->write(0x01, 0x05);
	decode(fetch8());
	assert(PC.get() == 0x07); // Jump relative, (fetch8(), fetch8(), 0x05)
	assert(instruction_cycles == 12);
	clear_volatile();

	// RRA
	A.set(0x01);
	set_cf();
	mem->write(0x00, 0x1f);
	decode(fetch8());
	assert(instruction_cycles == 4);
	assert(A.get() == ((0x01 >> 1) | (0x01 << 7)));
	clear_cf();
	A.set(0x01);
	mem->write(0x01, 0x1f);
	decode(fetch8());
	assert(A.get() == 0x01 >> 1);
	assert(instruction_cycles == 8);
	clear_volatile();

	// JRNZ d8
	set_zf();
	mem->write(0x00, 0x20);
	mem->write(0x01, 0x08);
	decode(fetch8());
	assert(PC.get() == 2);
	assert(instruction_cycles == 8);
	PC.set(0);
	clear_zf();
	decode(fetch8());
	assert(PC.get() == 10);
	assert(instruction_cycles == 20);
	clear_volatile();

	// Loadi (r16), r8
	mem->write(0x00, 0x22);
	mem->write(0x50, 0x70);
	HL.set(0x50);
	A.set(0x05);
	decode(fetch8());
	assert(HL.get() == 0x51);
	assert(mem->read(0x50) == 0x05);
	assert(instruction_cycles == 8);
	clear_volatile();

	// DAA

	// JRZ d8
	mem->write(0x00, 0x28);
	mem->write(0x01, 0x08);
	decode(fetch8());
	assert(PC.get() == 2);
	assert(instruction_cycles == 8);
	PC.set(0);
	set_zf();
	decode(fetch8());
	assert(PC.get() == 10);
	assert(instruction_cycles == 20);
	clear_volatile();

	// Loadi r8, (r16)
	mem->write(0x00, 0x2a);
	mem->write(0x50, 0x70);
	HL.set(0x50);
	A.set(0x05);
	decode(fetch8());
	assert(HL.get() == 0x51);
	assert(A.get() == 0x70);
	assert(instruction_cycles == 8);
	clear_volatile();

	// CPL
	mem->write(0x00, 0x2f);
	A.set(0x00);
	decode(fetch8());
	assert(A.get() == 0xff);
	assert(get_nf() == 1);
	assert(get_hf() == 1);
	assert(instruction_cycles == 4);
	clear_volatile();

	// JRNC d8
	set_cf();
	mem->write(0x00, 0x30);
	mem->write(0x01, 0x08);
	decode(fetch8());
	assert(PC.get() == 2);
	assert(instruction_cycles == 8);
	PC.set(0);
	clear_cf();
	decode(fetch8());
	assert(PC.get() == 10);
	assert(instruction_cycles == 20);
	clear_volatile();

	// Loadd (r16), r8
	mem->write(0x00, 0x32);
	mem->write(0x50, 0x70);
	HL.set(0x50);
	A.set(0x05);
	decode(fetch8());
	assert(HL.get() == 0x50 - 1);
	assert(instruction_cycles == 8);
	assert(mem->read(0x50) == 0x05);
	clear_volatile();

	// Inc/Dec (HL)
	mem->write(0x20, 0x05);
	HL.set(0x20);
	mem->write(0x00, 0x34);
	decode(fetch8());
	assert(mem->read(0x20) == 0x06);
	assert(instruction_cycles == 12);
	mem->write(0x01, 0x35);
	decode(fetch8());
	assert(mem->read(0x20) == 0x05);
	assert(instruction_cycles == 24);
	clear_volatile();

	// SCF
	mem->write(0x00, 0x37);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(instruction_cycles == 4);
	clear_volatile();

	// JRC d8
	clear_cf();
	mem->write(0x00, 0x38);
	mem->write(0x01, 0x08);
	decode(fetch8());
	assert(PC.get() == 2);
	assert(instruction_cycles == 8);
	PC.set(0);
	set_cf();
	decode(fetch8());
	assert(PC.get() == 10);
	assert(instruction_cycles == 20);
	clear_volatile();

	// Loadi r8, (r16)
	mem->write(0x00, 0x3a);
	mem->write(0x50, 0x70);
	HL.set(0x50);
	A.set(0x05);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(HL.get() == 0x4f);
	assert(A.get() == 0x70);
	clear_volatile();

	// CCF
	set_cf();
	mem->write(0x00, 0x3f);
	decode(fetch8());
	assert(get_cf() == 0);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0x3f);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(instruction_cycles == 8);
	clear_volatile();

	// LD r8, r8
	mem->write(0x00, 0x41);
	C.set(0x54);
	decode(fetch8());
	assert(B.get() == 0x54);
	assert(instruction_cycles == 4);
	clear_volatile();

	// LD r8, (HL)
	mem->write(0x54, 0x05);
	HL.set(0x54);
	mem->write(0x00, 0x46);
	decode(fetch8());
	assert(B.get() == 0x05);
	assert(instruction_cycles == 8);
	clear_volatile();

	// LD (HL), r8
	HL.set(0x50);
	B.set(0x05);
	mem->write(0x00, 0x70);
	decode(fetch8());
	assert(mem->read(0x50) == B.get());
	assert(instruction_cycles == 8);
	clear_volatile();

	// HALT

	// Add r8, r8
	set_nf();
	A.set(100);
	B.set(101);
	mem->write(0x00, 0x80);
	decode(fetch8());
	assert(get_nf() == 0);
	assert(A.get() == 201);
	assert(instruction_cycles == 4);
	// check for bit 3 carry
	A.set(0x8);
	B.set(0x8);
	mem->write(0x01, 0x80);
	decode(fetch8());
	assert(get_hf() == 1);
	assert(A.get() == 0x10);
	clear_hf();
	// check for bit 7 carry
	A.set(0xff);
	B.set(0xff);
	mem->write(0x02, 0x80);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(A.get() == 0xfe);
	clear_volatile();
	// check for zero
	A.set(0xff);
	B.set(0x01);
	mem->write(0x03, 0x80);
	decode(fetch8());
	assert(get_zf() == 1);
	assert(A.get() == 0x00);
	clear_volatile();

	// Add r8, (HL)
	set_nf();
	mem->write(0x50, 0x40);
	HL.set(0x50);
	A.set(0x10);
	mem->write(0x00, 0x86);
	decode(fetch8());
	assert(get_nf() == 0);
	assert(A.get() == 0x50);
	assert(instruction_cycles == 8);
	// check for bit 3 carry
	mem->write(0x50, 0x8);
	A.set(0x8);
	mem->write(0x01, 0x86);
	decode(fetch8());
	assert(get_hf() == 1);
	assert(A.get() == 0x10);
	clear_hf();
	// check for bit 7 carry
	A.set(0xff);
	HL.set(0x50);
	mem->write(0x50, 0xff);
	mem->write(0x02, 0x86);
	decode(fetch8());
	assert(get_cf() == 1);
	assert(A.get() == 0xfe);
	clear_volatile();
	// check for zero
	A.set(0xff);
	HL.set(0x50);
	mem->write(0x50, 0x01);
	mem->write(0x03, 0x86);
	decode(fetch8());
	assert(get_zf() == 1);
	assert(A.get() == 0x00);
	clear_volatile();

	// Adc tests work on the foundation of add tests
	// Adc A, r8
	set_nf();
	set_cf();
	A.set(100);
	B.set(101);
	mem->write(0x00, 0x88);
	decode(fetch8());
	assert(instruction_cycles == 4);
	assert(get_nf() == 0);
	assert(A.get() == 202);
	clear_volatile();

	// Adc A, (HL)
	set_nf();
	set_cf();
	mem->write(0x50, 0x40);
	HL.set(0x50);
	A.set(0x10);
	mem->write(0x00, 0x8E);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(get_nf() == 0);
	assert(A.get() == 0x50 + 1);
	clear_volatile();

	// Sub B
	A.set(101);
	B.set(100);
	mem->write(0x00, 0x90);
	decode(fetch8());
	assert(instruction_cycles == 4);
	assert(A.get() == 1);
	// N
	mem->write(0x01, 0x90);
	B.set(1);
	decode(fetch8());
	assert(A.get() == 0);
	assert(get_nf() == 1);
	clear_nf();
	// H
	mem->write(0x02, 0x90);
	A.set(23);
	B.set(8);
	decode(fetch8());
	//assert(get_hf() == 1);
	clear_cf();
	// C
	mem->write(0x03, 0x90);
	A.set(0xfe);
	B.set(0xff);
	decode(fetch8());
	//assert(get_cf() == 1);
	clear_volatile();
	// (HL)
	mem->write(0x00, 0x96);
	mem->write(0x50, 0x10);
	HL.set(0x50);
	A.set(0xf0);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(A.get() == (0xf0 - 0x10));
	clear_volatile();

	// Sbc B
	set_cf();
	A.set(101);
	B.set(100);
	mem->write(0x00, 0x98);
	decode(fetch8());
	assert(instruction_cycles == 4);
	assert(A.get() == 0);
	clear_volatile();
	// (HL)
	set_cf();
	mem->write(0x00, 0x9E);
	mem->write(0x50, 0x10);
	HL.set(0x50);
	A.set(0xf0);
	decode(fetch8());
	assert(A.get() == (0xf0 - (0x10 + 1)));
	clear_volatile();

	// And B / (HL)
	mem->write(0x00, 0xA0);
	B.set(0xff);
	A.set(0xff);
	decode(fetch8());
	assert(A.get() == 0xff);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0xA0);
	B.set(0x00);
	A.set(0xff);
	decode(fetch8());
	assert(get_zf() == 1);
	assert(A.get() == 0x00);
	clear_volatile();
	mem->write(0x00, 0xA6);
	mem->write(0x50, 0xff);
	HL.set(0x50);
	A.set(0xff);
	decode(fetch8());
	assert(A.get() == 0xff);
	clear_volatile();

	// Xor B / (HL)
	mem->write(0x00, 0xA8);
	B.set(0x00);
	A.set(0xff);
	decode(fetch8());
	assert(A.get() == 0xff);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0xA8);
	B.set(0xff);
	A.set(0xff);
	decode(fetch8());
	assert(get_zf() == 1);
	assert(A.get() == 0x00);
	clear_volatile();
	mem->write(0x00, 0xAE);
	mem->write(0x50, 0x00);
	HL.set(0x50);
	A.set(0xff);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(A.get() == 0xff);
	clear_volatile();

	// Or B / (HL)
	mem->write(0x00, 0xB0);
	B.set(0x00);
	A.set(0xff);
	decode(fetch8());
	assert(A.get() == 0xff);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0xB0);
	B.set(0x00);
	A.set(0x00);
	decode(fetch8());
	assert(get_zf() == 1);
	assert(A.get() == 0x00);
	clear_volatile();
	mem->write(0x00, 0xB6);
	mem->write(0x50, 0x00);
	HL.set(0x50);
	A.set(0xff);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(A.get() == 0xff);
	clear_volatile();

	// Cp B / (HL) - Flags have been tested for this via the sub op
	mem->write(0x00, 0xB8);
	B.set(0x00);
	A.set(0xff);
	decode(fetch8());
	assert(get_zf() == 0);
	assert(get_nf() == 1);
	assert(instruction_cycles == 4);
	mem->write(0x01, 0xB8);
	B.set(0xff);
	A.set(0xff);
	decode(fetch8());
	assert(get_zf() == 1);
	clear_volatile();
	mem->write(0x00, 0xBE);
	mem->write(0x50, 0xf0);
	HL.set(0x50);
	A.set(0xf0);
	decode(fetch8());
	assert(instruction_cycles == 8);
	assert(get_zf() == 1);
	clear_volatile();

	// Push/pop
	mem->write(0x00, 0xc5);
	BC.set(0xffff);
	decode(fetch8());
	assert(SP.get() == 0xfffe - 2);
	assert(instruction_cycles == 16);
	assert(mem->read(0xfffe) == 0xff);
	assert(mem->read(0xfffe - 1) == 0xff);
	mem->write(0x01, 0xc1);
	BC.set(0x0000);
	decode(fetch8());
	assert(instruction_cycles == 28);
	assert(BC.get() == 0xffff);
	clear_volatile();

	// Ret [condition]?
	mem->write(0x00, 0xC9);
	BC.set(0x0005);
	push(&BC);
	decode(fetch8());
	assert(PC.get() == 0x0005);
	assert(SP.get() == 0xfffe);
	assert(instruction_cycles == 28); // 16 (ret) + 12 (PUSH, no fetch)
	clear_volatile();

	// Rst n
	mem->write(0x00, 0xff);
	decode(fetch8());
	assert(mem->read(0xfffe - 1) == 0x01);
	assert(mem->read(0xfffe) == 0x00);
	assert(instruction_cycles == 16);
	assert(PC.get() == 0x38);
	clear_volatile();

	// Call
	mem->write(0x00, 0xCD);
	mem->write(0x01, 0x01);
	mem->write(0x02, 0xff);
	decode(fetch8());
	assert(PC.get() == 0xff01);
	assert(instruction_cycles == 24);
	clear_volatile();

	// Push/Pop/Ret
	mem->write(0x00, 0xCD);
	mem->write(0x01, 0x50);
	mem->write(0x02, 0x00);
	decode(fetch8());
	assert(PC.get() == 0x0050);
	mem->write(0x50, 0xc9);
	decode(fetch8());
	assert(PC.get() == 0x0003);
	BC.set(0x0003);
	mem->write(0x03, 0xC5);
	decode(fetch8());
	assert(mem->read(SP.get() + 1) == 0x03);
	mem->write(0x04, 0xC3);
	mem->write(0x05, 0x50);
	mem->write(0x06, 0x00);
	decode(fetch8());
	assert(PC.get() == 0x0050);
	decode(fetch8());
	assert(PC.get() == 0x0003);
}