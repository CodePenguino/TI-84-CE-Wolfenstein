#include <keypadc.h>
#include <tice.h>
#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
#include "util.h"
#include <sys/timers.h>
#include "time.h"
#include <debug.h>
#include "spi.h"
#include <sys/lcd.h>

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

extern uint8_t test_texture[];

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

	// Set up basic color palette
	gfx_SetDefaultPalette(gfx_8bpp);

	fixed24 x = 2, y = 170;
	//uint8_t timer = 0;

	time_enable();
	set_scaled_mode();

	do {
		key_update();
		check_inputs(&x, &y);

		for(uint8_t i = 158; i > 0; --i) {
			//uint24_t line_length = (240-((127+lu_sin(timer+(i*x)))>>3)-y)<<1;
			gfx_TexturedVertLine(i, y, test_texture);
		}

		dbg_printf("%lu\n", time_get_fps());
		timer_1_Counter = 0;

		gfx_SwapDraw();
	} while (!key_pressed(kb_Clear));

	//benchmark_disable();
	time_disable();

	// Reset the SPI to how it was before the program was run
	asm("call $000384");

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
