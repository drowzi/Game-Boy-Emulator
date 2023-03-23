
#include "common/stdafx.h"
#include "cli.h"
#include "config.h"

#include <string>
#include <cstring>

using namespace std;

bool arg_eq(const char *arg, const char *opt1, const char *opt2)
{
	return strncmp(arg, opt1, strlen(opt1)) == 0 || strncmp(arg, opt2, strlen(opt2)) == 0;
}

// (todo): eat -'s so invalid commands can be recognized
int parse_opts(int argc, char *argv[], gb_opts *opts)
{
	int i;
	const char *arg;

	memset(opts, 0, sizeof(gb_opts));

	Config::parse("config.ini", *opts); //for baseline config

	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		if (arg_eq(arg, "-b", "--enable-bootload"))
		{
			opts->enable_bootload = true;
			opts->bootload_path = strchr(arg, '=') + 1;
		}
		else if (arg_eq(arg, "-d", "--enable-debug"))
		{
			opts->enable_debug = true;
		}
		else if (arg_eq(arg, "-m", "--enable-tilemap-view"))
		{
			opts->enable_tilemap_view = true;
		}
		else if (arg_eq(arg, "-r", "--set-renderer"))
		{
			opts->renderer = strchr(arg, '=') + 1;
		}
		else if (arg_eq(arg, "-h", "--help"))
		{
			return 0;
		}
		else
		{
			opts->rom_path = arg;
		}
	}

	if (opts->rom_path.empty())
		return 0;

	return 1;
}

int usage()
{
	cout << endl;
	cout << "Usage:" << endl;
	cout << " gameboy [options] <file> [...]" << endl;
	cout << endl;
	cout << "A GameBoy emulator for Windows." << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << " -b, --enable-bootload=<path>\t supply bootload rom" << endl;
	cout << " -d, --enable-debug\t\t enable debugging features" << endl;
	cout << " -t, --enable-unit-tests\t run all unit tests beforehand" << endl;
	cout << " -m, --enable-tilemap-view\t render the currently used tilemap" << endl;
	cout << " -r, --set-renderer=<renderer>\t set the renderer to sdl or gl (default=sdl)" << endl;
	cout << " -h, --help\t\t\t usage" << endl;
	return 0;
}
