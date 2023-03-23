#pragma once
#include "common/stdafx.h"

class Memory
{
public:
	Memory();
	~Memory();
	__uint8 Memory::read(__uint16 address);
	void Memory::write(__uint16 address, __uint8 data);

	// Memory is 0xffff + 0x0001 so that memory location 0xffff may be used
	__uint8 memory[0x10000];
};

