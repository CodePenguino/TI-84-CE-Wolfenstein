#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "fixed.h"
#include "math.h"
#include "util.h"
#include <string.h>
#include <sys/timers.h>

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
uint8_t texture[64] = {
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
	0xE0,
	0xE1,
	0xE2,
	0xE3,
	0xE4,
	0xE5,
	0xE6,
	0xE7,
	0xE8,
	0xE9,
	0xEA,
	0xEB,
	0xEC,
	0xED,
	0xEE,
	0xEF,
	0xE0,
	0xF1,
	0xF2,
	0xF3,
	0xF4,
	0xF5,
	0xF6,
	0xF7,
	0xF8,
	0xF9,
	0xFA,
	0xFB,
	0xFC,
	0xFD,
	0xFE,
	0xFF,
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

	do
	{
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

    uint8_t length = y;
    #pragma unroll(2) 
		for(uint24_t i = 0; i < 320; i+=2) {
			length--;
			length = clamp(length, 1, 180);
			uint8_t y_pos = (120-(length>>1))-30;
      gfx_TexturedVertLine_NoClip(i, y_pos, length, texture+timer);
		}

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
		if(timer > 64) { timer = 0; }

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
