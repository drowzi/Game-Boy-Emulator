#pragma once
#include "common/stdafx.h"
#include "memory/mmu.h"
#include "display/display.h"

enum class PipelineState { HBLANK, VBLANK, NO_DISPLAY, SAM, PT };

struct PPU_state
{
	int status;
	unsigned int cycles_left;

};

struct SAM_state
{
	int status;
	unsigned int cycles_left;
	__uint16 next_address;
};

struct PT_state
{
	int status;
	unsigned int cycles_left;

	// Tile taken from background tile map
	int fetch_tile_id;

	// Current fetch step
	unsigned int fetch_cycles_left;

	// Address of tile in VRAM
	__uint16 fetch_address;

	// LCD x and y
	int fetch_current_line;
	int fetch_current_tile;

	// Single layer of a tile
	__uint16 fetch_buffer;

	__uint8 pixel_fifo[16];
	__uint8 pixel_fifo_len;
};

struct IDLE_state
{
	int status;
	unsigned int cycles_left;
	unsigned int line;
};

struct DMA_state
{
	int status;
	unsigned int cycles_left;
	__uint16 sam_buffer;
	__uint8 offset;
};

class Ppu
{
public:
	Ppu(Mmu *mmu, Display *display);
	~Ppu();
	void Ppu::update(unsigned int cycles);
	void Ppu::stress_test();

	Mmu *mmu;
	Display *display;

	Memory *memory;
private:
	inline bool Ppu::window_visible();
	inline __uint16 Ppu::get_map_base();
	inline __uint16 Ppu::get_next_tile_addr();
	inline void Ppu::maybe_trigger_lcdi();
	inline void Ppu::check_coincidence();
	void Ppu::view_bg();
	inline void Ppu::set_vram_access(int mask);
	inline void Ppu::reset_pt();
	void Ppu::set_sync();
	__uint8 Ppu::color(__uint8 palette_data, __uint8 palette_id);
	unsigned int Ppu::pixel_transfer_step(unsigned int cycles);
	void Ppu::pixel_transfer_fifo_dequeue();
	void Ppu::pixel_transfer_fifo_enqueue();
	void Ppu::pixel_transfer_fetch();

	unsigned int Ppu::sam_search(unsigned int cycles);
	unsigned int Ppu::pixel_transfer(unsigned int cycles);
	unsigned int Ppu::hblank(unsigned int cycles);
	unsigned int Ppu::vblank(unsigned int cycles);
	unsigned int Ppu::idle(unsigned int cycles);

	PipelineState State;
	SAM_state SAM;
	PT_state PT;
	IDLE_state IDLE;

	// most of these may be associated with a state
	__uint8 sp_h = 8, sp_w = 8;
	__uint8 curr_x = 0, curr_y = 0;
	__uint16 visible_sprites[10];
	__uint8 visible_sprite_count = 0;
};

void test_ppu();

#define display_on() ((*mmu->LCDC) & 0x01)
#define sprites_on() ((*mmu->LCDC) & 0x02)
#define sprites_sz() (((*mmu->LCDC) & 0x04) ? 8 : 16)
#define bg_tile_map_base() (((*mmu->LCDC) & 0x08) ? 0x9c00 : 0x9800)
#define tile_data_base() (((*mmu->LCDC) & 0x10) ? 0x8000 : 0x8800)
#define window_on() ((*mmu->LCDC) & 0x20)
#define wn_tile_map_base() (((*mmu->LCDC) & 0x40) ? 0x9c00 : 0x9800)
#define lcd_on() ((*mmu->LCDC) & 0x80)

