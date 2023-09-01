#include <ti/screen.h>
#include <graphx.h>
#include "color.h"
#include "input.h"
#include <string.h>
#include "util.h"

/*void FillScreen(uint16_t color)
{
	memset(lcd_Ram, color, (320*240)<<1);
}*/

/* Main function, called first */
int main(void)
{
	/* Clear the homescreen */
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	gfx_palette[0] = gfx_RGBTo1555(255, 255, 255);
	gfx_palette[1] = gfx_RGBTo1555(255, 0, 0);

	uint8_t x = 0, y = 0;

	// TODO: Change key update to be per frame, not whatever it is right now...
	// TODO: Add page flipping
	do
	{
		key_update();
		gfx_ZeroScreen();
		
		if(key_pressed(sk_Right))
			x += 4;
		if(key_pressed(sk_Left))
			x -= 4;
		if(key_pressed(sk_Up))
			y += 4;
		if(key_pressed(sk_Down))
			y -= 4;

		gfx_SetColor(1);
		gfx_FillRectangle(0+x, 0+y, 20, 20);
	} while (!key_pressed(sk_2nd));
	gfx_End();

	/* Return 0 for success */
	return 0;
}