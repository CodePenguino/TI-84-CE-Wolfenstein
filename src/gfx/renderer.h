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

uint24_t RENDER_tex_length;
uint24_t RENDER_other_length;
uint24_t RENDER_texture_offset;
uint24_t RENDER_delta;

/**
 * Sets a color pixel
 * @note Pixel is not clipped at all
 * @param[in] x X coordinate
 * @param[in] y Y coordinate
 * @param[in] c Color
 */
extern void gfx_SetPixel2_NoClip(uint8_t x, uint8_t y, uint8_t c);

extern void _gfx_TexturedVertLine_Partial();
extern void _gfx_TexturedVertLine_Full();

#ifdef __cplusplus
}
#endif

// Draw a vertical line on the screen
static inline void gfx_TexturedVertLine() {
	if(RENDER_length < 180) {
        RENDER_tex_length = line_length_lut[RENDER_length];
        RENDER_other_length = RENDER_length;
        RENDER_delta = texture_lut_u24[RENDER_length];

		_gfx_TexturedVertLine_Partial();
	} else {
		RENDER_length -= 180;
        RENDER_texture_offset = texture_offset_lut[RENDER_length];
        RENDER_delta = texture_lut_u8[RENDER_length];

		_gfx_TexturedVertLine_Full();
	}
}
