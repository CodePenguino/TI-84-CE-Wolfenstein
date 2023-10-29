#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "fixed.h"
#include "math.h"
#include "util.h"

// Holds information for the rendered texture
uint8_t texture[64] = {
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
	0xC0,
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

	fixed24 x = 2, y = 15;
	uint24_t timer = 0;

	gfx_SetTextFGColor(31);

	do
	{
		key_update();
		gfx_ZeroScreen();

		if(key_pressed(kb_Right))
			x += 1;
		if(key_pressed(kb_Left))
			x -= 1;
		if(key_pressed(kb_Down))
			y += 1;
		if(key_pressed(kb_Up))
			y -= 1;

		/*for(uint24_t i = 0; i < 320; i+=2) {
			gfx_TexturedVertLine_NoClip(i, 120-(y>>1), y, texture, delta_lut[y]);
		}*/

		for(uint16_t i = 0; i < 320; i+=2)
		{
			uint8_t sine_length = 120-((127+lu_sin(timer+(i*x)))>>3)-y;
			uint8_t line_length = (120-sine_length)<<1;

			gfx_TexturedVertLine_NoClip(i, sine_length-30, 
					line_length, texture);
		}

		timer++;

		gfx_SetTextXY(0,0);
		//gfx_PrintUInt(timer, 8);

		gfx_SwapDraw();
		// gfx_palette[224] = gfx_palette[timer&255];		
	} while (!key_pressed(kb_2nd));

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
