
#include "common/stdafx.h"
#include "memory/memory.h"

#include <cstring>


Memory::Memory()
{
	// Memory was zeroed by default when I had the boot writer included in this object, no idea why.
	memset(this->memory, 0, 0xffff);
}

Memory::~Memory()
{
}

// Note: these primitive access methods include temporary debug hooks

__uint8 Memory::read(__uint16 address)
{
	return static_cast<__uint8>(this->memory[address]);
}

void Memory::write(__uint16 address, __uint8 data)
{
	if (address == 0xFF02 && data == 0x81) {
		std::cout << memory[0xff01];
	}
	if (address == 0xff01)
		bool b = true;

	this->memory[address] = data;
}