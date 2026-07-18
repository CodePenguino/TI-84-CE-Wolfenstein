#include <keypadc.h>
#include <stdint.h>
#include <tice.h>
#include <ti/screen.h>
#include <graphx.h>
#include "input.h"
#include "math/fixed.h"
#include "gfx/renderer.h"
#include <sys/timers.h>
#include "time.h"
#include "math/math.h"
#include <debug.h>
#include "gfx/spi.h"
#include "gfx/texture/gfx.h"
#include "math/lut.h"
#include "ray/map.h"
#include "ray/ray.h"
#include <string.h>

int main(void) {
	// Calls _boot_InitializeHardware
	asm("call $000384");
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up wolfenstein color palette
	gfx_SetPalette(global_palette, sizeof(global_palette), 0);

	time_enable();
	//texture_init();

	// Draw blue border
	memset(gfx_vbuffer, 1, 160*240);
	gfx_SwapDraw();
	memset(gfx_vbuffer, 1, 160*240);
	gfx_SwapDraw();

	set_scaled_mode();

	int8_t dirX = -127, dirY = 0;

	uint8_t rotation = 0;

	do {
		key_update();

		if(key_pressed(kb_Up)) {
			posX += fxmul8(dirX, 128);
			posY += fxmul8(dirY, 128);
		}
		if(key_pressed(kb_Down)) {
			posX -= fxmul8(dirX, 128);
			posY -= fxmul8(dirY, 128);
		}
		if(key_pressed(kb_Right)) {
			rotation -= 4;
		}
		if(key_pressed(kb_Left)) {
			rotation += 4;
		}

		dirX = lu_cosneg(rotation);
		dirY = lu_sinneg(rotation);

		//gfx_Wait();
		for(uint8_t x = 0; x < 160; x++) {
			//calculate ray position and direction
			fixed24 cameraX = camera_x_lut[x]; //x-coordinate in camera space
			F_rayDirX = ((dirX<<1) + fxmul24(dirY<<1, cameraX));
			F_rayDirY = ((dirY<<1) - fxmul24(dirX<<1, cameraX));

			F_deltaDistX = div_lut[abs24(F_rayDirX)];
			F_deltaDistY = div_lut[abs24(F_rayDirY)];

			uint24_t asm_perpWallDist = raycast(F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY);
			uint24_t wall_height = 46080/asm_perpWallDist;

			gfx_TexturedVertLine(x, wall_height, texture_pointer);
			texture_pointer = (uint8_t*)door_texture_data;
		}

		gfx_SetPixel2_NoClip(0, 181, time_get_fps());

		//gfx_palette[1] = time_get_fps() << 11;
		timer_1_Counter = 0;

		//#ifdef DEBUG
		//dbg_printf("%lu\n", time_get_fps());
		//timer_1_Counter = 0;
		//#endif

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	time_disable();
	//texture_disable();

	// Reset the SPI to how it was before the program was run
	asm("call $000384");

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
