// SDLSandbox.cpp : Defines the entry point for the console application.
//

#include "common/stdafx.h"
#include "gameboy.h"

#include <SDL.h>
#include <time.h>
#include <iostream>

const char *tetris = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\ROM\\Tetris (World).gb";
const char *blarggs_cpu_instr = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\cpu_instrs.gb";
const char *blarggs_int_time = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\interrupt_time\\interrupt_time.gb";
const char *blarggs_cpu_1 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\01-special.gb";
const char *blarggs_cpu_2 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\02-interrupts.gb";
const char *blarggs_cpu_3 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\03-op sp,hl.gb";
const char *blarggs_cpu_4 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\04-op r,imm.gb";
const char *blarggs_cpu_5 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\05-op rp.gb";
const char *blarggs_cpu_6 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\06-ld r,r.gb";
const char *blarggs_cpu_7 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\07-jr,jp,call,ret,rst.gb";
const char *blarggs_cpu_8 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\08-misc instrs.gb";
const char *blarggs_cpu_9 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\09-op r,r.gb";
const char *blarggs_cpu_10 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\10-bit ops.gb";
const char *blarggs_cpu_11 = "C:\\Users\\Allen\\Desktop\\Programming\\C++\\GameBoyEmulatorHardware\\GameBoyEmulatorHardware\\blarggs-tests\\gb-test-roms-master\\cpu_instrs\\individual\\11-op a,(hl).gb";

/*
// Note: if your main doesn't match this main's signature, there will be a linker error unless you undef main
int main(int argc, char *argv[])
{

	GameBoy gameboy(DISABLE_BOOTLOAD);
	gameboy.run(tetris);
	//gameboy.run(blarggs_cpu_6);

    return 0;
}
*/

