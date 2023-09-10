#pragma once

#ifdef __cplusplus
extern "C" {
#endif
extern const uint8_t SineTable[64];
extern int8_t _getSinCos();
#ifdef __cplusplus
}
#endif

inline int8_t lu_sin(uint8_t x)
{
	// Load n variable into the a register
	__asm__("ld a, %0" : : "r" (x));
	return _getSinCos();
}

inline int8_t lu_cos(uint8_t x)
{
	// Load n variable into the a register
	__asm__("ld a, %0" : : "r" (x));
	// Add 64 to the a register; cos(x) = 64 + sin(x)
	__asm__("add a, 64");
	return _getSinCos();
}