#pragma once

#include "common/stdafx.h"
#include "SDL.h"
#include "SDL_image.h"

class Renderer
{
public:
	Renderer(int height, int width);
	~Renderer();

	void Renderer::present();
	void Renderer::draw_px(int x, int y);
	void Renderer::set_draw_color(int r, int g, int b, int a);
	void Renderer::set_window_title(char *title);
	bool Renderer::exit();

	SDL_Window *window;
	SDL_Renderer *window_renderer;
	SDL_Event event;
	int height, width;
};

Renderer *init_renderer(const char *renderer, int height, int width);
void render_dev_view(__uint8 *memory, __uint8 scx, __uint8 scy, __uint16 tile_map, __uint16 tile_data);