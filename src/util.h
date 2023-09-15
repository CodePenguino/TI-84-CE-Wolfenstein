#pragma once

#define gfx_vbuffer16 \
(**(uint16_t(**)[])0xE30014)

// TODO: use gfx_SetColor (for textures not useful, for plain colors yes...)
inline void gfx_SetPixel2(uint24_t x, uint8_t y, uint8_t c)
{
	//gfx_vbuffer16[(y*GFX_LCD_WIDTH+x)>>1] = dup8(c);
	gfx_vbuffer16[(y*160+x)] = dup8(c);
}

/*#ifdef __cplusplus
extern "C" {
#endif
//extern void gfx_SetPixel(uint24_t x, uint8_t y);
extern uint16_t gfx_SetColor16(uint16_t index);
#ifdef __cplusplus
}
#endif*/