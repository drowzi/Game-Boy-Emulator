
#include "common/stdafx.h"
#include "cli.h"
#include "gameboy.h"

// If you continue to get stuck, use this: https://github.com/jgilchrist/gbemu

int main(int argc, char *argv[])
{
	gb_opts opts;

	if (!parse_opts(argc, argv, &opts)) 
		return usage();

	GameBoy gameboy(&opts);
	gameboy.run();

	return 0;
}