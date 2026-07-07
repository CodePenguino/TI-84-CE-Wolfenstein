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

	//fixed24 posX = int2fx(22), posY = int2fx(12);
	int8_t dirX = -127, dirY = 0;

	uint8_t rotation = 0;
	//disable_interlacing();

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

			//which box of the map we're in
			//uint8_t mapX = posX >> 8;
			//uint8_t mapY = posY >> 8;

			//uint24_t F_sideDistX;
			//uint24_t F_sideDistY;

			F_deltaDistX = div_lut[abs24(F_rayDirX)];
			F_deltaDistY = div_lut[abs24(F_rayDirY)];

			/*int8_t stepX;
			int8_t stepY;

			//calculate step and initial sideDist
			if(F_rayDirX < 0) {
				F_sideDistX = fxmul8abs(posX, F_deltaDistX);
				stepX = -1;
			}
			else {
				F_sideDistX = fxmul8abs((-posX)-1, F_deltaDistX);
				stepX = 1;
			}
			if(F_rayDirY < 0) {
				F_sideDistY = fxmul8abs(posY, F_deltaDistY);
				stepY = -1;
			}
			else {
				F_sideDistY = fxmul8abs((-posY)-1, F_deltaDistY);
				stepY = 1;
			}

			bool side = false; //was a NS or a EW wall hit?
			//perform DDA
			//#pragma unroll
			//for(uint8_t i = 32; i > 0; i--) {
			while(1) {
				//jump to next map square, either in x-direction, or in y-direction
				if(F_sideDistX <= F_sideDistY) {
					F_sideDistX += F_deltaDistX;
					mapX += stepX;
					if(Map[mapY+(mapX*32)] != 0) {
						side = false;
						break;
					}
				}
				else {
					F_sideDistY += F_deltaDistY;
					mapY += stepY;
					if(Map[mapY+(mapX*32)] != 0) {
						side = true;
						break;
					}
				}
			}

			uint24_t F_perpWallDist;

			if(!side) {
				F_perpWallDist = F_sideDistX - F_deltaDistX;
			}
			else {
				F_perpWallDist = F_sideDistY - F_deltaDistY;
			}

			uint24_t asm_perpWallDist = raycast(F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY);

			if(asm_perpWallDist != F_perpWallDist) {
				dbg_printf("Expected: %d\n", F_perpWallDist);
				dbg_printf("Vars: %d, %d, %d, %d, %d, %d\n", F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY);
				dbg_Debugger();

				raycast(F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY);
			}

			uint8_t texX;
			if(!side) {
				texX = posY + fxmul24(F_perpWallDist, F_rayDirY);
				//if(F_rayDirX > 0) {
				if(stepX != -1) {
					texX = (-texX)-1;
				}
			} else {
				texX = posX + fxmul24(F_perpWallDist, F_rayDirX);
				//if(F_rayDirY < 0) {
				if(stepY != 1) {
					texX = (-texX)-1;
				}
			}

			uint24_t texOff = tex_OffCalc(texX);*/

			uint24_t asm_perpWallDist = raycast(F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY, posX, posY);
			//uint24_t wall_height = 46080/F_perpWallDist;
			uint24_t wall_height = 46080/asm_perpWallDist;
			//gfx_TexturedVertLine(x, wall_height, door_texture_data + texOff);
			gfx_TexturedVertLine(x, wall_height, door_texture_data + 64);
		}

		//gfx_palette[1] = time_get_fps() << 11;
		//timer_1_Counter = 0;

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
