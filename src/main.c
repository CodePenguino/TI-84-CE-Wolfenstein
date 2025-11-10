#include <keypadc.h>
#include <stdint.h>
#include <string.h>
#include <tice.h>
#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
#include "gfx/renderer.h"
#include <sys/timers.h>
#include "time.h"
#include "math/math.h"
#include <debug.h>
#include "gfx/spi.h"
#include "gfx/gfx.h"

// TODO: Have this map do literally anything
const uint8_t map[64] = {
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

static inline void check_inputs(fixed24* x, fixed24* y) {
	if(key_pressed(kb_Right))
		*x += 1;
	if(key_pressed(kb_Left))
		*x -= 1;
	if(key_pressed(kb_Down))
		*y += 1;
	if(key_pressed(kb_Up))
		*y -= 1;
}

int main(void) {
	// Calls _boot_InitializeHardware
	asm("call $000384");
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up wolfenstein color palette
	//gfx_SetDefaultPalette(gfx_8bpp);
	gfx_SetPalette(global_palette, sizeof(global_palette), 0);

	fixed24 x = 2, y = 170;
	uint8_t timer = 0;

	time_enable();
	set_scaled_mode();

	// Draw blue border
	gfx_FillScreen(6);
	gfx_SwapDraw();
	gfx_FillScreen(6);

	uint24_t line_length = 0;

	// Aligns texture data so last 8 bits are all 0 (fixes alignment bug)
	uint8_t* full_data = malloc(4096 + 256);
	uintptr_t offset = 256 - ((uintptr_t)full_data % 256);
	memcpy((void*)(full_data + offset), door_texture_data, sizeof(door_texture_data));
	uint8_t* data = (uint8_t*)(full_data+offset);

	do {
		key_update();
		check_inputs(&x, &y);

		gfx_Wait();
		for(uint8_t i = 0; i <= 159; i++) {
			line_length = (240-((127+lu_sin(timer+(i*x)))>>3)-y)<<1;
			gfx_TexturedVertLine(i, line_length, data + (i<<6));
		}

		//dbg_printf("%lu\n", time_get_fps());
		timer_1_Counter = 0;
		timer++;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	//benchmark_disable();
	time_disable();

	free(data);

	// Reset the SPI to how it was before the program was run
	asm("call $000384");

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
