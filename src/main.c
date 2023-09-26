#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "fixed.h"
#include "math.h"
#include "util.h"

int main(void)
{
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up basic color palette
	//gfx_palette[0] = gfx_RGBTo1555(255, 255, 255);
	//gfx_palette[1] = gfx_RGBTo1555(255, 0, 0);
	//gfx_palette[2] = gfx_RGBTo1555(0, 0, 255);
	gfx_SetDefaultPalette(gfx_8bpp);

	gfx_SetTextScale(2, 2);

	fixed24 x = 2, y = 15;
	uint24_t timer = 0;

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

		gfx_SetColor(timer>>1);
		//gfx_SetPixel2_NoClip(x, y, dup8(timer>>1));
		//gfx_VertLine2_NoClip(x, y, 15, dup8(timer>>1));

		for(uint16_t i = 0; i < 320; i+=2)
		{
			uint8_t sine_length = 120-((127+lu_sin(timer+(i*x)))>>3)-y;
			__gfx_VertLine2_NoClip(i, sine_length, (120-sine_length)<<1, dup8(i-(timer>>1)));
			//gfx_Rectangle_NoClip(i, sine_length, 2, (120-sine_length)<<1);
			// gfx_VertLine_NoClip(i, sine_length, (120-sine_length)<<1);
			// gfx_VertLine(i, sine_length, (120-sine_length)<<1);
		}

		//gfx_SetColor(1);
		//gfx_FillRectangle((lu_cos(timer)>>1) + 148, (lu_sin(timer)>>1) + 108, 12, 12);

		//gfx_SetTextFGColor(2);
		//gfx_SetTextXY(0, 0);
		//gfx_PrintInt(timer >> 8, 8);
		timer++;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}