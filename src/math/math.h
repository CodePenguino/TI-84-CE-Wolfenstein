#pragma once

#include <stdint.h>

// Duplicate u8 to form a u16; (ex.) 0x12 -> 0x1212
#define dup8(x) (uint16_t)(x | (x<<8))

#define max(a, b) ({\
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a > _b ? _a : _b; })

#define min(a, b) ({\
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a < _b ? _a : _b; })

#define clamp(x, mn, mx) ({\
	__typeof__ (x) _x = (x); \
	__typeof__ (mn) _mn = (mn); \
	__typeof__ (mx) _mx = (mx); \
	max(_mn, min(_mx, _x)); })

//#define abs(x) ((x < 0) ? -(x) : (x))



#ifdef __cplusplus
extern "C" {
#endif
extern int8_t _getSinCos();
extern int8_t _getSinCosNeg();
extern uint24_t abs24(int24_t x);
#ifdef __cplusplus
}
#endif

static inline int8_t lu_sin(uint8_t x) {
	// Load x variable into the a register
	asm("ld a, %0" :: "r" (x));
	return _getSinCos();
}

static inline int8_t lu_cos(uint8_t x) {
	// Load x variable into the a register
	asm("ld a, %0" :: "r" (x));
	// Add 64 to the a register; cos(x) = sin(x + 64) (assuming there's 256 total angles...)
	asm("add a, 64");
	return _getSinCos();
}

// Same as lu_sin but result is negative
static inline int8_t lu_sinneg(uint8_t x) {
	asm("ld a, %0" :: "r" (x));
	return _getSinCosNeg();
}

// Same as lu_cos but result is negative
static inline int8_t lu_cosneg(uint8_t x) {
	// Load x variable into the a register
	asm("ld a, %0" :: "r" (x));
	// Add 64 to the a register; cos(x) = sin(x + 64) (assuming there's 256 total angles...)
	asm("add a, 64");
	return _getSinCosNeg();
}
