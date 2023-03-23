#include "common/stdafx.h"
#include "memory/register.h"

void test_registers()
{
	Register8 *A = new Register8("A");
	Register8 *B = new Register8("B");
	Register16 *AB = new Register16("AB", A, B);

	A->set(16);
	assert(A->get() == 16);

	B->set(127);
	assert(B->get() == 127);

	assert(AB->get() == ((A->get() << 8) | B->get()));

	AB->set(65535);

	assert(A->get() == 0xff);
	assert(B->get() == 0xff);
}
