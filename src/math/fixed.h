#pragma once

#include <stdint.h>

typedef int24_t fixed24;

#define FIX_SHIFT       8
#define FIX_SCALE       (1<<FIX_SHIFT)
#define FIX_MASK        (FIX_SCALE-1)
#define FIX_SCALEF      ((float)FIX_SCALE)
#define FIX_SCALEF_INV  (1.0/FIX_SCALEF)
#define FIX_MAX         (fixed24)8388607

#define FIX_ONE         FIX_SCALE

// Convert int to fixed24
#define int2fx(x) (fixed24)(x << FIX_SHIFT)
// Convert float to fixed24
#define float2fx(x) (fixed24)(x * FIX_SCALEF)
// Convert fixed24 to unsigned int
#define fx2uint(x) (uint24_t)(x >> FIX_SHIFT)
// Get fractional bit from fixed24
#define fx2ufrac(x) (fixed24)(x & FIX_MASK)
// Convert fixed24 to int
#define fx2int(x) (int24_t)(x / FIX_SCALE)
// Convert fixed24 to float
#define fx2float(x) (float)(x / FIX_SCALEF)
// Multiply two fixed24 numbers
#define fxmul(a, b) (fixed24)((a * b) >> FIX_SHIFT)
// Divide two fixed24 numbers
#define fxdiv(a, b) (fixed24)(((a) * FIX_SCALE) / (b))

// Multiply two fixed24 numbers (the first is stored as a uint8_t)
// The fixed24 has to be within bounds of a signed 16-bit int
extern fixed24 fxmul8abs(uint8_t a, fixed24 b);

// Multiply two fixed 24 numbers (the first is stored as an int8_t)
// The fixed24 has to be within bounds of a signed 16-bit int
extern fixed24 fxmul8(int8_t a, fixed24 b);

// Multiply two fixed24 numbers (both are positive)
// Both fixed24s have to be within bounds of a signed 16-bit int
extern fixed24 fxmul24abs(fixed24 a, fixed24 b);

// Multiply two fixed24 numbers (positive or negative)
// Both fixed24s have to be within bounds of a signed 16-bit int
extern fixed24 fxmul24(fixed24 a, fixed24 b);

#define FIX_PI          804
