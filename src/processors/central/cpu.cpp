
#include "common/stdafx.h"

#include "processors/central/cpu.h"
#include "boot/bootloader.h"
#include "time/timer.h"

#include <fstream>

Cpu::Cpu(Mmu *mmu) : 
	A("A"), B("B"), C("C"), D("D"), E("E"), F("F"), H("H"), L("L"),
	AF("AF", &A, &F), BC("BC", &B, &C), DE("DE", &D, &E), HL("HL", &H, &L),
	SP("SP"), PC("PC"), mmu(mmu), mem(mmu->mem)
{
	clear_volatile();
}

Cpu::~Cpu()
{
}

inline bool Cpu::check_carry(__bit h, __uint8 l, __uint8 r)
{
	return static_cast<bool>((l & 0x0f) + (r & 0x0f) + h & 0x10);
}

// Despite what the documents say, the cpu sets the borrow flag on carry
inline bool Cpu::check_borrow(__uint8 l, __uint8 r)
{
	return static_cast<bool>((l & 0xff) < (r & 0xff));
}

void Cpu::page_game(const char *file_name)
{
	std::ifstream rom_file;

	rom_file.open(file_name, std::ios::in | std::ios::binary);

	//size_t size = rom_file.tellg();

	if (rom_file.is_open())
	{
		for (unsigned int i = 0x00; i < 0x8000; i++)
		{
			this->mem->write(i, rom_file.get());
		}
	}
	else
	{
		std::cout << "Failed to open rom file at: " << file_name << std::endl;
		abort();
	}
	rom_file.close();
	page_boot(gb_bootloader, 0x100);
}

void Cpu::page_boot(__uint8 bootloader[], unsigned int size)
{
	*mmu->DMG_ROM = 0x00;
	for (unsigned int i = 0x00; i < size; i++)
	{
		this->mem->write(i, bootloader[i]);
	}
}

void Cpu::unpage_boot(const char *file_name)
{
	std::ifstream rom_file;

	*mmu->DMG_ROM = 0x00;
	assert(PC.get() == 0x100);

	rom_file.open(file_name, std::ios::in | std::ios::binary);
	if (rom_file.is_open())
	{
		for (unsigned int i = 0x00; i < 0x100; i++)
		{
			this->mem->write(i, rom_file.get());
		}
	}
	else
	{
		std::cout << "Failed to open rom file at: " << file_name << std::endl;
		abort();
	}
	rom_file.close();
}

void Cpu::clear_volatile()
{
	AF.set(0);
	BC.set(0);
	DE.set(0);
	HL.set(0);
	PC.set(0x0000);
	SP.set(0xfffe);
	interrupt_master_enable = false;
	cycles = 0;
	instruction_cycles = 0;
}

unsigned int Cpu::offload_instruction_cycles()
{
	unsigned int tmp;

	cycles += instruction_cycles;
	tmp = instruction_cycles;
	instruction_cycles = 0;
	return tmp;
}

__uint8 Cpu::fetch8()
{
	__uint8 val;

	MACHINE_CYCLES(1);

#if LOUD_CPU
	if (PC.get() == 0x00fa)
		std::cout << "Bootstrap logo check reached" << std::endl;

	if (PC.get() == 0x00fe)
		std::cout << "Bootstrap successfully ran" << std::endl;
#endif

	val = this->mem->read(PC.get()); // In dma mode, all memory outside of HRAM is unaccessible and therefore this requires multiplexing.
	PC.set(PC.get() + 1);

	return val;
}

__uint16 Cpu::fetch16()
{
	__uint8 lo, hi;

	lo = fetch8();
	hi = fetch8();
	return (hi << 8) | lo;
}

void Cpu::execute()
{
	decode(fetch8());
}

__uint8 Cpu::read(__uint16 address)
{
	MACHINE_CYCLES(1);
	return mmu->read(address);
}

void Cpu::write(__uint16 a, __uint8 d)
{
	MACHINE_CYCLES(1);
	mmu->write(a, d);
}

// LD - Complete

void Cpu::load(Register8 *r1, __uint8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << r1->id << ", $" << (int)d << std::endl;
#endif
	r1->set(d);
}

void Cpu::load(Register8 *r1, Register8 *r2)
{
	load(r1, r2->get());
}

void Cpu::load(__uint16 a, __uint8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld ($" << a << "), $" << (int)d << std::endl;
#endif
	write(a, d);
}

void Cpu::load(__uint16 a, Register8 *r1)
{
	load(a, r1->get());
}

void Cpu::load(__uint16 a, Register16 *r1)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld ($" << a << "), " << r1->id << std::endl;
#endif
	write(a, r1->get() & 0x00ff);
	write(a + 1, r1->get() >> 8);
}

void Cpu::load(Register16 *r1, __uint16 a)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << r1->id << ", ($" << (int)a << ")" << std::endl;
#endif
	r1->set(a);
}

void Cpu::loads(__int8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << HL.id << ", " << SP.id << "+" << (int)d << std::endl;
#endif
	__bit h, c;

	MACHINE_CYCLES(2);

	// The carry from the first operation doesn't matter
	h = check_carry(0, (__uint8)SP.get(), d);
	c = check_carry(h, SP.get() >> 4, d >> 4);

	HL.set(SP.get() + d);
	set_flags(0, 0, h, c);
}

void Cpu::load(Register16 *r1, Register16 *r2)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << r1->id << ", " << r2->id << std::endl;
#endif
	MACHINE_CYCLES(1);
	
	r1->set(r2->get());
}

void Cpu::loadi(__uint16 a, Register8 *r1)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld (" << HL.id << "+), " << r1->id << std::endl;
#endif
	write(a, r1->get());
	HL.set(HL.get() + 1);
}

void Cpu::loadi(Register8 *r1, __uint8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << r1->id << ", (" << HL.id << "+)" << std::endl;
#endif
	r1->set(d);
	HL.set(HL.get() + 1);
}

void Cpu::loadd(__uint16 a, Register8 *r1)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld (" << HL.id << "-), " << r1->id << std::endl;
#endif
	write(a, r1->get());
	HL.set(HL.get() - 1);
}

void Cpu::loadd(Register8 *r1, __uint8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld (" << HL.id << "-), $" << (int)d << std::endl;
#endif
	r1->set(d);
	HL.set(HL.get() - 1);
}

void Cpu::loadh(__uint8 d, Register8 *r1)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld ($0xff00 + $" << (int)d << ")," << r1->id << std::endl;
#endif
	write(0xff00 + d, r1->get());
}

void Cpu::loadh(Register8 *r1, __uint8 d)
{
#if PRINT_INSTRUCTIONS
	std::cout << "ld " << r1->id << ", ($0xff00 + $" << (int)d << "), "<< std::endl;
#endif
	r1->set(read(0xff00 + d));
}

// STACK - Complete. No flags are needed

// Assume canary is always 0 (0xfffe)
void Cpu::push(Register16 *r1)
{
	MACHINE_CYCLES(1);

	write(SP.get(), (r1->get() & 0xff00) >> 8);
	write(SP.get() - 1, r1->get() & 0x00ff);
	SP.set(SP.get() - 2);
}

void Cpu::pop(Register16 *r1)
{
	// The lower nibble of F must remain 0

	if (r1 == &AF)
		r1->set((read(SP.get() + 2) << 8) | (read(SP.get() + 1) & 0xf0));
	else 
		r1->set((read(SP.get() + 2) << 8) | read(SP.get() + 1));
	assert(SP.get() < 0xfffd);
	SP.set(SP.get() + 2);
}

// ADD - Complete

void Cpu::add(Register8 *r1, __uint8 d)
{
	__bit z, h, c;

	h = check_carry(0, r1->get(), d);
	c = check_carry(h, r1->get() >> 4, d >> 4);

	r1->set(r1->get() + d);

	z = !r1->get();

	set_flags(z, 0, h, c);
}

void Cpu::add(Register8 *r1, Register8 *r2)
{
	add(r1, r2->get());
}

void Cpu::add(Register16 *r1, __uint16 d)
{
	__bit z, h, c;

	MACHINE_CYCLES(1);

	h = check_carry(0, r1->get() >> 8, d >> 8);
	c = check_carry(h, r1->get() >> 12, d >> 12);

	r1->set(r1->get() + d);

	z = !r1->get();

	set_flags(z, 0, h, c);
}

void Cpu::add(Register16 *r1, Register16 *r2)
{
	__bit z, h, c;

	MACHINE_CYCLES(1);

	// This is weird but I think it correctly emulates the ALU, which works in nibbles
	h = check_carry(0, (__uint8)r1->get(), (__uint8)r2->get());
	c = check_carry(h, r1->get() >> 4, r2->get() >> 4);

	h = check_carry(c, r1->get() >> 8, r2->get() >> 8);
	c = check_carry(h, r1->get() >> 12, r2->get() >> 12);

	r1->set(r1->get() + r2->get());

	z = get_zf();

	set_flags(z, 0, h, c);
}

void Cpu::add(Register16 *r1, Register8 *r2)
{
	assert(r1->get() == SP.get());
	return add(r1, r2->get());
}

void Cpu::adds(__int8 d)
{
	__bit h, c;

	MACHINE_CYCLES(2);

	// The lower bits are the only ones that matter
	h = check_carry(0, (__uint8)SP.get(), d);
	c = check_carry(h, SP.get() >> 4, d >> 4);

	SP.set(SP.get() + d);

	set_flags(0, 0, h, c);
}

// ADC

// (note): d=d+carry where d=255 and carry=1 will set d to 0 and break carry checks, which is why this routine's carry check is an anomaly
void Cpu::adc(__uint8 d)
{
	__bit z, h, c;

	h = check_carry(get_cf(), A.get(), d);
	c = check_carry(h, A.get() >> 4, d >> 4);

	d += get_cf();

	A.set(A.get() + d);

	z = !A.get();

	set_flags(z, 0, h, c);
}

void Cpu::adc(Register8 *r1)
{
	return adc(r1->get());
}

void Cpu::adc(Register8 *r1, Register8 *r2)
{
	return adc(r2->get());
}

// SUB

void Cpu::sub(__uint8 d)
{
	__bit z, h, c;

	h = check_borrow(A.get() << 4, d << 4);
	c = check_borrow(A.get(), d);

	A.set(A.get() - d);

	z = !A.get();

	set_flags(z, 1, h, c);
}

void Cpu::sub(Register8 *r1)
{
	return sub(r1->get());
}

// SBC

// (todo): remove use of inconsistent integer here
void Cpu::sbc(__uint8 d)
{
	__bit z, h, c;

	int tmp = A.get() - d - get_cf();

	if (tmp < 0)
	{
		c = 1;
	}
	else c = 0;

	tmp = (A.get() & 0x0f) - (d & 0x0f) - get_cf();

	if (tmp < 0)
	{
		h = 1;
	}
	else h = 0;

	A.set(A.get() - d - get_cf());

	z = !A.get();

	set_flags(z, 1, h, c);
}

void Cpu::sbc(Register8 *r1)
{
	return sbc(r1->get());
}

void Cpu::sbc(Register8 *r1, Register8 *r2)
{
	return sbc(r2->get());
}

// INC

void Cpu::inc(Register8 *r1)
{
	__bit z, h, c;

	h = check_carry(0, r1->get(), 1);
	r1->set(r1->get() + 1);
	z = !r1->get();
	c = get_cf();
	set_flags(z, 0, h, c);
}

void Cpu::inc(__uint16 a)
{
	__bit z, h, c;
	__uint8 old_val, new_val;
	
	old_val = read(a);
	new_val = old_val + 1;
	h = check_carry(0, old_val, 1);
	write(a, new_val);
	z = !new_val;
	c = get_cf();
	set_flags(z, 0, h, c);
}

void Cpu::inc(Register16 *r1)
{
	MACHINE_CYCLES(1);

	r1->set(r1->get() + 1);
}

// DEC

void Cpu::dec(Register8 *r1)
{
	__bit z, h, c;

	h = check_borrow(r1->get() << 4, 1);
	r1->set(r1->get() - 1);
	z = !r1->get();
	c = get_cf();
	set_flags(z, 1, h, c);
}

void Cpu::dec(Register16 *r1)
{
	MACHINE_CYCLES(1);

	r1->set(r1->get() - 1);
}

void Cpu::dec(__uint16 a)
{
	__bit z, h, c;
	__uint8 old_val, new_val;
	
	old_val = read(a);
	new_val = old_val - 1;
	h = check_borrow(old_val << 4, 1);
	write(a, new_val);
	z = !new_val;
	c = get_cf();
	set_flags(z, 1, h, c);
}

// AND

void Cpu::and(__uint8 d)
{
	__bit z;

	A.set(A.get() & d);
	z = !A.get();
	set_flags(z, 0, 1, 0);
}

void Cpu::and(Register8 *r1)
{
	return and(r1->get());
}

// OR

void Cpu::or(__uint8 d)
{
	__bit z;

	A.set(A.get() | d);
	z = !A.get();
	set_flags(z, 0, 0, 0);
}

void Cpu::or(Register8 *r1)
{
	return or(r1->get());
}

// XOR

void Cpu::xor(__uint8 d)
{
	__bit z;

	A.set(A.get() ^ d);
	z = !A.get();
	set_flags(z, 0, 0, 0);
}

void Cpu::xor(Register8 *r1)
{
	return xor(r1->get());
}

// CP

void Cpu::cp(__uint8 d)
{
	__bit z, h, c;
	__uint8 r1, result;

	if (d == 192)
		bool blarg_int_test_bp = 1;

	r1 = A.get();
	h = check_borrow(r1 << 4, d << 4);
	c = check_borrow(r1, d);
	result = r1 - d;
	z = !result;
	set_flags(z, 1, h, c);
}

void Cpu::cp(Register8 *r1)
{
	return cp(r1->get());
}

void Cpu::cp(__uint16 a)
{
	return cp(read(a));
}

// CB SET

void Cpu::set(Register8 *r1, __uint8 idx)
{
	assert(idx < 8);
	r1->set_bit(idx);
}

void Cpu::set(__uint16 a, __uint8 idx)
{
	write(a, (read(a) | (1 << idx)));
}

// CB BIT

void Cpu::bit(Register8 *r1, __uint8 idx)
{
	__bit z, c;
	__uint8 result;

	result = r1->get_bit(idx);
	z = !result;
	c = get_cf();
	set_flags(z, 0, 1, c);
}

void Cpu::bit(__uint16 a, __uint8 idx)
{
	__bit z, c;
	__uint8 result;

	MACHINE_CYCLES(1);

	result = (read(a) & (1 << idx));
	z = !result;
	c = get_cf();
	set_flags(z, 0, 1, c);
}

// CB RES

void Cpu::res(Register8 *r1, __uint8 idx)
{
	r1->clear_bit(idx);
}

void Cpu::res(__uint16 a, __uint8 idx)
{
	write(a, read(a) & ~(1U << idx));
}

// A OPS

void Cpu::rlc(__uint16 a)
{
	__bit z, c;
	__uint8 old_val, new_val;
	
	old_val = read(a);
	c = old_val & (1 << 7);
	new_val = (old_val << 1) | (__uint8)c;
	write(a, new_val);
	z = !new_val;
	set_flags(z, 0, 0, c);
}

void Cpu::rlc(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(7);

	r1->set(static_cast<__uint8>((r1->get() << 1) | (__uint8)(c)));

	z = !r1->get();

	set_flags(z, 0, 0, c);
}

void Cpu::rlca()
{
	rlc(&A);
	set_flags(0, 0, 0, get_cf());
}

void Cpu::rl(__uint16 a)
{
	__bit z, c;
	__uint8 old_val, new_val;

	old_val = read(a);
	new_val = old_val << 1;
	c = old_val & (1 << 7);
	new_val = get_cf() ? set_bit8(new_val, 0) : res_bit8(new_val, 0);
	write(a, new_val);
	z = !new_val;
	set_flags(z, 0, 0, c);
}

void Cpu::rl(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(7);
	r1->set(static_cast<__uint8>(r1->get() << 1));
	r1->set(get_cf() ? set_bit8(r1->get(), 0) : res_bit8(r1->get(), 0));
	z = !r1->get();
	set_flags(z, 0, 0, c);
}

void Cpu::rla()
{
	rl(&A);
	set_flags(0, 0, 0, get_cf());
}

void Cpu::rrc(__uint16 a)
{
	__bit z, c;
	__uint8 old_val, new_val;

	old_val = read(a);
	new_val = (old_val >> 1) | (old_val << 7);
	c = old_val & 0x01;
	write(a, new_val);
	z = !new_val;
	set_flags(z, 0, 0, c);
}

void Cpu::rrc(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(0);

	r1->set(static_cast<__uint8>((r1->get() >> 1) | (r1->get_bit(0) << 7)));

	z = !r1->get();
	set_flags(z, 0, 0, c);
}

void Cpu::rrca()
{
	rrc(&A);
	set_flags(0, 0, 0, get_cf());
}

void Cpu::rr(__uint16 a)
{
	__bit z, c;
	__uint8 old_carry, old_val, new_val;

	old_carry = get_cf();
	old_val = read(a);
	new_val = (old_val >> 1) | (old_carry << 7);

	c = old_val & 0x01;

	write(a, new_val);

	z = !new_val;

	set_flags(z, 0, 0, c);
}

void Cpu::rr(Register8 *r1)
{
	__bit z, c;
	__uint8 oldcarry = get_cf();

	c = r1->get_bit(0);

	r1->set(static_cast<__uint8>((r1->get() >> 1) | (oldcarry << 7)));

	z = !r1->get();

	set_flags(z, 0, 0, c);
}

void Cpu::rra()
{
	rr(&A);
	set_flags(0, 0, 0, get_cf());
}

void Cpu::sla(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(7);

	r1->set(static_cast<__uint8>((r1->get() << 1) & 0xfe));
	r1->clear_bit(0);

	z = !r1->get();

	set_flags(z, 0, 0, c);
}

void Cpu::sla(__uint16 a)
{
	__bit z, c;
	__uint8 data = read(a);

	c = !!(data & (1 << 7));

	write(a, static_cast<__uint8>((data << 1) & 0xfe));

	z = !mem->read(a);

	set_flags(z, 0, 0, c);
}

void Cpu::sra(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(0);

	r1->set(static_cast<__uint8>((r1->get() >> 1) | (r1->get() & 0x80)));

	z = !r1->get();

	set_flags(z, 0, 0, c);
}

// According to the opcode manual, the flags for this are Z000
void Cpu::sra(__uint16 a)
{
	__bit z, c;
	__uint8 old_val, new_val;

	old_val = read(a);
	new_val = (old_val >> 1) | (old_val & 0x80);

	c = old_val & 0x01;

	write(a, new_val);

	z = !new_val;
	set_flags(z, 0, 0, c);
}

void Cpu::srl(Register8 *r1)
{
	__bit z, c;

	c = r1->get_bit(0);

	r1->set(static_cast<__uint8>(r1->get() >> 1));

	z = !r1->get();

	set_flags(z, 0, 0, c);
}

void Cpu::srl(__uint16 a)
{
	__bit z, c;
	__uint8 old_val, new_val;
	
	old_val = read(a);
	new_val = old_val >> 1;

	write(a, new_val);

	z = !new_val;
	c = old_val & 0x01;

	set_flags(z, 0, 0, c);
}

// JMP

void Cpu::jp(__uint16 a)
{
	MACHINE_CYCLES(1);

#if LOUD_CPU
	if (a == 0x0150)
		std::cout << "Jumped to game cartridge code" << std::endl;
#endif

	PC.set(a);
}

void Cpu::jpnz(__uint16 a)
{
	if (!get_zf())
		return jp(a);
}

void Cpu::jpz(__uint16 a)
{
	if (get_zf())
		return jp(a);
}

void Cpu::jpnc(__uint16 a)
{
	if (!get_cf())
		return jp(a);
}

void Cpu::jpc(__uint16 a)
{
	if (get_cf())
		return jp(a);
}

void Cpu::jr(__int8 a)
{
	MACHINE_CYCLES(1);

	PC.set(PC.get() + a);
}

void Cpu::jrnz(__int8 a)
{
	if (!get_zf())
		return jr(a);
}

void Cpu::jrz(__int8 a)
{
	if (get_zf())
		return jr(a);
}

void Cpu::jrnc(__int8 a)
{
	if (!get_cf())
		return jr(a);
}

void Cpu::jrc(__int8 a)
{
	if (get_cf())
		return jr(a);
}

void Cpu::call(__uint16 a)
{
	push(&PC);
	PC.set(a); // 1 cycle off, cant use jp :)
}

void Cpu::callnz(__uint16 a)
{
	if (!get_zf())
		return call(a);
}

void Cpu::callz(__uint16 a)
{
	if (get_zf())
		return call(a);
}

void Cpu::callnc(__uint16 a)
{
	if (!get_cf())
		return call(a);
}

void Cpu::callc(__uint16 a)
{
	if (get_cf())
		return call(a);
}

void Cpu::ret()
{
	__uint16 address;

	address = (read(SP.get() + 2) << 8) | read(SP.get() + 1);
	SP.set(SP.get() + 2);
	jp(address);
}

void Cpu::retnz()
{
	if (!get_zf())
		return ret();
}

void Cpu::retz()
{
	if (get_zf())
		return ret();
}

void Cpu::retnc()
{
	if (!get_cf())
		return ret();
}

void Cpu::retc()
{
	if (get_cf())
		return ret();
}

void Cpu::reti()
{
	ret();
	ei();
}

void Cpu::rst(__uint16 a)
{
	call(a);
}

void Cpu::swap(Register8 *r1)
{
	__bit z;

	r1->set(((r1->get() & 0x0f) << 4) | (r1->get() >> 4));
	z = !r1->get();

	set_flags(z, 0, 0, 0);
}

void Cpu::swap(__uint16 a)
{
	__bit z;
	__uint8 old_val, new_val;

	MACHINE_CYCLES(1);

	old_val = read(a);
	new_val = ((old_val & 0x0f) << 4) | (old_val >> 4);

	write(a, new_val);

	z = !new_val;

	set_flags(z, 0, 0, 0);
}

void Cpu::daa() 
{
	// Im gonna assume most games don't use this
	// Decimal adjust register A so that the correct representation of Binary Coded Decimal is obtained.
	// 0100(4) 1000(8) -> 48 

	if (get_nf())
	{
		A.set(A.get() - (get_cf() * 0x60) - (get_hf() * 0x06));
	}
	else
	{
		if (get_cf() || A.get() > 0x99)
		{
			A.set(A.get() + 0x60);
			set_cf();
		}
		if (get_hf() || (A.get() & 0x0F) > 0x09)
		{
			A.set(A.get() + 0x06);
		}
	}
	
	if (!A.get())
		set_zf();
	else
		clear_zf();
	clear_hf();
}

void Cpu::ccf()
{
	__bit z, c;

	c = !get_cf();
	z = get_zf();
	set_flags(z, 0, 0, c);
}

void Cpu::scf()
{
	__bit z, c;

	c = 1;
	z = get_zf();
	set_flags(z, 0, 0, c);
}

void Cpu::cpl()
{
	__bit z, c;

	A.set(~(A.get()));
	z = get_zf();
	c = get_cf();
	set_flags(z, 1, 1, c);
}

void Cpu::nop()
{
}

void Cpu::hlt()
{
	// Power down CPU until interrupt occurs. Use this whenever possible to reduce energy consumption
	if (!(*mmu->IF))
		return;
	PC.set(PC.get() - 1);
}

void Cpu::stop()
{
	// Halt CPU & LCD display until button is pressed
}

void Cpu::di()
{
	interrupt_master_enable = false;
}

void Cpu::ei()
{
	// Note: interrupts are disabled until the instruction after EI, due to a hardware issue
	interrupt_master_enable = true;
}

void Cpu::set_zf()
{
	F.set_bit(7);
}

void Cpu::clear_zf()
{
	F.clear_bit(7);
}

__uint8 Cpu::get_zf()
{
	return F.get_bit(7);
}

void Cpu::set_nf()
{
	F.set_bit(6);
}

void Cpu::clear_nf()
{
	F.clear_bit(6);
}

__uint8 Cpu::get_nf()
{
	return F.get_bit(6);
}

void Cpu::set_hf()
{
	F.set_bit(5);
}

void Cpu::clear_hf()
{
	F.clear_bit(5);
}

__uint8 Cpu::get_hf()
{
	return F.get_bit(5);
}

void Cpu::set_cf()
{
	F.set_bit(4);
}

void Cpu::clear_cf()
{
	F.clear_bit(4);
}

__uint8 Cpu::get_cf()
{
	return F.get_bit(4);
}

// Z, N, H, C
void Cpu::set_flags(__uint8 z, __uint8 n, __uint8 h, __uint8 c)
{
	F.set((z << 7) | (n << 6) | (h << 5) | (c << 4));
}

void test_cpu()
{
	Cpu cpu(new Mmu(new Memory()));
	cpu.stress_test();
}

void test_timer()
{
	Mmu *mmu = new Mmu(new Memory());
	Cpu *cpu = new Cpu(mmu);
	cpu->mem->write(0xff07, 0x04); // Enable timer, set frequency to 4096
	Timer *timer = new Timer(mmu);

	mmu->write(0x00, 0xFB);

	for (unsigned int i = 1; i < 100; i++)
	{
		mmu->write(i, 0x00);
	}
	mmu->write(101, 0xc3);
	mmu->write(102, 0x00);
	mmu->write(103, 0x00);

	while (true)
	{
		cpu->decode(cpu->fetch8());
		timer->update(cpu->instruction_cycles);
		cpu->probe_interrupts();
		cpu->cycles += cpu->instruction_cycles;
		cpu->instruction_cycles = 0;
	}
}



