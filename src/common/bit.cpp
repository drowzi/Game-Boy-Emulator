#include "common/stdafx.h"
#include "common/bit.h"

inline __uint8 set_bit8(__uint8 d, __uint8 idx)
{
	return d | (1U << idx);
}

inline __uint8 res_bit8(__uint8 d, __uint8 idx)
{
	return d & ~(1U << idx);
}

bool get_bit8(__uint8 d, __uint8 idx)
{
	static const __uint8 bitmask8[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };
	return static_cast<bool>((d & bitmask8[idx]));
}

void test_bit()
{
	__uint8 x = 0xff;
	assert(get_bit8(x, 0) == 1);
	assert(get_bit8(x, 1) == 1);
	assert(get_bit8(x, 2) == 1);
	assert(get_bit8(x, 3) == 1);
	assert(get_bit8(x, 4) == 1);
	assert(get_bit8(x, 5) == 1);
	assert(get_bit8(x, 6) == 1);
	assert(get_bit8(x, 7) == 1);

	x = res_bit8(x, 7);
	assert(get_bit8(x, 7) == 0);
	x = res_bit8(x, 0);
	assert(get_bit8(x, 0) == 0);

	x = set_bit8(x, 7);
	assert(get_bit8(x, 7) == 1);
	x = set_bit8(x, 0);
	assert(get_bit8(x, 0) == 1);
}

