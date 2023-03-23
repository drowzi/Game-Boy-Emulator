

#include "common/stdafx.h"
#include "processors/pixel/ppu.h"

void Ppu::stress_test()
{
	// IDLE state
	update(8);
	assert(State == PipelineState::NO_DISPLAY);

	// SAM state
	(*mmu->LCDC) |= 0x01;
	update(4);
	assert(State == PipelineState::SAM);
	update(72);
	assert((*(mmu->STAT) & 0x02) == 0x02);
	assert(State == PipelineState::SAM);
	
	// PT state
	update(4);
	assert(State == PipelineState::PT);
	update(168);
	assert((*(mmu->STAT) & 0x03) == 0x03);
	assert(State == PipelineState::PT);

	// HBLANK state
	update(4);
	assert(State == PipelineState::HBLANK);
	update(200);
	assert(State == PipelineState::HBLANK);
	assert((*(mmu->STAT) & 0x03) == 0x00);

	// Cycle until VBLANK
	update(4);
	for (unsigned int line = 1; line < 144; line++)
	{
		assert(State == PipelineState::SAM);
		update(80);
		update(172);
		if (line < 143) update(204);
	}

	// VBLANK state
	update(4);
	assert(State == PipelineState::VBLANK);
	update(4552);
	assert(State == PipelineState::VBLANK);
	update(4);
	assert(State == PipelineState::SAM);
}