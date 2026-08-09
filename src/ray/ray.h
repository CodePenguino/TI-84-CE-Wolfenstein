#pragma once

#include <stdint.h>
#include "../math/fixed.h"
#include "../gfx/texture.h"

#ifdef __cplusplus
extern "C" {
#endif

fixed16 posX = int2fx(22), posY = int2fx(12);

fixed16 camera_x;

fixed16 F_rayDirX;
fixed16 F_rayDirY;

uint16_t F_deltaDistX;
uint16_t F_deltaDistY;

extern uint16_t raycast();

#ifdef __cplusplus
}
#endif
