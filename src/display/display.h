#pragma once
#include "common/stdafx.h"
#include "render.h"

class Display
{
public:
	Display(Renderer *r);
	~Display();
	void Display::render();
	void Display::render_background(__uint8 *memory, __uint8 scx, __uint8 scy, __uint16 tile_map, __uint16 tile_data);
	void Display::push_px(__uint8 color);
	void Display::next_row();
	void Display::flush();
	Renderer *renderer;
	__uint8 screen[144][160], screenx, screeny;
};

