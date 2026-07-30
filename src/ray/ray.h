#pragma once

#include <stdint.h>
#include "../math/fixed.h"
#include "../gfx/texture.h"

#ifdef __cplusplus
extern "C" {
#endif

fixed24 posX = int2fx(22), posY = int2fx(12);

fixed24 F_rayDirX;
fixed24 F_rayDirY;

uint24_t F_deltaDistX;
uint24_t F_deltaDistY;

extern uint24_t raycast();

#ifdef __cplusplus
}
#endif
