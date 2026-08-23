#pragma once

#include <stdint.h>
#include "lut/linestep.h"
#include "math/fixed.h"
#include "texture.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t RENDER_x;

uint16_t RENDER_length;

uint16_t RENDER_tex_length;
uint16_t RENDER_texture_offset;
uint16_t RENDER_delta;

/**
 * Sets a color pixel
 * @note Pixel is not clipped at all
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] c Color
 */
extern void gfx_SetPixel2_NoClip(uint8_t x, uint8_t y, uint8_t c);

// Draw a vertical line on the screen
extern void gfx_TexturedVertLine();

#ifdef __cplusplus
}
#endif
