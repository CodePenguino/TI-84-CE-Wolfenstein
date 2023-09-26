#pragma once

/*asm ("\n\t"
"	repeat 200\n\t"
"		_DrawVert#%:\n\t"
"			repeat %\n\t"
"				ld  (hl),bc\n\t"
"				add hl,de\n\t"
"			end repeat\n\t"
"			ret\n\t"
"	end repeat");*/

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
extern void gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint8_t length, uint24_t c);
#ifdef __cplusplus
}
#endif

void __gfx_VertLine2_NoClip(uint24_t x, uint8_t y, uint8_t length, uint24_t c)
{
	gfx_VertLine2_NoClip(x, y, (3*(length*length) + 5*length)>>1, c);
}