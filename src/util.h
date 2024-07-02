#pragma once

#include <stdint.h>
#include "lut/linestep.h"
//include "math/fixed.h"

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
//extern void _gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint24_t length,
//		uint24_t c);

/**
 * Draws a textured vertical line
 * @note Line is not clipped
 * @param[in] x X coordinate
 * @param[in] length Length of the line
 * @param[in] other_length Length of the ceiling/floor
 * @param[in] texture Pointer to the texture drawn
 * @param[in] delta Step size for each pixel (in fixed point)
 */
extern void _gfx_TexturedVertLine_Partial(uint24_t x, uint24_t length,
	uint24_t other_length, const uint8_t* texture, uint24_t delta);

extern void _gfx_TexturedVertLine_Full(uint24_t x, const uint8_t* texture,
	uint24_t texture_offset, uint24_t delta);

extern void _gfx_VertLine_NoClip();

extern void _gfx_VertLine_Scuffed(uint24_t x, uint24_t length, uint8_t color);

#ifdef __cplusplus
}
#endif

static inline void gfx_VertLine_Scuffed(uint24_t x, uint24_t length,
		uint8_t color) {
	_gfx_VertLine_Scuffed(x, 180-(length<<1), color);
}

// Draw a vertical line on the screen
static inline void gfx_TexturedVertLine(uint24_t x, uint24_t length,
		const uint8_t* texture) {
	if(length < 180) {
		// other_length = 180 - (2*((180-length)/2))
		_gfx_TexturedVertLine_Partial(x, 1260 - (7*length), (length>>1)<<1,
			texture, texture_lut_u24[length]);
	}
	else {
		length -= 180;
		_gfx_TexturedVertLine_Full(x, texture,
			texture_offset_lut[length], texture_lut_u8[length]);
	}
}
