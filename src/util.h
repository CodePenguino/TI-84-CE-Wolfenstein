#pragma once

#define gfx_vbuffer16 \
(**(uint16_t(**)[])0xE30014)

// TODO: use gfx_SetColor (for textures not useful, for plain colors yes...)

#ifdef __cplusplus
extern "C" {
#endif
extern void _gfx_SetPixel2_NoClip(uint24_t x, uint8_t y, uint16_t c);
#ifdef __cplusplus
}
#endif

static inline void gfx_SetPixel2(uint24_t x, uint8_t y, uint8_t c)
{
	_gfx_SetPixel2_NoClip(x, y, c);
}

/*#ifdef __cplusplus
extern "C" {
#endif
//extern void gfx_SetPixel(uint24_t x, uint8_t y);
extern uint16_t gfx_SetColor16(uint16_t index);
#ifdef __cplusplus
}
#endif*/