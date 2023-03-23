#include "common/stdafx.h"
#include "processors/pixel/ppu.h"
#include "render.h"

#define LOUD_PPU 0

struct sprite_block
{
	__uint8 x;
	__uint8 y;
	__uint8 pattern;
	__uint8 attributes;
};

Ppu::Ppu(Mmu *mmu, Display *display)
{
	this->mmu = mmu;
	this->memory = mmu->mem;
	this->display = display;
	this->State = PipelineState::NO_DISPLAY;
}

Ppu::~Ppu()
{
}

/* Improvements
 *	- Stop returning cycles and attach it to ppu state 
 */

inline void Ppu::set_vram_access(int mask)
{
	*(mmu->STAT) &= ~(0x03U);
	*(mmu->STAT) |= mask;
}

inline void Ppu::maybe_trigger_lcdi()
{
	// If stat & 0x04, 0x08, 0x10, or 0x20
	if (((*mmu->STAT) >> 2) & 0x0f)
	{
		(*mmu->IF) |= 0x02U;
	}
}

void Ppu::update(unsigned int cycles)
{
	while (cycles)
	{
		switch (State)
		{
			case PipelineState::SAM:		cycles = sam_search(cycles); break;
			case PipelineState::PT:			cycles = pixel_transfer(cycles); break;
			case PipelineState::VBLANK:		cycles = vblank(cycles); break;
			case PipelineState::HBLANK:		cycles = hblank(cycles); break;
			case PipelineState::NO_DISPLAY: cycles = idle(cycles); break;
		}
	}
	maybe_trigger_lcdi();
}

// Probe for display
unsigned int Ppu::idle(unsigned int cycles)
{
	if (!display_on())
		return 0;
	State = PipelineState::SAM;
	return cycles;
}

unsigned int Ppu::hblank(unsigned int cycles)
{
	set_vram_access(0x00U);

	if (!IDLE.status)
	{
		IDLE.cycles_left = 204;
		IDLE.status = 1;
		check_coincidence();
	}

	if (cycles > IDLE.cycles_left)
	{
		cycles -= IDLE.cycles_left;
		IDLE.cycles_left = 0;
	}
	else
	{
		IDLE.cycles_left -= cycles;
		cycles = 0;
	}

	if (!IDLE.cycles_left)
	{
		State = PipelineState::SAM;
		IDLE.status = 0;
	}

	return cycles;
}

unsigned int Ppu::vblank(unsigned int cycles)
{
	set_vram_access(0x01U);

	if (!IDLE.status)
	{
		//display->render();

#if 1
		display->render_background(memory->memory, *mmu->SCX, *mmu->SCY, bg_tile_map_base(), tile_data_base());
#endif
		IDLE.cycles_left = 4560;
		IDLE.status = 1;
		PT.fetch_current_line = 0;
		*(mmu->IF) = set_bit8(*(mmu->IF), 0);
	}

	if (cycles > IDLE.cycles_left)
	{
		cycles -= IDLE.cycles_left;
		SAM.cycles_left = 0;
	}
	else
	{
		IDLE.cycles_left -= cycles;
		cycles = 0;
	}

	if (IDLE.cycles_left)
		*(mmu->LY) = 144 + (4560 / IDLE.cycles_left) - 1;

	// theres a gap between the highest ly tick and 0 for coincidence checking

	if (!IDLE.cycles_left)
	{
		IDLE.status = 0;
		State = PipelineState::SAM;
		*(mmu->LY) = 0;
	}

	check_coincidence();

	return cycles;
}

// 80 cycles (20mc) - 40*2 reads to be done. May need to toggle this if sprites are off.
unsigned int Ppu::sam_search(unsigned int cycles)
{
	set_vram_access(0x02U);

	__uint8 LY = (*mmu->LY);

	if (!SAM.status)
	{
		SAM.cycles_left = 80;
		SAM.next_address = 0xfe00;
		SAM.status = 1;
		visible_sprite_count = 0;
	}

	// 0xfe00 - 0xfea0
	__uint16 end_address;
	if (cycles > SAM.cycles_left)
	{
		end_address = SAM.next_address + (4 * SAM.cycles_left);
		cycles -= SAM.cycles_left;
		SAM.cycles_left = 0;
	}
	else
	{
		end_address = SAM.next_address + (4 * cycles);
		SAM.cycles_left -= cycles;
		cycles = 0;
	}

	for (__uint16 i = SAM.next_address; i < end_address; i+=4)
	{
		sprite_block *sprite = (sprite_block *)(&memory->memory[i]);
		if (sprite->x)
		{
			if ((LY + 16 >= sprite->y) && (LY + 16 < sprite->y + sp_h))
			{
				visible_sprites[visible_sprite_count++] = i;
				std::cout << "1" << std::endl;
			}
		}
	}

	SAM.next_address = end_address;

	if (!SAM.cycles_left)
	{
		State = PipelineState::PT;
		SAM.status = 0;
		//std::cout << "Found " << (int)visible_sprite_count << " sprites\n";
	}

	return cycles;
}

inline bool Ppu::window_visible()
{
	return (*mmu->WX / 8) >= PT.fetch_current_tile && (*mmu->WY / 8) > 0;
}

inline __uint16 Ppu::get_map_base()
{
	return window_on() && window_visible() ? wn_tile_map_base() : bg_tile_map_base();
}

inline __uint16 Ppu::get_next_tile_addr()
{
	return get_map_base() + ((((*mmu->SCY >> 3) + (PT.fetch_current_line / 8)) % 32) * 32) + (((*mmu->SCX >> 3) + PT.fetch_current_tile) % 32);
}

inline void Ppu::reset_pt()
{
	PT.status = 1;
	PT.cycles_left = 172;
	PT.fetch_current_tile = 0;
	PT.fetch_cycles_left = 6;
	PT.pixel_fifo_len = 0;
	PT.fetch_address = get_next_tile_addr();
	PT.fetch_tile_id = 0;
	PT.fetch_buffer = 0x00;

	display->screenx = 0;
	display->screeny = PT.fetch_current_line;
}

void Ppu::set_sync()
{
	if (PT.fetch_current_line >= 144)
	{
		State = PipelineState::VBLANK;
	}
	else
	{
		State = PipelineState::HBLANK;
	}

	PT.status = 0;
}

// 172 cycles (43+mc)
unsigned int Ppu::pixel_transfer(unsigned int cycles)
{
	set_vram_access(0x03U);

	if (!PT.status)
		reset_pt();

	cycles = pixel_transfer_step(cycles);

	if (!PT.cycles_left)
		set_sync();

	return cycles;
}

// Simulates the pixel pipeline fetching in steps
void Ppu::pixel_transfer_fetch()
{
	switch (PT.fetch_cycles_left)
	{
		case 6: 
		{
			if (tile_data_base() == 0x8800) { // Signed.
				// Used by dr. mario
				int id = memory->memory[PT.fetch_address];
				//PT.fetch_tile_id = (int)memory->memory[PT.fetch_address];
				std::cout << id << std::endl;
				PT.fetch_tile_id = id - 127;
			}
			else { // Unsigned.
				// Used by intro
				PT.fetch_tile_id = memory->memory[PT.fetch_address];
			}
			break;
		}
		// This needs to use signed values for the 8800 base
		case 4:	
		{
			if (tile_data_base() == 0x8800) { // Signed.
				PT.fetch_buffer = memory->memory[0x9000 + (PT.fetch_tile_id * 16) + (PT.fetch_current_line % 8) * 2];
				PT.fetch_buffer &= memory->memory[0x9000 + 1 + (PT.fetch_tile_id * 16) + (PT.fetch_current_line % 8) * 2];
			}
			else {
				PT.fetch_buffer = memory->memory[tile_data_base() + (PT.fetch_tile_id * 16) + (PT.fetch_current_line % 8) * 2];
				PT.fetch_buffer &= memory->memory[tile_data_base() + 1 + (PT.fetch_tile_id * 16) + (PT.fetch_current_line % 8) * 2];
			}
			break;
		}
		// This is meant to be for fetching the second half of data
		//case 2: PT.fetch_buffer &= memory->memory[tile_data_base() + 1 + (PT.fetch_tile_id * 16) + (PT.fetch_current_line % 8) * 2]; break;
	}
	PT.fetch_cycles_left--;
}

void Ppu::pixel_transfer_fifo_dequeue()
{
	__uint8 px;

	if (PT.pixel_fifo_len > 8 && display->screenx < 160)
	{
		px = color(*mmu->BGP, PT.pixel_fifo[0]);
		display->push_px(px);
		for (unsigned int i = 1; i < PT.pixel_fifo_len; i++)
		{
			PT.pixel_fifo[i - 1] = PT.pixel_fifo[i];
		}
		PT.pixel_fifo_len--;
	}
}

void Ppu::pixel_transfer_fifo_enqueue()
{
	__uint8 data;

	if (PT.pixel_fifo_len <= 8)
	{
		//std::cout << "Pushed tile to fifo" << std::endl;
		// Extraction
		for (unsigned int i = 0, hi = 0x8000, lo = 0x0080; i < 8; i++, hi >>= 1, lo >>= 1)
		{
			data = (static_cast<bool>((PT.fetch_buffer & hi))) << 1 | static_cast<bool>(PT.fetch_buffer & lo);
			PT.pixel_fifo[i + PT.pixel_fifo_len] = data;
		}
		PT.pixel_fifo_len += 8;
		//if (there exists a sprite whose x is within this tile)
		PT.fetch_cycles_left = 6;
		PT.fetch_current_tile++;
		// H or V Blank
		if (PT.fetch_current_tile > 20)
		{
			PT.fetch_current_tile = 0;
			PT.fetch_current_line++;
			PT.cycles_left = 0;
			(*mmu->LY)++;
			return;
			//return cycles + usable_cycles - 1;
		}

		PT.fetch_address = get_next_tile_addr();
	}
}

// 1 cycle per step, however, this isn't a step, just encapsulation for the sake of organization.
unsigned int Ppu::pixel_transfer_step(unsigned int cycles)
{
	unsigned int usable_cycles;

	if (cycles > PT.cycles_left)
	{
		usable_cycles = cycles - PT.cycles_left;
		cycles -= PT.cycles_left;
		PT.cycles_left = 0;
	}
	else
	{
		usable_cycles = cycles;
		PT.cycles_left -= cycles;
		cycles = 0;
	}

	while (usable_cycles--)
	{
		// This first runs after 12 cycles, two fetch dumps
		pixel_transfer_fifo_dequeue();
		pixel_transfer_fifo_dequeue();

		if (!PT.fetch_cycles_left)
		{
			// (issue): this enqueue function ignores hblank trigger premature exit
			if (PT.pixel_fifo_len <= 8)
			{
				//std::cout << "Pushed tile to fifo" << std::endl;
				// Extraction
				for (unsigned int i = 0, hi = 0x8000, lo = 0x0080; i < 8; i++, hi >>= 1, lo >>= 1)
				{
					PT.pixel_fifo[i + PT.pixel_fifo_len] = (!!((PT.fetch_buffer & hi))) << 1 | !!(PT.fetch_buffer & lo);
				}
				PT.pixel_fifo_len += 8;
				//if (there exists a sprite whose x is within this tile)
				PT.fetch_cycles_left = 6;
				PT.fetch_current_tile++;
				// H or V Blank

				// (note): changing this to 21 fixes the missing tile issue, figure out why
				if (PT.fetch_current_tile > 20)
				{
					PT.fetch_current_tile = 0;
					PT.fetch_current_line++;
					PT.cycles_left = 0;
					(*mmu->LY)++;
					//std::cout << (int)(*mmu->LYC) << std::endl;
					return cycles + usable_cycles - 1;
				}
				PT.fetch_address = get_next_tile_addr();
			}
		}
		else
		{
			pixel_transfer_fetch();
		}
	}
	return cycles;
}

inline void Ppu::check_coincidence()
{
	if ((*mmu->LY) == (*mmu->LYC))
	{
		(*mmu->STAT) = set_bit8((*mmu->STAT), 3);
		if (get_bit8((*mmu->STAT), 7))
		{
			(*mmu->IF) = set_bit8((*mmu->IF), 2);
		}
	}
	else
	{
		(*mmu->STAT) = res_bit8((*mmu->STAT), 3);
		(*mmu->IF) = res_bit8((*mmu->IF), 2);
	}
}

// Usage: cvt_palette_id([bgp, obp0, obp1], id);
__uint8 Ppu::color(__uint8 palette_data, __uint8 palette_id)
{
	switch (palette_id)
	{
		case 0x00: return palette_data & 0x03;
		case 0x01: return palette_data & 0x0c;
		case 0x02: return palette_data & 0x30;
		case 0x04: return palette_data & 0xc0;
		default:
		{
			std::cout << "Invalid palette id: " << palette_id << std::endl;
			return 0;
		}
	}
}

void Ppu::view_bg()
{
	system("cls");
	int scrollpos = ((*mmu->SCY >> 3) * 32) + ((*mmu->SCX) >> 3);
	for (unsigned int i = 0; i < 32; i++)
	{
		for (unsigned int j = 0; j < 32; j++)
		{
			int offset = (32 * i) + j;
			if (scrollpos == offset) std::cout << 'S' << " ";
			else std::cout << (int)memory->memory[bg_tile_map_base() + offset] << " ";
		}
		std::cout << std::endl;
	}
}

void test_ppu()
{
	//Ppu ppu(new Mmu(new Memory()));
	//ppu.stress_test();
}
