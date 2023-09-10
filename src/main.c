#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "fixed.h"
#include "math.h"

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

	//fixed24 x = 0, y = 0;
	uint24_t timer = 0;

	do
	{
		key_update();
		gfx_ZeroScreen();

		/*if(key_pressed(kb_Right))
			x += FIX_ONE;
		if(key_pressed(kb_Left))
			x -= FIX_ONE;
		if(key_pressed(kb_Down))
			y += FIX_ONE;
		if(key_pressed(kb_Up))
			y -= FIX_ONE;*/

		gfx_SetColor(1);
		gfx_FillRectangle((lu_cos(timer)>>1) + 148, (lu_sin(timer)>>1) + 108, 12, 12);

		gfx_SetTextFGColor(2);
		gfx_SetTextXY(0, 8);
		gfx_PrintInt(timer, 8);
		timer++;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}