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
#include "gfx/texture.h"
#include "math/lut.h"
#include "ray/map.h"
#include "ray/ray.h"

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
	texture_init();
	set_scaled_mode();

	// Draw blue border
	gfx_FillScreen(6);
	gfx_SwapDraw();
	gfx_FillScreen(6);

	fixed24 posX = int2fx(22), posY = int2fx(12);
	fixed24 dirX = -FIX_ONE, dirY = 0;

	uint8_t rotation = 0;

	do {
		key_update();

		if(key_pressed(kb_Up)) {
			posX += fxmul(dirX, 64);
			posY += fxmul(dirY, 64);
		}
		if(key_pressed(kb_Down)) {
			posX -= fxmul(dirX, 64);
			posY -= fxmul(dirY, 64);
		}
		if(key_pressed(kb_Right)) {
			rotation -= 4;
		}
		if(key_pressed(kb_Left)) {
			rotation += 4;
		}

		dirX = -lu_cos(rotation)<<1;
		dirY = -lu_sin(rotation)<<1;

		//gfx_Wait();
		for(uint8_t x = 0; x < 160; x++) {
			//calculate ray position and direction
			fixed24 cameraX = camera_x_lut[x]; //x-coordinate in camera space
			fixed24 F_rayDirX = dirX + fxmul24(dirY, cameraX);
			fixed24 F_rayDirY = dirY - fxmul24(dirX, cameraX);

			//which box of the map we're in
			uint8_t mapX = posX >> 8;
			uint8_t mapY = posY >> 8;

			uint24_t F_sideDistX;
			uint24_t F_sideDistY;

			uint24_t F_deltaDistX = div_lut[abs(F_rayDirX)];
			uint24_t F_deltaDistY = div_lut[abs(F_rayDirY)];

			int8_t stepX;
			int8_t stepY;

			//calculate step and initial sideDist
			if(F_rayDirX < 0) {
				F_sideDistX = fxmul8(posX, F_deltaDistX);
				stepX = -1;
			}
			else {
				F_sideDistX = fxmul8((-posX), F_deltaDistX);
				stepX = 1;
			}
			if(F_rayDirY < 0) {
				F_sideDistY = fxmul8(posY, F_deltaDistY);
				stepY = -1;
			}
			else {
				F_sideDistY = fxmul8((-posY), F_deltaDistY);
				stepY = 1;
			}

			bool side; //was a NS or a EW wall hit?
			//perform DDA
			#pragma unroll(24)
			for(int i = 0; i < 24; i++) {
				//jump to next map square, either in x-direction, or in y-direction
				if(F_sideDistX < F_sideDistY) {
					F_sideDistX += F_deltaDistX;
					mapX += stepX;
					side = false;
				}
				else {
					F_sideDistY += F_deltaDistY;
					mapY += stepY;
					side = true;
				}
				//Check if ray has hit a wall
				if(worldMap[mapX][mapY] != 0) {
					break;
				}
			}

			uint24_t F_perpWallDist;

			if(!side) {
				F_perpWallDist = F_sideDistX - F_deltaDistX;
			}
			else {
				F_perpWallDist = F_sideDistY - F_deltaDistY;
			}

			uint24_t line_height = int2fx(180) / F_perpWallDist;

			uint8_t wallX;
			uint8_t texX;
			if(!side) {
				wallX = posY + fxmul24(F_perpWallDist, F_rayDirY);
				texX = wallX>>2;
				if(F_rayDirX > 0) {
					texX = 64 - texX - 1;
				}
			} else {
				wallX = posX + fxmul24(F_perpWallDist, F_rayDirX);
				texX = wallX>>2;
				if(F_rayDirY < 0) {
					texX = 64 - texX - 1;
				}
			}

			//dbg_Debugger();
			//dbg_printf("%d, ", fxmul8(-posX, F_deltaDistX));
			//dbg_printf("%d\n", raycast(x, F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY));
			//dbg_Debugger();

			gfx_TexturedVertLine(x, line_height, texture_data + (texX*64));
		}

		dbg_printf("%lu\n", time_get_fps());
		timer_1_Counter = 0;

		gfx_SwapDraw();
	} while (!key_pressed(kb_2nd));

	time_disable();
	texture_disable();

	// Reset the SPI to how it was before the program was run
	asm("call $000384");

	// Clear memory and gfx api (otherwise you get corrupted graphics)
	gfx_End();
	return 0;
}
