#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
//#include "math/math.h"
#include "util.h"
//#include <string.h>
#include <sys/timers.h>
#include "benchmark.h"
#include "time.h"
#include <debug.h>
#include "math/math.h"

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

static inline void check_inputs(fixed24* x, fixed24* y)
{
	if(key_pressed(kb_Right))
		*x += 1;
	if(key_pressed(kb_Left))
		*x -= 1;
	if(key_pressed(kb_Down))
		*y += 1;
	if(key_pressed(kb_Up))
		*y -= 1;
}

int main(void)
{
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up basic color palette
	gfx_SetDefaultPalette(gfx_8bpp);

	gfx_SetTextScale(2, 2);

	fixed24 x = 2, y = 15;
	int timer = 0;

	gfx_SetTextFGColor(224);

	// Draw HUD once at the beginning (TODO: Draw texture instead)
	gfx_SetColor(29);
	gfx_FillRectangle_NoClip(0, 180, 320, 60);
	gfx_PrintStringXY("Put menu here", 0, 180);
	gfx_SwapDraw();
	gfx_FillRectangle_NoClip(0, 180, 320, 60);
	gfx_PrintStringXY("Put menu here", 0, 180);

	time_enable();
	benchmark_enable();

	gfx_SetTextBGColor(0);

	dbg_ClearConsole();

	do {
		key_update();
		check_inputs(&x, &y);

		//benchmark_start();
		// Unroll fixes weird texture glitching. No clue how/why this fixes it
		#pragma unroll(2)
		for(uint24_t i = 2; i < 318; i+=2) {
			uint24_t sine_length = 120-((127+lu_sin(timer+(i*x)))>>3)-y;
			uint24_t line_length = (120-sine_length)<<1;
			gfx_TexturedVertLine(i, line_length, test_texture);
		}

		//register int i asm("pc");

		// GCC does a stupid
		//#pragma GCC diagnostic push
		//#pragma GCC diagnostic ignored "-Wunused-value"

		//dbg_printf("%d\n", fx2uint(256));
		//#pragma GCC diagnostic pop

		//dbg_printf("%d\n", time_get_fps());
		gfx_SetTextXY(0, 0);
		gfx_PrintUInt(time_get_fps(), 2);

		// It took me WAY too long to figure out that you need this if you don't want
		// ALL the graphics to occasionally mess up for no apparent reason
		timer_1_Counter = 0;
		timer++;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	benchmark_disable();
	time_disable();

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
