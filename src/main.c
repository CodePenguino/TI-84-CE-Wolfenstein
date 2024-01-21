#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
#include "math/math.h"
#include "util.h"
#include <string.h>
#include <sys/timers.h>
#include "debug.h"
#include "time.h"

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
	0xC0,
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

int main(void)
{
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up basic color palette
	gfx_SetDefaultPalette(gfx_8bpp);

	gfx_SetTextScale(2, 2);

	fixed24 x = 2, y = 170;
	uint24_t timer = 0;

	gfx_SetTextFGColor(224);
	// Draw HUD once at the beginning (TODO: Draw texture instead)
	gfx_SetColor(29);
	gfx_FillRectangle_NoClip(0, 180, 320, 60);
	gfx_PrintStringXY("Put menu here", 0, 180);
	gfx_SwapDraw();
	gfx_FillRectangle_NoClip(0, 180, 320, 60);
	gfx_PrintStringXY("Put menu here", 0, 180);

	time_enable();
	debug_enable();

	do
	{
		time_update();
		key_update();
		memset(gfx_vbuffer, 4, 28800);
		memset(gfx_vbuffer+90, 6, 28800);

		if(key_pressed(kb_Right))
			x += 1;
		if(key_pressed(kb_Left))
			x -= 1;
		if(key_pressed(kb_Down))
			y += 1;
		if(key_pressed(kb_Up))
			y -= 1;

		//benchmark_start();
		#pragma unroll(2)
		for(uint24_t i = 0; i < 320; i+=2) {
			uint8_t y_pos = (120-(y>>1))-30;
			gfx_TexturedVertLine_NoClip(i, y_pos, y,
					texture/*+(lu_sin(timer*2)/4)*/);
		}
		//benchmark_stop();
		//timer = benchmark_stop_time - benchmark_start_time;

		gfx_SetTextXY(0, 200);
		gfx_SetTextBGColor(0);
		//gfx_PrintUInt(time_get_fps(), 8);


		//gfx_SetTextXY(0,180);
		//gfx_PrintInt(fx2uint(512), 8);
		//#pragma unroll(2)
		/*for(uint16_t i = 0; i < 320; i+=2)
		{
			uint8_t sine_length = 120-((127+lu_sin(timer+(i*x)))>>3)-y;
			uint8_t line_length = (120-sine_length)<<1;

			gfx_TexturedVertLine_NoClip(i, sine_length-30, line_length, texture);
		}*/

		timer++;
		//timer %= 64;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	time_disable();

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
