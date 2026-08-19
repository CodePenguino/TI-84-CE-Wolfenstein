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

	set_scaled_mode();

	// Draw blue border
	memset(gfx_vram, 1, 160*240);
	memset(gfx_vram+(320*240), 1, 160*240);

	int8_t dirX, dirY;

	uint8_t rotation = 0;

	do {
		key_update();

		if(key_pressed(kb_Up)) {
			posX += dirX/2;//fxmul8(dirX, 128);
			posY += dirY/2;//fxmul8(dirY, 128);
		}
		if(key_pressed(kb_Down)) {
			posX -= dirX/2;//fxmul8(dirX, 128);
			posY -= dirY/2;//fxmul8(dirY, 128);
		}
		if(key_pressed(kb_Right)) {
			rotation -= 4;
		}
		if(key_pressed(kb_Left)) {
			rotation += 4;
		}

		dirX = lu_cosneg(rotation);
		dirY = lu_sinneg(rotation);

		for(RENDER_x = 0; RENDER_x < 160; RENDER_x++) {
			// calculate ray position and direction
			camera_x = camera_x_lut[RENDER_x]; //x-coordinate in camera space
			F_rayDirX = ((dirX*2) + fxmul24(dirY*2, camera_x));
			F_rayDirY = ((dirY*2) - fxmul24(dirX*2, camera_x));

			F_deltaDistX = div_lut[abs24(F_rayDirX)];
			F_deltaDistY = div_lut[abs24(F_rayDirY)];

            RENDER_length = 46080/raycast();

			gfx_TexturedVertLine();
			texture_pointer = (uint8_t*)door_texture_data;
		}

		//gfx_SetPixel2_NoClip(0, 180, time_get_fps());

		//texture_pointer = (uint8_t*)door_texture_data;
		//for(RENDER_x = 0; RENDER_x < 160; RENDER_x++) {
		//	RENDER_length = rotation;
		//	gfx_TexturedVertLine();
		//	texture_pointer += 64;
		//}

		sprintf(((char*)0xFB0000), "%hu\n", time_get_fps());
		timer_1_Counter = 0;

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
