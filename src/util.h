#pragma once

#include <sys/lcd.h>

volatile uint16_t* VRAM = (uint16_t*)0xD40000;

#define plot(x, y, clr) (VRAM[y*LCD_WIDTH+x]=clr)