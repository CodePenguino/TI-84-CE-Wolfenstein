#pragma once

extern const uint8_t SineTable[64];
extern int8_t _getSinCos();

inline int8_t lu_sin(uint8_t n)
{
	// Load n variable into the a register
	__asm__("ld a, %0" : : "r" (n));
	return _getSinCos();
}

/*#ifdef __cplusplus
extern "C" {
#endif
	int24_t SineTable[65];
	int24_t getSinCos(int24_t x);
#ifdef __cplusplus
}
#endif*/