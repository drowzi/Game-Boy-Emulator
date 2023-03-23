
#pragma once

#include <string>

struct gb_opts
{
	bool enable_bootload;
	bool enable_debug;
	bool enable_tilemap_view;
	bool enable_window_display;
	std::string bootload_path;
	std::string debug_type;
	std::string debug_host;
	std::string renderer;
	std::string rom_path;
	std::string last_rom_path;
	int debug_port;
	int height, width;
	int joypad_a;
	int joypad_b;
	int joypad_start;
	int joypad_select;
	int joypad_up;
	int joypad_right;
	int joypad_down;
	int joypad_left;
};