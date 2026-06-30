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

// Vars
//#define mapX 8
//#define mapY 8
//#define mapS 64

// TODO: Have this map do literally anything
int worldMap[24][24]=
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

int main(void) {
	// Calls _boot_InitializeHardware
	asm("call $000384");
	// Clear homescreen and set up gfx api
	os_ClrHome();
	gfx_Begin();
	gfx_SetDrawScreen();

	// Set up wolfenstein color palette
	gfx_SetPalette(global_palette, sizeof(global_palette), 0);

	fixed24 x = 2, y = 170;
	uint8_t timer = 0;

	time_enable();
	texture_init();
	set_scaled_mode();

	// Draw blue border
	gfx_FillScreen(6);
	gfx_SwapDraw();
	gfx_FillScreen(6);

	uint24_t line_length = 0;

	fixed24 posX = int2fx(22), posY = int2fx(12);
	fixed24 dirX = -FIX_ONE, dirY = 0;
	fixed24 planeX = 0, planeY = FIX_ONE;

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

		planeX = -lu_sin(rotation)<<1;
		planeY = lu_cos(rotation)<<1;

		gfx_Wait();
		for(int x = 0; x < 160; x++) {
			//calculate ray position and direction
			fixed24 cameraX = camera_x_lut[x]; //x-coordinate in camera space
			fixed24 F_rayDirX = dirX + fxmul(planeX, cameraX);
			fixed24 F_rayDirY = dirY + fxmul(planeY, cameraX);

			//which box of the map we're in
			uint8_t mapX = fx2int(posX);
			uint8_t mapY = fx2int(posY);

			fixed24 F_sideDistX;
			fixed24 F_sideDistY;

			fixed24 F_deltaDistX = (F_rayDirX == 0) ? INT24_MAX : abs(fxdiv(256, F_rayDirX));
			fixed24 F_deltaDistY = (F_rayDirY == 0) ? INT24_MAX : abs(fxdiv(256, F_rayDirY));

			//dbg_printf("(%d, %d) - (%d, %d)\n", F_rayDirX, F_rayDirY, F_deltaDistX, F_deltaDistY);

			fixed24 F_perpWallDist;

			int8_t stepX;
			int8_t stepY;

			bool hit = false; //was there a wall hit?
			bool side; //was a NS or a EW wall hit?
			//calculate step and initial sideDist
			if(F_rayDirX < 0) {
				stepX = -1;
				F_sideDistX = fxmul((uint8_t)posX, F_deltaDistX);
			}
			else {
				stepX = 1;
				F_sideDistX = fxmul((uint8_t)(-posX), F_deltaDistX);
			}
			if(F_rayDirY < 0) {
				stepY = -1;
				F_sideDistY = fxmul((uint8_t)posY, F_deltaDistY);
			}
			else {
				stepY = 1;
				F_sideDistY = fxmul((uint8_t)(-posY), F_deltaDistY);
			}
			//perform DDA
			while(hit == false) {
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
					hit = true;
				}
			}

			if(!side) {
				F_perpWallDist = F_sideDistX - F_deltaDistX;
			}
			else {
				F_perpWallDist = F_sideDistY - F_deltaDistY;
			}

			uint24_t line_height = int2fx(180) / F_perpWallDist;

			//draw the pixels of the stripe as a vertical line
			if(side)
				gfx_TexturedVertLine(x, line_height, texture_data);
			else
			 	gfx_TexturedVertLine(x, line_height, texture_data+64);
		}

		//dbg_printf("%lu\n", time_get_fps());
		timer_1_Counter = 0;
		timer++;

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
