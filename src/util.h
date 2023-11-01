#pragma once

#include <stdint.h>
#include "lut/linestep.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Sets a color pixel with duplication trick potential
 * @note Pixel is not clipped at all and allows for horizontal duplication trick
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] c Color
 */
extern void gfx_SetPixel2_NoClip(uint24_t x, uint8_t y, uint24_t c);

/**
 * Draws a flatly colored vertical line with duplication trick potential
 * @note Line is not clipped at all and allows for horizontal duplication trick
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] length Length of the line
 * @param[in] c Color
 */
extern void _gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint16_t length, uint24_t c);

/**
 * Draws a textured vertical line
 * @note Line is not clipped
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] length Length of the line
 * @param[in] texture Pointer to the texture drawn
 * @param[in] delta Step size for each pixel (in fixed point)
 */
extern void _gfx_TexturedVertLine_NoClip(uint24_t x, uint8_t y, uint16_t length,
		uint8_t* texture, uint16_t delta);

#ifdef __cplusplus
}
#endif

static inline void gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint8_t length,
		uint24_t c)
{
	if(length > 200) return;
	_gfx_VertLine2_NoClip(x, y, 600 - (3*length), c);
}

static inline void gfx_TexturedVertLine_NoClip(uint24_t x, uint8_t y, uint16_t length,
		uint8_t* texture)
{
	//if(length > 180) length = 180;
	_gfx_TexturedVertLine_NoClip(x, y, 1620 - (9*length), texture, delta_lut[length]);
}
