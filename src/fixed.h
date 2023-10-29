#pragma once

#include <stdint.h>

typedef int24_t fixed24;

#define FIX_SHIFT       8 
#define FIX_SCALE       ( 1<<FIX_SHIFT		)
#define FIX_MASK        ( FIX_SCALE-1		)
#define FIX_SCALEF      ( (float)FIX_SCALE	)
#define FIX_SCALEF_INV	( 1.0/FIX_SCALEF	)

#define FIX_ONE         FIX_SCALE

// Get fixed24 point reciprocal of a, in fp fractional bits
#define FX_RECIPROCAL(a, fp) ( ((1<<(fp))+(a)-1)/(a) )

// Division approximation by reciprocal multiplication. Basically x * (1 / a)
#define FX_RECIMUL(x, a, fp) ( ((x)*((1<<(fp))+(a)-1)/(a))>>(fp) )

// TODO: Maybe change to assembly instructions? ¯\_(ツ)_/¯

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
