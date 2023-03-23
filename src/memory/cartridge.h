#pragma once
#include "common/stdafx.h"

class Cartridge
{
public:
	Cartridge();
	~Cartridge();

	const char *file;
	char *title;
	__uint8 *ROM;
};

