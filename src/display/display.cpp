
#include "common/stdafx.h"
#include "display/display.h"


Display::Display(Renderer *r) : renderer(r)
{
}


Display::~Display()
{
}

void Display::render()
{
	unsigned int y_start, y_end, x_start, x_end;

	y_start = (renderer->height / 2) - (144 / 2);
	y_end = (renderer->height / 2) + (144 / 2);
	x_start = (renderer->width / 2) - (160 / 2);
	x_end = (renderer->width / 2) + (160 / 2);

	renderer->set_draw_color(0, 0, 0, 0);
	for (unsigned int y = y_start, i = 0; y < y_end; y++, i++)
	{
		for (unsigned int x = x_start, j = 0; x < x_end; x++, j++)
		{
			switch (screen[i][j])
			{
			case 0: renderer->set_draw_color(155, 188, 15, 255); break;
			case 1: renderer->set_draw_color(139, 172, 15, 255); break;
			case 2: renderer->set_draw_color(48, 98, 48, 255); break;
			case 4: renderer->set_draw_color(15, 56, 15, 255); break;
			case 12: renderer->set_draw_color(15, 56, 15, 255); break;
			}
			renderer->draw_px(x, y);
		}
	}
	renderer->present();
	flush();
}

void Display::render_background(__uint8 *memory, __uint8 scx, __uint8 scy, __uint16 tile_map, __uint16 tile_data)
{
	__uint8 tile_id;
	__uint16 data;

	renderer->set_draw_color(0, 0, 0, 0);
	for (unsigned int y = (renderer->height / 2) - (255 / 2), i = 0; y < (renderer->height / 2) + (255 / 2); y++, i++)
	{
		for (unsigned int x = (renderer->width / 2) - (255 / 2), j = 0; x < (renderer->width / 2) + (255 / 2); x += 8, j += 8)
		{
			tile_id = memory[tile_map + ((i / 8) * 32) + (j / 8)];
			data = *(__uint16 *)&memory[tile_data + (tile_id * 16) + (i % 8) * 2];

			for (unsigned int k = 0, mask = 0x8000, lomask = 0x0080; k < 8; k++, mask >>= 1, lomask >>= 1)
			{
				if (((i == scy || i == scy + 144U) && j + k >= scx && j + k <= scx + 160U)) 
					renderer->set_draw_color(255, 0, 0, 255);
				else if ((i >= scy && i <= scy + 144U) && (j + k == scx || j + k == scx + 160U)) 
					renderer->set_draw_color(255, 0, 0, 255);
				else if (((!!(data & mask)) << 1) | !!(data & (lomask)))
					renderer->set_draw_color(0, 0, 0, 255);
				else
					renderer->set_draw_color(255, 255, 255, 255);
				renderer->draw_px(x + k, y);
			}
		}
	}
	renderer->present();
}

void Display::push_px(__uint8 color)
{
	if (screenx == 160) return;

	screen[screeny][screenx] = color;

	screenx = (screenx + 1);
	if (!screenx) screeny = (screeny + 1) % 144;
}

void Display::flush()
{
	screenx = screeny = 0;
}
