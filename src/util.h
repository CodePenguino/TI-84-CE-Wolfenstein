#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
*	Sets a color pixel with duplication trick potential
*	@note Pixel is not clipped at all and allows for horizontal duplication trick
* @param[in] x X coordinate
* @param[in] y Y coordinate
* @param[in] c Color
*/
extern void gfx_SetPixel2_NoClip(uint24_t x, uint8_t y, uint24_t c);

/**
*	Draws a vertical line with duplication trick potential
*	@note Line is not clipped at all and allows for horizontal duplication trick
* @param[in] x X coordinate
* @param[in] y Y coordinate
* @param[in] length length of the line
* @param[in] c Color
*/
extern void _gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint16_t length, uint24_t c);
//extern void draw_column_tall(uint8_t *column, uint8_t *texture, uint24_t texture_delta, uint8_t texture_fraction);

extern void _gfx_TexturedVertLine_NoClip(uint24_t x, uint8_t y, uint16_t length, uint8_t* texture, uint24_t delta);

extern void _gfx_TexturedVertLine2_NoClip(uint24_t x, uint8_t y, uint16_t length, uint8_t* texture, uint24_t delta);

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
		uint8_t* texture, uint24_t delta)
{
	if(length > 180) length = 180;
	if(y < 0) y = 0;
	_gfx_TexturedVertLine_NoClip(x, y, 1620 - (9*length), texture, delta);
}
