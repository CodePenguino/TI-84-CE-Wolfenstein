#pragma once

#include <stdint.h>
#include "../math/fixed.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint24_t raycast(uint8_t x, fixed24 F_rayDirX, fixed24 F_rayDirY,
    uint24_t F_deltaDistX, uint24_t F_deltaDistY, fixed24 posX, fixed24 posY);

#ifdef __cplusplus
}
#endif
