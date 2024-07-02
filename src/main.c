#include <keypadc.h>
#include <tice.h>
#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
//#include "math/math.h"
#include "util.h"
//#include <string.h>
#include <sys/timers.h>
#include "time.h"
#include <debug.h>
#include "math/math.h"
#include "spi.h"

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
	if(key_tapped(kb_2nd))
		set_scaled_mode();
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
	gfx_SetTextScale(1, 1);

	fixed24 x = 2, y = 170;
	int timer = 0;

	gfx_SetTextFGColor(224);
	// Draw HUD once at the beginning (TODO: Draw texture instead)
	//gfx_SetColor(29);
	//gfx_FillRectangle_NoClip(0, 180, 320, 60);
	//gfx_PrintStringXY("Put menu here", 0, 180);
	//gfx_SwapDraw();
	//gfx_FillRectangle_NoClip(0, 180, 320, 60);
	//gfx_PrintStringXY("Put menu here", 0, 180);

	time_enable();
	//benchmark_enable();
	gfx_SetTextScale(1,2);

	gfx_SetTextBGColor(0);

	//dbg_ClearConsole();

	do {
		key_update();
		check_inputs(&x, &y);
		//gfx_ZeroScreen();

		for(uint24_t i = 158; i > 0; i--) {
			uint24_t line_length = (240-((127+lu_sin(timer+(i*x)))>>3)-y)<<1;
			gfx_TexturedVertLine(i, line_length, test_texture);
		}

		//dbg_printf("%d\n", y);
		//gfx_SetTextXY(0, 0);
		//gfx_PrintUInt(time_get_fps(), 2);
		//dbg_printf("%d\n", time_get_fps());
		timer_1_Counter = 0;
		timer++;

		gfx_SwapDraw();
	} while (!key_pressed(kb_Clear));

	//benchmark_disable();
	time_disable();

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
