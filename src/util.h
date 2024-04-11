#pragma once

#include <stdint.h>
#include "lut/linestep.h"
//#include "math/fixed.h"

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
 * @param[in] y Y coordinate
 * @param[in] length Length of the line
 * @param[in] texture Pointer to the texture drawn
 * @param[in] delta Step size for each pixel (in fixed point)
 */
extern void _gfx_TexturedVertLine_Partial(uint24_t x, uint8_t y,
		uint24_t length, uint24_t otherLength, uint8_t* texture, uint16_t delta);

extern void _gfx_TexturedVertLine_Full(uint24_t x, uint8_t* texture,
		uint16_t delta);

#ifdef __cplusplus
}
#endif

/*static inline void gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint8_t length,
		uint24_t c)
{
	if(length > 200) return;
	_gfx_VertLine2_NoClip(x, y, 600 - (3*length), c);
}*/

static inline void gfx_TexturedVertLine(uint24_t x,
		uint24_t length, const uint8_t* texture)
{
	if(length < 180)
	{
		//int8_t ceiling_length = (180-length)/2;           //(120-(length>>1))-30;
		//                               otherLength = 180 - (2*((180-length)/2))
		_gfx_TexturedVertLine_Partial(x, 0, 1620 - (9*length), (length*3)>>1,
			(uint8_t*)texture, delta_lut[length]);
	}
	else
	{
		_gfx_TexturedVertLine_Full(x, (uint8_t*)texture+
			texture_offset_lut[length-180], delta_lut[length]);
			//((90-delta_lut[length])>>1), delta_lut[length]);
	}
}
