#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "fixed.h"
#include "math.h"
#include "util.h"

uint24_t texture[16] = {
  dup8(0xE0),
  dup8(0xE1),
  dup8(0xE2),
  dup8(0xE3),
  dup8(0xE4),
  dup8(0xE5),
  dup8(0xE6),
  dup8(0xE7),
  dup8(0xE8),
  dup8(0xE9),
  dup8(0xEA),
  dup8(0xEB),
  dup8(0xEC),
  dup8(0xED),
  dup8(0xEE),
  dup8(0xEF)
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
    //_gfx_TexturedVertLine2_NoClip(x, y, 60, texture);

		for(uint16_t i = 0; i < 320; i+=2)
		{
      uint8_t sine_length = 120-((127+lu_sin(timer+(i*x)))>>3)-y;

      // draw_column_tall((uint8_t*)0xd40000, );
      _gfx_TexturedVertLine2_NoClip(i, sine_length, 200, texture);
			// gfx_VertLine2_NoClip(i, sine_length, (120-sine_length)<<1, dup8(i));
		}

		timer++;

		gfx_SwapDraw();
		// gfx_palette[224] = gfx_palette[timer&255];		
	} while (!key_pressed(kb_2nd));

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
