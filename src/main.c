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
	gfx_palette[0] = gfx_RGBTo1555(255, 255, 255);
	gfx_palette[1] = gfx_RGBTo1555(255, 0, 0);
	gfx_palette[2] = gfx_RGBTo1555(0, 0, 255);

	gfx_SetTextScale(2, 2);

	fixed24 x = 0, y = 0;
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

		// gfx_SetColor16(dup8(1));
		//gfx_SetColor(1);
		//gfx_SetPixel(1, 1);
		//gfx_VertLine(20, 20, 20);

		gfx_SetPixel2(x, y, 2);

		//for(uint8_t i = 0; i < 30; i++)
			//gfx_vbuffer16[fx2uint(y)+i][fx2uint(x)] = 0x0001;

		gfx_SetColor(1);
		gfx_FillRectangle((lu_cos(timer)>>1) + 148, (lu_sin(timer)>>1) + 108, 12, 12);

		gfx_SetTextFGColor(1);
		gfx_SetTextXY(0, 0);
		gfx_PrintInt(x, 8);
		gfx_SetTextXY(0, 16);
		gfx_PrintInt(y, 8);
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