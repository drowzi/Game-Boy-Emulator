#pragma once

#include "common/stdafx.h"
#include "memory/register.h"
#include "memory/memory.h"
#include "memory/mmu.h"

#include <iostream>

class Cpu
{
public:
	Cpu(Mmu *mmu);
	~Cpu();

	// These bad boys here need a new home
	void Cpu::page_game(const char *file_name);
	void Cpu::page_boot(__uint8 bootloader[], unsigned int size);
	void Cpu::unpage_boot(const char *file_name);

	void Cpu::clear_volatile();
	unsigned int Cpu::offload_instruction_cycles();

	void Cpu::probe_interrupts();
	void Cpu::interrupt(__uint16 isr, __uint8 id_bit);

	// Decode
	void Cpu::decode(__uint8 op);
	void Cpu::decode_ext(__uint8 op);

	// Fetch
	__uint8 Cpu::fetch8();
	__uint16 Cpu::fetch16();
	__uint8 Cpu::read(__uint16 address);
	void Cpu::write(__uint16 a, __uint8 d);
	void Cpu::reg_write(Register8 *r1, __uint8 d);
	void Cpu::reg_write(Register16 *r1);

	// Execute
	void Cpu::execute();

	void Cpu::set_flags(__uint8 z, __uint8 n, __uint8 h, __uint8 c);

	// Flags
	void Cpu::set_zf();
	void Cpu::clear_zf();
	__uint8 Cpu::get_zf();
	void Cpu::set_nf();
	void Cpu::clear_nf();
	__uint8 Cpu::get_nf();
	void Cpu::set_hf();
	void Cpu::clear_hf();
	__uint8 Cpu::get_hf();
	void Cpu::set_cf();
	void Cpu::clear_cf();
	__uint8 Cpu::get_cf();

	// Execution engine
	// INS DEST, SRC

	void Cpu::nop();

	void Cpu::load(Register8 *r1, Register8 *r2);
	void Cpu::load(Register8 *r1, __uint8 d);
	void Cpu::load(__uint16 a, Register8 *r1);
	void Cpu::load(__uint16 a, __uint8 d);
	void Cpu::load(__uint16 a, Register16 *r1);
	void Cpu::load(Register16 *r1, __uint16 a);
	void Cpu::load(Register16 *r1, Register16 *r2);
	void Cpu::loads(__int8 d);

	void Cpu::loadh(__uint8 d, Register8 *r1);
	void Cpu::loadh(Register8 *r1, __uint8 d);

	void Cpu::loadi(__uint16 a, Register8 *r1);
	void Cpu::loadi(Register8 *r1, __uint8 d);
	void Cpu::loadd(__uint16 a, Register8 *r1);
	void Cpu::loadd(Register8 *r1, __uint8 d);

	void Cpu::push(Register16 *r1);
	void Cpu::pop(Register16 *r1);

	void Cpu::add(Register8 *r1, Register8 *r2);
	void Cpu::add(Register8 *r1, __uint8 d);
	void Cpu::add(Register16 *r1, Register16 *r2);
	void Cpu::add(Register16 *r1, __uint16 d);
	void Cpu::add(Register16 *r1, Register8 *r2);
	void Cpu::adds(__int8 d);

	void Cpu::adc(Register8 *r1);
	void Cpu::adc(Register8 *r1, Register8 *r2);
	void Cpu::adc(Register8 *r1, __uint16 a);
	void Cpu::adc(__uint8 d);

	void Cpu::sub(Register8 *r1);
	void Cpu::sub(__uint8 d);

	void Cpu::sbc(Register8 *r1);
	void Cpu::sbc(Register8 *r1, Register8 *r2);
	void Cpu::sbc(Register8 *r1, __uint16 a);
	void Cpu::sbc(__uint8 d);

	void Cpu::and(Register8 *r1);
	void Cpu::and(__uint8 d);

	void Cpu::xor(Register8 *r1);
	void Cpu::xor(__uint8 d);

	void Cpu::or(Register8 *r1);
	void Cpu::or(__uint8 d);

	void Cpu::inc(Register8 *r1);
	void Cpu::inc(Register16 *r1);
	void Cpu::inc(__uint16 a);

	void Cpu::dec(Register8 *r1);
	void Cpu::dec(Register16 *r1);
	void Cpu::dec(__uint16 a);

	void Cpu::set(Register8 *r1, __uint8 idx);
	void Cpu::set(__uint16 a, __uint8 idx);

	void Cpu::bit(Register8 *r1, __uint8 idx);
	void Cpu::bit(__uint16 a, __uint8 idx);

	void Cpu::res(Register8 *r1, __uint8 idx);
	void Cpu::res(__uint16 a, __uint8 idx);

	void Cpu::cp(__uint8 d);
	void Cpu::cp(Register8 *r1);
	void Cpu::cp(__uint16 a);

	void Cpu::rlc(__uint16 a);
	void Cpu::rlc(Register8 *r1);
	void Cpu::rlca();

	void Cpu::rrc(__uint16 a);
	void Cpu::rrc(Register8 *r1);
	void Cpu::rrca();

	void Cpu::rl(__uint16 a);
	void Cpu::rl(Register8 *r1);
	void Cpu::rla();

	void Cpu::rr(__uint16 a);
	void Cpu::rr(Register8 *r1);
	void Cpu::rra();

	void Cpu::sla(Register8 *r1);
	void Cpu::sla(__uint16 a);

	void Cpu::sra(Register8 *r1);
	void Cpu::sra(__uint16 a);

	void Cpu::srl(Register8 *r1);
	void Cpu::srl(__uint16 a);

	void Cpu::jp(__uint16 a);
	void Cpu::jpnz(__uint16 a);
	void Cpu::jpz(__uint16 a);
	void Cpu::jpnc(__uint16 a);
	void Cpu::jpc(__uint16 a);

	void Cpu::jr(__int8 a);
	void Cpu::jrnz(__int8 a);
	void Cpu::jrz(__int8 a);
	void Cpu::jrnc(__int8 a);
	void Cpu::jrc(__int8 a);

	void Cpu::call(__uint16 a);
	void Cpu::callnz(__uint16 a);
	void Cpu::callz(__uint16 a);
	void Cpu::callnc(__uint16 a);
	void Cpu::callc(__uint16 a);

	void Cpu::ret();
	void Cpu::retnz();
	void Cpu::retz();
	void Cpu::retnc();
	void Cpu::retc();

	void Cpu::reti();

	void Cpu::rst(__uint16 a);

	void Cpu::ccf();
	void Cpu::scf();
	void Cpu::swap(Register8 *r1);
	void Cpu::swap(__uint16 a);
	void Cpu::daa();
	void Cpu::cpl();
	void Cpu::di();
	void Cpu::ei();
	void Cpu::hlt();
	void Cpu::stop();

	// These two lads can most likely be merged
	inline bool Cpu::check_carry(__bit h, __uint8 l, __uint8 r);
	inline bool Cpu::check_borrow(__uint8 l, __uint8 r);

	void Cpu::stress_test();
	void Cpu::__lab();


	// Bypass mmu (for backwards compatible stress test)
	Memory *mem;

	// Make these two variables signed long integers and clean up all underflow checks, kiss.
	unsigned int cycles;
	unsigned int instruction_cycles;

private:

	Register8  A;
	Register8  F;
	Register16 AF;

	Register8  B;
	Register8  C;
	Register16 BC;

	Register8  D;
	Register8  E;
	Register16 DE;

	Register8  H;
	Register8  L;
	Register16 HL;

	Register16 SP;

	Register16 PC;

	// Bus to mmu
	Mmu *mmu;
	
	bool interrupt_master_enable = false;

	// Refactor layout:
	//__uint16 AF;
	//__uint8 *A = (__uint8 *)((char *)(&_AF));
	//__uint8 *F = (__uint8 *)((char *)(&_AF[3]));

};

#define MACHINE_CYCLES(n) (instruction_cycles += (4 * n))
#define LOUD_CPU 0
#define PRINT_INSTRUCTIONS 0

void test_cpu();
void test_timer();


