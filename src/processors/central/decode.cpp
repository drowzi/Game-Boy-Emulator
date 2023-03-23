#include "common/stdafx.h"
#include "processors/central/cpu.h"

// Fetch & Decode 0xCBXX
void Cpu::decode_ext(__uint8 op)
{
	//std::cout << std::hex << static_cast<int>(op) << std::endl;
	switch (op)
	{
	case 0x00:
		return rlc(&B);
	case 0x01:
		return rlc(&C);
	case 0x02:
		return rlc(&D);
	case 0x03:
		return rlc(&E);
	case 0x04:
		return rlc(&H);
	case 0x05:
		return rlc(&L);
	case 0x06:
		return rlc(HL.get());
	case 0x07:
		return rlc(&A);
	case 0x08:
		return rrc(&B);
	case 0x09:
		return rrc(&C);
	case 0x0A:
		return rrc(&D);
	case 0x0B:
		return rrc(&E);
	case 0x0C:
		return rrc(&H);
	case 0x0D:
		return rrc(&L);
	case 0x0E:
		return rrc(HL.get());
	case 0x0F:
		return rrc(&A);

	case 0x10:
		return rl(&B);
	case 0x11:
		return rl(&C);
	case 0x12:
		return rl(&D);
	case 0x13:
		return rl(&E);
	case 0x14:
		return rl(&H);
	case 0x15:
		return rl(&L);
	case 0x16:
		return rl(HL.get());
	case 0x17:
		return rl(&A);
	case 0x18:
		return rr(&B);
	case 0x19:
		return rr(&C);
	case 0x1A:
		return rr(&D);
	case 0x1B:
		return rr(&E);
	case 0x1C:
		return rr(&H);
	case 0x1D:
		return rr(&L);
	case 0x1E:
		return rr(HL.get());
	case 0x1F:
		return rr(&A);

	case 0x20:
		return sla(&B);
	case 0x21:
		return sla(&C);
	case 0x22:
		return sla(&D);
	case 0x23:
		return sla(&E);
	case 0x24:
		return sla(&H);
	case 0x25:
		return sla(&L);
	case 0x26:
		return sla(HL.get());
	case 0x27:
		return sla(&A);
	case 0x28:
		return sra(&B);
	case 0x29:
		return sra(&C);
	case 0x2A:
		return sra(&D);
	case 0x2B:
		return sra(&E);
	case 0x2C:
		return sra(&H);
	case 0x2D:
		return sra(&L);
	case 0x2E:
		return sra(HL.get());
	case 0x2F:
		return sra(&A);

	case 0x30:
		return swap(&B);
	case 0x31:
		return swap(&C);
	case 0x32:
		return swap(&D);
	case 0x33:
		return swap(&E);
	case 0x34:
		return swap(&H);
	case 0x35:
		return swap(&L);
	case 0x36:
		return swap(HL.get());
	case 0x37:
		return swap(&A);
	case 0x38:
		return srl(&B);
	case 0x39:
		return srl(&C);
	case 0x3A:
		return srl(&D);
	case 0x3B:
		return srl(&E);
	case 0x3C:
		return srl(&H);
	case 0x3D:
		return srl(&L);
	case 0x3E:
		return srl(HL.get());
	case 0x3F:
		return srl(&A);

	case 0x40:
		return bit(&B, 0);
	case 0x41:
		return bit(&C, 0);
	case 0x42:
		return bit(&D, 0);
	case 0x43:
		return bit(&E, 0);
	case 0x44:
		return bit(&H, 0);
	case 0x45:
		return bit(&L, 0);
	case 0x46:
		return bit(HL.get(), 0);
	case 0x47:
		return bit(&A, 0);
	case 0x48:
		return bit(&B, 1);
	case 0x49:
		return bit(&C, 1);
	case 0x4A:
		return bit(&D, 1);
	case 0x4B:
		return bit(&E, 1);
	case 0x4C:
		return bit(&H, 1);
	case 0x4D:
		return bit(&L, 1);
	case 0x4E:
		return bit(HL.get(), 1);
	case 0x4F:
		return bit(&A, 1);

	case 0x50:
		return bit(&B, 2);
	case 0x51:
		return bit(&C, 2);
	case 0x52:
		return bit(&D, 2);
	case 0x53:
		return bit(&E, 2);
	case 0x54:
		return bit(&H, 2);
	case 0x55:
		return bit(&L, 2);
	case 0x56:
		return bit(HL.get(), 2);
	case 0x57:
		return bit(&A, 2);
	case 0x58:
		return bit(&B, 3);
	case 0x59:
		return bit(&C, 3);
	case 0x5A:
		return bit(&D, 3);
	case 0x5B:
		return bit(&E, 3);
	case 0x5C:
		return bit(&H, 3);
	case 0x5D:
		return bit(&L, 3);
	case 0x5E:
		return bit(HL.get(), 3);
	case 0x5F:
		return bit(&A, 3);

	case 0x60:
		return bit(&B, 4);
	case 0x61:
		return bit(&C, 4);
	case 0x62:
		return bit(&D, 4);
	case 0x63:
		return bit(&E, 4);
	case 0x64:
		return bit(&H, 4);
	case 0x65:
		return bit(&L, 4);
	case 0x66:
		return bit(HL.get(), 4);
	case 0x67:
		return bit(&A, 4);
	case 0x68:
		return bit(&B, 5);
	case 0x69:
		return bit(&C, 5);
	case 0x6A:
		return bit(&D, 5);
	case 0x6B:
		return bit(&E, 5);
	case 0x6C:
		return bit(&H, 5);
	case 0x6D:
		return bit(&L, 5);
	case 0x6E:
		return bit(HL.get(), 5);
	case 0x6F:
		return bit(&A, 5);

	case 0x70:
		return bit(&B, 6);
	case 0x71:
		return bit(&C, 6);
	case 0x72:
		return bit(&D, 6);
	case 0x73:
		return bit(&E, 6);
	case 0x74:
		return bit(&H, 6);
	case 0x75:
		return bit(&L, 6);
	case 0x76:
		return bit(HL.get(), 6);
	case 0x77:
		return bit(&A, 6);
	case 0x78:
		return bit(&B, 7);
	case 0x79:
		return bit(&C, 7);
	case 0x7A:
		return bit(&D, 7);
	case 0x7B:
		return bit(&E, 7);
	case 0x7C:
		return bit(&H, 7);
	case 0x7D:
		return bit(&L, 7);
	case 0x7E:
		return bit(HL.get(), 7);
	case 0x7F:
		return bit(&A, 7);

	case 0x80:
		return res(&B, 0);
	case 0x81:
		return res(&C, 0);
	case 0x82:
		return res(&D, 0);
	case 0x83:
		return res(&E, 0);
	case 0x84:
		return res(&H, 0);
	case 0x85:
		return res(&L, 0);
	case 0x86:
		return res(HL.get(), 0);
	case 0x87:
		return res(&A, 0);
	case 0x88:
		return res(&B, 1);
	case 0x89:
		return res(&C, 1);
	case 0x8A:
		return res(&D, 1);
	case 0x8B:
		return res(&E, 1);
	case 0x8C:
		return res(&H, 1);
	case 0x8D:
		return res(&L, 1);
	case 0x8E:
		return res(HL.get(), 1);
	case 0x8F:
		return res(&A, 1);

	case 0x90:
		return res(&B, 2);
	case 0x91:
		return res(&C, 2);
	case 0x92:
		return res(&D, 2);
	case 0x93:
		return res(&E, 2);
	case 0x94:
		return res(&H, 2);
	case 0x95:
		return res(&L, 2);
	case 0x96:
		return res(HL.get(), 2);
	case 0x97:
		return res(&A, 2);
	case 0x98:
		return res(&B, 3);
	case 0x99:
		return res(&C, 3);
	case 0x9A:
		return res(&D, 3);
	case 0x9B:
		return res(&E, 3);
	case 0x9C:
		return res(&H, 3);
	case 0x9D:
		return res(&L, 3);
	case 0x9E:
		return res(HL.get(), 3);
	case 0x9F:
		return res(&A, 3);

	case 0xA0:
		return res(&B, 4);
	case 0xA1:
		return res(&C, 4);
	case 0xA2:
		return res(&D, 4);
	case 0xA3:
		return res(&E, 4);
	case 0xA4:
		return res(&H, 4);
	case 0xA5:
		return res(&L, 4);
	case 0xA6:
		return res(HL.get(), 4);
	case 0xA7:
		return res(&A, 4);
	case 0xA8:
		return res(&B, 5);
	case 0xA9:
		return res(&C, 5);
	case 0xAA:
		return res(&D, 5);
	case 0xAB:
		return res(&E, 5);
	case 0xAC:
		return res(&H, 5);
	case 0xAD:
		return res(&L, 5);
	case 0xAE:
		return res(HL.get(), 5);
	case 0xAF:
		return res(&A, 5);

	case 0xB0:
		return res(&B, 6);
	case 0xB1:
		return res(&C, 6);
	case 0xB2:
		return res(&D, 6);
	case 0xB3:
		return res(&E, 6);
	case 0xB4:
		return res(&H, 6);
	case 0xB5:
		return res(&L, 6);
	case 0xB6:
		return res(HL.get(), 6);
	case 0xB7:
		return res(&A, 6);
	case 0xB8:
		return res(&B, 7);
	case 0xB9:
		return res(&C, 7);
	case 0xBA:
		return res(&D, 7);
	case 0xBB:
		return res(&E, 7);
	case 0xBC:
		return res(&H, 7);
	case 0xBD:
		return res(&L, 7);
	case 0xBE:
		return res(HL.get(), 7);
	case 0xBF:
		return res(&A, 7);

	case 0xC0:
		return set(&B, 0);
	case 0xC1:
		return set(&C, 0);
	case 0xC2:
		return set(&D, 0);
	case 0xC3:
		return set(&E, 0);
	case 0xC4:
		return set(&H, 0);
	case 0xC5:
		return set(&L, 0);
	case 0xC6:
		return set(HL.get(), 0);
	case 0xC7:
		return set(&A, 0);
	case 0xC8:
		return set(&B, 1);
	case 0xC9:
		return set(&C, 1);
	case 0xCA:
		return set(&D, 1);
	case 0xCB:
		return set(&E, 1);
	case 0xCC:
		return set(&H, 1);
	case 0xCD:
		return set(&L, 1);
	case 0xCE:
		return set(HL.get(), 1);
	case 0xCF:
		return set(&A, 1);

	case 0xD0:
		return set(&B, 2);
	case 0xD1:
		return set(&C, 2);
	case 0xD2:
		return set(&D, 2);
	case 0xD3:
		return set(&E, 2);
	case 0xD4:
		return set(&H, 2);
	case 0xD5:
		return set(&L, 2);
	case 0xD6:
		return set(HL.get(), 2);
	case 0xD7:
		return set(&A, 2);
	case 0xD8:
		return set(&B, 3);
	case 0xD9:
		return set(&C, 3);
	case 0xDA:
		return set(&D, 3);
	case 0xDB:
		return set(&E, 3);
	case 0xDC:
		return set(&H, 3);
	case 0xDD:
		return set(&L, 3);
	case 0xDE:
		return set(HL.get(), 3);
	case 0xDF:
		return set(&A, 3);

	case 0xE0:
		return set(&B, 4);
	case 0xE1:
		return set(&C, 4);
	case 0xE2:
		return set(&D, 4);
	case 0xE3:
		return set(&E, 4);
	case 0xE4:
		return set(&H, 4);
	case 0xE5:
		return set(&L, 4);
	case 0xE6:
		return set(HL.get(), 4);
	case 0xE7:
		return set(&A, 4);
	case 0xE8:
		return set(&B, 5);
	case 0xE9:
		return set(&C, 5);
	case 0xEA:
		return set(&D, 5);
	case 0xEB:
		return set(&E, 5);
	case 0xEC:
		return set(&H, 5);
	case 0xED:
		return set(&L, 5);
	case 0xEE:
		return set(HL.get(), 5);
	case 0xEF:
		return set(&A, 5);

	case 0xF0:
		return set(&B, 6);
	case 0xF1:
		return set(&C, 6);
	case 0xF2:
		return set(&D, 6);
	case 0xF3:
		return set(&E, 6);
	case 0xF4:
		return set(&H, 6);
	case 0xF5:
		return set(&L, 6);
	case 0xF6:
		return set(HL.get(), 6);
	case 0xF7:
		return set(&A, 6);
	case 0xF8:
		return set(&B, 7);
	case 0xF9:
		return set(&C, 7);
	case 0xFA:
		return set(&D, 7);
	case 0xFB:
		return set(&E, 7);
	case 0xFC:
		return set(&H, 7);
	case 0xFD:
		return set(&L, 7);
	case 0xFE:
		return set(HL.get(), 7);
	case 0xFF:
		return set(&A, 7);

	default:
		std::cout << "Invalid opcode: 0xCB" << std::hex << static_cast<int>(op) << std::endl;
		abort();
	}
}

// Fetch & Decode 0xXX
void Cpu::decode(__uint8 op)
{
	//log << PC.get() << '\n';

	//if (PC.get() > 500)
		//std::cout << PC.get() << std::endl;
	if (PC.get() > 255)
		bool b = true;
	//std::cout << std::hex << static_cast<int>(op) << std::endl;

	switch (op)
	{
	case 0x00:
		return nop();
	case 0x01:
		return load(&BC, fetch16());
	case 0x02:
		return load(BC.get(), &A);
	case 0x03:
		return inc(&BC);
	case 0x04:
		return inc(&B);
	case 0x05:
		return dec(&B);
	case 0x06:
		return load(&B, fetch8());
	case 0x07:
		return rlca();
	case 0x08:
		return load(fetch16(), &SP);
	case 0x09:
		return add(&HL, &BC);
	case 0x0A:
		return load(&A, read(BC.get()));
	case 0x0B:
		return dec(&BC);
	case 0x0C:
		return inc(&C);
	case 0x0D:
		return dec(&C);
	case 0x0E:
		return load(&C, fetch8());
	case 0x0F:
		return rrca();

	case 0x10:
		return stop();
	case 0x11:
		return load(&DE, fetch16());
	case 0x12:
		return load(DE.get(), &A);
	case 0x13:
		return inc(&DE);
	case 0x14:
		return inc(&D);
	case 0x15:
		return dec(&D);
	case 0x16:
		return load(&D, fetch8());
	case 0x17:
		return rla();
	case 0x18:
		return jr(fetch8());
	case 0x19:
		return add(&HL, &DE);
	case 0x1A:
		return load(&A, read(DE.get()));
	case 0x1B:
		return dec(&DE);
	case 0x1C:
		return inc(&E);
	case 0x1D:
		return dec(&E);
	case 0x1E:
		return load(&E, fetch8());
	case 0x1F:
		return rra();

	case 0x20:
		return jrnz(fetch8());
	case 0x21:
		return load(&HL, fetch16());
	case 0x22:
		return loadi(HL.get(), &A);
	case 0x23:
		return inc(&HL);
	case 0x24:
		return inc(&H);
	case 0x25:
		return dec(&H);
	case 0x26:
		return load(&H, fetch8());
	case 0x27:
		return daa();
	case 0x28:
		return jrz(fetch8());
	case 0x29:
		return add(&HL, &HL);
	case 0x2A:
		return loadi(&A, read(HL.get()));
	case 0x2B:
		return dec(&HL);
	case 0x2C:
		return inc(&L);
	case 0x2D:
		return dec(&L);
	case 0x2E:
		return load(&L, fetch8());
	case 0x2F:
		return cpl();

	case 0x30:
		return jrnc(fetch8());
	case 0x31:
		return load(&SP, fetch16());
	case 0x32:
		return loadd(HL.get(), &A);
	case 0x33:
		return inc(&SP);
	case 0x34:
		return inc(HL.get());
	case 0x35:
		return dec(HL.get());
	case 0x36:
		return load(HL.get(), fetch8());
	case 0x37:
		return scf();
	case 0x38:
		return jrc(fetch8());
	case 0x39:
		return add(&HL, &SP);
	case 0x3A:
		return loadd(&A, read(HL.get()));
	case 0x3B:
		return dec(&SP);
	case 0x3C:
		return inc(&A);
	case 0x3D:
		return dec(&A);
	case 0x3E:
		return load(&A, fetch8());
	case 0x3F:
		return ccf();

	case 0x40:
		return load(&B, &B);
	case 0x41:
		return load(&B, &C);
	case 0x42:
		return load(&B, &D);
	case 0x43:
		return load(&B, &E);
	case 0x44:
		return load(&B, &H);
	case 0x45:
		return load(&B, &L);
	case 0x46:
		return load(&B, read(HL.get()));
	case 0x47:
		return load(&B, &A);
	case 0x48:
		return load(&C, &B);
	case 0x49:
		return load(&C, &C);
	case 0x4A:
		return load(&C, &D);
	case 0x4B:
		return load(&C, &E);
	case 0x4C:
		return load(&C, &H);
	case 0x4D:
		return load(&C, &L);
	case 0x4E:
		return load(&C, read(HL.get()));
	case 0x4F:
		return load(&C, &A);

	case 0x50:
		return load(&D, &B);
	case 0x51:
		return load(&D, &C);
	case 0x52:
		return load(&D, &D);
	case 0x53:
		return load(&D, &E);
	case 0x54:
		return load(&D, &H);
	case 0x55:
		return load(&D, &L);
	case 0x56:
		return load(&D, read(HL.get()));
	case 0x57:
		return load(&D, &A);
	case 0x58:
		return load(&E, &B);
	case 0x59:
		return load(&E, &C);
	case 0x5A:
		return load(&E, &D);
	case 0x5B:
		return load(&E, &E);
	case 0x5C:
		return load(&E, &H);
	case 0x5D:
		return load(&E, &L);
	case 0x5E:
		return load(&E, read(HL.get()));
	case 0x5F:
		return load(&E, &A);

	case 0x60:
		return load(&H, &B);
	case 0x61:
		return load(&H, &C);
	case 0x62:
		return load(&H, &D);
	case 0x63:
		return load(&H, &E);
	case 0x64:
		return load(&H, &H);
	case 0x65:
		return load(&H, &L);
	case 0x66:
		return load(&H, read(HL.get()));
	case 0x67:
		return load(&H, &A);
	case 0x68:
		return load(&L, &B);
	case 0x69:
		return load(&L, &C);
	case 0x6A:
		return load(&L, &D);
	case 0x6B:
		return load(&L, &E);
	case 0x6C:
		return load(&L, &H);
	case 0x6D:
		return load(&L, &L);
	case 0x6E:
		return load(&L, read(HL.get()));
	case 0x6F:
		return load(&L, &A);

	case 0x70:
		return load(HL.get(), &B);
	case 0x71:
		return load(HL.get(), &C);
	case 0x72:
		return load(HL.get(), &D);
	case 0x73:
		return load(HL.get(), &E);
	case 0x74:
		return load(HL.get(), &H);
	case 0x75:
		return load(HL.get(), &L);
	case 0x76:
		return hlt();
	case 0x77:
		return load(HL.get(), &A);
	case 0x78:
		return load(&A, &B);
	case 0x79:
		return load(&A, &C);
	case 0x7A:
		return load(&A, &D);
	case 0x7B:
		return load(&A, &E);
	case 0x7C:
		return load(&A, &H);
	case 0x7D:
		return load(&A, &L);
	case 0x7E:
		return load(&A, read(HL.get()));
	case 0x7F:
		return load(&A, &A);

	case 0x80:
		return add(&A, &B);
	case 0x81:
		return add(&A, &C);
	case 0x82:
		return add(&A, &D);
	case 0x83:
		return add(&A, &E);
	case 0x84:
		return add(&A, &H);
	case 0x85:
		return add(&A, &L);
	case 0x86:
		return add(&A, read(HL.get()));
	case 0x87:
		return add(&A, &A);
	case 0x88:
		return adc(&A, &B);
	case 0x89:
		return adc(&A, &C);
	case 0x8A:
		return adc(&A, &D);
	case 0x8B:
		return adc(&A, &E);
	case 0x8C:
		return adc(&A, &H);
	case 0x8D:
		return adc(&A, &L);
	case 0x8E:
		return adc(read(HL.get()));
	case 0x8F:
		return adc(&A, &A);

	case 0x90:
		return sub(&B);
	case 0x91:
		return sub(&C);
	case 0x92:
		return sub(&D);
	case 0x93:
		return sub(&E);
	case 0x94:
		return sub(&H);
	case 0x95:
		return sub(&L);
	case 0x96:
		return sub(read(HL.get()));
	case 0x97:
		return sub(&A);
	case 0x98:
		return sbc(&A, &B);
	case 0x99:
		return sbc(&A, &C);
	case 0x9A:
		return sbc(&A, &D);
	case 0x9B:
		return sbc(&A, &E);
	case 0x9C:
		return sbc(&A, &H);
	case 0x9D:
		return sbc(&A, &L);
	case 0x9E:
		return sbc(read(HL.get()));
	case 0x9F:
		return sbc(&A, &A);

	case 0xA0:
		return and(&B);
	case 0xA1:
		return and(&C);
	case 0xA2:
		return and(&D);
	case 0xA3:
		return and(&E);
	case 0xA4:
		return and(&H);
	case 0xA5:
		return and(&L);
	case 0xA6:
		return and(read(HL.get()));
	case 0xA7:
		return and(&A);
	case 0xA8:
		return xor(&B);
	case 0xA9:
		return xor(&C);
	case 0xAA:
		return xor(&D);
	case 0xAB:
		return xor(&E);
	case 0xAC:
		return xor(&H);
	case 0xAD:
		return xor(&L);
	case 0xAE:
		return xor(read(HL.get()));
	case 0xAF:
		return xor(&A);

	case 0xB0:
		return or(&B);
	case 0xB1:
		return or(&C);
	case 0xB2:
		return or(&D);
	case 0xB3:
		return or(&E);
	case 0xB4:
		return or(&H);
	case 0xB5:
		return or(&L);
	case 0xB6:
		return or(read(HL.get()));
	case 0xB7:
		return or(&A);
	case 0xB8:
		return cp(&B);
	case 0xB9:
		return cp(&C);
	case 0xBA:
		return cp(&D);
	case 0xBB:
		return cp(&E);
	case 0xBC:
		return cp(&H);
	case 0xBD:
		return cp(&L);
	case 0xBE:
		return cp(read(HL.get()));
	case 0xBF:
		return cp(&A);

	case 0xC0:
		return retnz();
	case 0xC1:
		return pop(&BC);
	case 0xC2:
		return jpnz(fetch16());
	case 0xC3:
		return jp(fetch16());
	case 0xC4:
		return callnz(fetch16());
	case 0xC5:
		return push(&BC);
	case 0xC6:
		return add(&A, fetch8());
	case 0xC7:
		return rst(0x00);
	case 0xC8:
		return retz();
	case 0xC9:
		return ret();
	case 0xCA:
		return jpz(fetch16());
	case 0xCB:
		return decode_ext(fetch8());
	case 0xCC:
		return callz(fetch16());
	case 0xCD:
		return call(fetch16());
	case 0xCE:
		return adc(fetch8());
	case 0xCF:
		return rst(0x08);

	case 0xD0:
		return retnc();
	case 0xD1:
		return pop(&DE);
	case 0xD2:
		return jpnc(fetch16());
	case 0xD3:
		goto HOLE;
	case 0xD4:
		return callnc(fetch16());
	case 0xD5:
		return push(&DE);
	case 0xD6:
		return sub(fetch8());
	case 0xD7:
		return rst(0x10);
	case 0xD8:
		return retc();
	case 0xD9:
		return reti();
	case 0xDA:
		return jpc(fetch16());
	case 0xDB:
		goto HOLE;
	case 0xDC:
		return callc(fetch16());
	case 0xDD:
		goto HOLE;
	case 0xDE:
		return sbc(fetch8());
	case 0xDF:
		return rst(0x18);

	case 0xE0:
		return loadh(fetch8(), &A);
	case 0xE1:
		return pop(&HL);
	case 0xE2:
		return loadh(C.get(), &A);
	case 0xE3:
		goto HOLE;
	case 0xE4:
		goto HOLE;
	case 0xE5:
		return push(&HL);
	case 0xE6:
		return and(fetch8());
	case 0xE7:
		return rst(0x20);
	case 0xE8:
		return adds(fetch8());
	case 0xE9:
		return jp(HL.get());
	case 0xEA:
		return load(fetch16(), &A); 
	case 0xEB:
		goto HOLE;
	case 0xEC:
		goto HOLE;
	case 0xED:
		goto HOLE;
	case 0xEE:
		return xor(fetch8());
	case 0xEF:
		return rst(0x28);

	case 0xF0:
		return loadh(&A, fetch8()); 
	case 0xF1:
		return pop(&AF);
	case 0xF2:
		return loadh(&A, C.get()); 
	case 0xF3:
		return di();
	case 0xF4:
		goto HOLE;
	case 0xF5:
		return push(&AF);
	case 0xF6:
		return or(fetch8());
	case 0xF7:
		return rst(0x30);
	case 0xF8:
		return loads(fetch8()); // LD HL, SP+R8
	case 0xF9:
		return load(&SP, &HL);
	case 0xFA:
		return load(&A, mem->read(fetch16())); // LD A, (a16) ; read the memory so there wouldnt need to be a conversion
	case 0xFB:
		return ei();
	case 0xFC:
		goto HOLE;
	case 0xFD:
		goto HOLE;
	case 0xFE:
		return cp(fetch8());
	case 0xFF:
		return rst(0x38);

	HOLE:
	default:
		std::cout << "Invalid opcode: 0x" << std::hex << static_cast<int>(op) << std::endl;
		abort();
	}
}