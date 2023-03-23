
#include "common/stdafx.h"
#include "render.h"

// SDL renderer

Renderer::Renderer(int height, int width)
{
	this->height = height;
	this->width = width;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("GameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	window_renderer = SDL_CreateRenderer(window, -1, 0);
	// Take the struct IMG_Load returns, unlink the image library, and just pass in the raw data
	SDL_SetWindowIcon(window, IMG_Load("bin/debug/icon.png"));
	SDL_GetWindowSize(window, &width, &height);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(window_renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::present()
{
	SDL_RenderPresent(window_renderer);
}

void Renderer::draw_px(int x, int y)
{
	SDL_RenderDrawPoint(window_renderer, x, y);
}

void Renderer::set_draw_color(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(window_renderer, r, g, b, a);
}

void Renderer::set_window_title(char *title)
{
	SDL_SetWindowTitle(window, title);
}

bool Renderer::exit()
{
	return SDL_PollEvent(&event) && event.type == SDL_QUIT;
}

// (todo): write a opengl port

Renderer *init_renderer(const char *renderer, int height, int width)
{
	return new Renderer(height, width);
}
