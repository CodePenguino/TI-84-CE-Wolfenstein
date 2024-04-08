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

// Holds information for the rendered texture
const uint8_t texture[64] = {
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5,
	0xA6,
	0xA7,
	0xA8,
	0xA9,
	0xAA,
	0xAB,
	0xAC,
	0xAD,
	0xAE,
	0xAF,
	0xB0,
	0xB1,
	0xB2,
	0xB3,
	0xB4,
	0xB5,
	0xB6,
	0xB7,
	0xB8,
	0xB9,
	0xBA,
	0xBB,
	0xBC,
	0xBD,
	0xBE,
	0xBF,
	0xE0,
	0xC1,
	0xC2,
	0xC3,
	0xC4,
	0xC5,
	0xC6,
	0xC7,
	0xC8,
	0xC9,
	0xCA,
	0xCB,
	0xCC,
	0xCD,
	0xCE,
	0xCF,
	0xD0,
	0xD1,
	0xD2,
	0xD3,
	0xD4,
	0xD5,
	0xD6,
	0xD7,
	0xD8,
	0xD9,
	0xDA,
	0xDB,
	0xDC,
	0xDD,
	0xDE,
	0xDF,
};

void check_inputs(fixed24* x, fixed24* y)
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

	fixed24 x = 2, y = 180;
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

	bool start_draw_line = 0;

	do
	{
		//gfx_BlitScreen();
		time_update();
		key_update();
		check_inputs(&x, &y);

		benchmark_start();
		#pragma unroll(2)
		for(uint24_t i = (int)start_draw_line*2; i < 320; i+=2) {
			int8_t y_pos = (120-((y)>>1))-30;
			if(y_pos < 0) y_pos = 0;
			gfx_TexturedVertLine_NoClip(i, y_pos, y, texture);
		}
		benchmark_stop();

		//start_draw_line = !start_draw_line;

		gfx_SetTextXY(0, 200);
		dbg_printf("%d\n", time_delta);
		//gfx_PrintUInt(benchmark_get_time(), 8);
		//dbg_printf("%d\n", time_get_fps());

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	benchmark_disable();
	time_disable();

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
