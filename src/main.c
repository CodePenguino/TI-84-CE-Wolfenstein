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

/*static inline void check_inputs(float* x, float* y, float* dirX, float* dirY) {
	if(key_pressed(kb_Up)) {
		if(worldMap[(int)(x + dirX)][(int)(y)] == false) posX += dirX * moveSpeed;
		if(worldMap[(int)(x)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
	}
}*/

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

	float posX = 22, posY = 12; // x and y start position
	float dirX = -1, dirY = 0; // initial direction vector
	float planeX = 0, planeY = 1; // the 2d raycaster version of camera plane

	const float moveSpeed = 0.5f;

	do {
		key_update();
		//check_inputs(&posX, &posY);

		if(key_pressed(kb_Up)) {
			if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == false)
				posX += dirX * moveSpeed;
			if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == false)
				posY += dirY * moveSpeed;
		}
		if(key_pressed(kb_Down)) {
			if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == false)
				posX -= dirX * moveSpeed;
			if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == false)
				posY -= dirY * moveSpeed;
		}

		for(int x = 0; x < 160; x++) {
			//calculate ray position and direction
			float cameraX = 2 * x / (float)160 - 1; //x-coordinate in camera space
			float rayDirX = dirX + planeX * cameraX;
			float rayDirY = dirY + planeY * cameraX;
			//which box of the map we're in
			int mapX = (int)(posX);
			int mapY = (int)(posY);

			float sideDistX;
			float sideDistY;

			float deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
			float deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);

			float perpWallDist;

			int8_t stepX;
			int8_t stepY;

			bool hit = false; //was there a wall hit?
			bool side; //was a NS or a EW wall hit?
			//calculate step and initial sideDist
			if(rayDirX < 0) {
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else {
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}
			if(rayDirY < 0) {
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else {
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}
			//perform DDA
			while(hit == false) {
				//jump to next map square, either in x-direction, or in y-direction
				if(sideDistX < sideDistY) {
					sideDistX += deltaDistX;
					mapX += stepX;
					side = false;
				}
				else {
					sideDistY += deltaDistY;
					mapY += stepY;
					side = true;
				}
				//Check if ray has hit a wall
				if(worldMap[mapX][mapY] > 0) {
					hit = true;
				}
			}

			if(!side) {
				perpWallDist = (sideDistX - deltaDistX);
			}
			else {
				perpWallDist = (sideDistY - deltaDistY);
			}

			uint24_t lineHeight = (int)(180 / perpWallDist);

			//draw the pixels of the stripe as a vertical line
			gfx_TexturedVertLine(x, lineHeight, texture_data);
		}

		/*gfx_Wait();
		for(uint8_t i = 0; i <= 159; i++) {
			line_length = (240-((127+lu_sin(timer+(i*x)))>>3)-y)<<1;
			gfx_TexturedVertLine(i, line_length, texture_data + (i<<6) % 4096);
		}*/

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
