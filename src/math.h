#pragma once

// Duplicate u8 to form a u16; (ex.) 0x12 -> 0x1212
#define dup8(x) (uint16_t)(x | (x<<8))
// inline uint16_t dup8(uint8_t x) { return  }

#ifdef __cplusplus
extern "C" {
#endif
extern int8_t _getSinCos();
#ifdef __cplusplus
}
#endif

inline int8_t lu_sin(uint8_t x)
{
	// Load n variable into the a register
	asm("ld a, %0" :: "r" (x));
	return _getSinCos();
}

inline int8_t lu_cos(uint8_t x)
{
	// Load n variable into the a register
	asm("ld a, %0" :: "r" (x));
	// Add 64 to the a register; cos(x) = 64 + sin(x) (assuming there's 256 total angles...)
	asm("add a, 64");
	return _getSinCos();
}