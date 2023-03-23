#pragma once

#include "common/stdafx.h"

class Register
{
public:
	Register() {}
	~Register() {}
};

class Register8 : Register
{
public:
	Register8() { data = 0; }
	Register8(const char *id) : id(id) { data = 0; }
	~Register8() {}
	void set(__uint8 to) 
	{
		data = to; 
	}
	__uint8 get()
	{
		return data;
	}

	bool get_bit(__uint8 idx)
	{
		return !!(data & bitmask8[idx]);
	}

	void set_bit(__uint8 idx)
	{
		data |= 1U << idx;
	}

	void clear_bit(__uint8 idx)
	{
		data &= ~(1U << idx);
	}

	friend std::ostream& operator<< (std::ostream &out, Register8 *r)
	{
		out << static_cast<int>(r->data);
		return out;
	}
	const char *id;
private:
	const __uint8 bitmask8[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };
	__uint8 data;
};

class Register16 : Register
{
public:
	Register16() 
	{ 
		this->hi = new Register8(); 
		this->lo = new Register8(); 
	}
	Register16(const char *id)
	{
		this->id = id;
		this->hi = new Register8();
		this->lo = new Register8();
	}
	Register16(const char *id, Register8 *hi, Register8 *lo) 
	{ 
		this->id = id;
		this->hi = hi; 
		this->lo = lo;
	}
	~Register16() {}
	void set(__uint16 to) 
	{ 
		lo->set(to & 0x00ff);
		hi->set((to & 0xff00) >> 8);
	}
	__uint16 get() {
		return (hi->get() << 8) | lo->get(); 
	}

	// This will all be redone later. It's ridiculous to waste like 10 instructions on conditions guarding bitwise ops, but it provides an alright interface
	bool get_bit(__uint8 idx)
	{
		return (idx <= 7) ? lo->get_bit(idx) : hi->get_bit(idx - 8);
	}

	void set_bit(__uint8 idx)
	{
		return (idx <= 7) ? lo->set_bit(idx) : hi->set_bit(idx - 8);
	}

	void clear_bit(__uint8 idx)
	{
		return (idx <= 7) ? lo->clear_bit(idx) : hi->clear_bit(idx - 8);
	}

	void operator=(Register16 *r1)
	{
		return this->set(r1->get());
	}

	void operator=(__uint16 to)
	{
		return this->set(to);
	}

	friend std::ostream& operator<< (std::ostream &out, Register16 *r)
	{
		out << r->get();
		return out;
	}
	const char *id;
private:
	Register8 *hi, *lo;
};

void test_registers();

