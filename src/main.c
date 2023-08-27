#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdio.h>
#include "fixed.h"
#include "color.h"
#include "input.h"
#include <string.h>
#include "util.h"

void FillScreen(uint16_t color)
{
	memset(lcd_Ram, color, (320*240)<<1);
}

/* Main function, called first */
int main(void)
{
	/* Clear the homescreen */
	os_ClrHome();

	do
	{
		key_update();

		FillScreen(RGB15(31,31,31));
		for(uint8_t y = 0; y < 30; y++)
		{
			memset(lcd_Ram+(y*LCD_WIDTH*2), RGB15(31,0,0), 60);
			//for(uint8_t x = 0; x < 30; x++)
			//{
			//	plot(x, y, RGB15(31,0,0));
			//}
		}
	} while (kb_Data[1] != kb_2nd);

	/* Return 0 for success */
	return 0;
}