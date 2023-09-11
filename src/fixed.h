#pragma once

typedef int24_t fixed24;

#define FIX_SHIFT       7 
#define FIX_SCALE       ( 1<<FIX_SHIFT		)
#define FIX_MASK        ( FIX_SCALE-1		)
#define FIX_SCALEF      ( (float)FIX_SCALE	)
#define FIX_SCALEF_INV	( 1.0/FIX_SCALEF	)

#define FIX_ONE         FIX_SCALE

// Get fixed24 point reciprocal of a, in fp fractional bits
#define FX_RECIPROCAL(a, fp) ( ((1<<(fp))+(a)-1)/(a) )

// Division approximation by reciprocal multiplication. Basically x * (1 / a)
#define FX_RECIMUL(x, a, fp) ( ((x)*((1<<(fp))+(a)-1)/(a))>>(fp) )

// static inline fixed24 int2fx(int d);
// static inline fixed24 float2fx(float f);
// static inline uint24_t fx2uint(fixed24 fx);
// static inline uint24_t fx2ufrac(fixed24 fx);
// static inline int fx2int(fixed24 fx);
// static inline float fx2float(fixed24 fx);
// static inline fixed24 fxmul(fixed24 fa, fixed24 fb);
// static inline fixed24 fxdiv(fixed24 fa, fixed24 fb);

// Convert int to fixed24
#define int2fx(x) ({ x << FIX_SHIFT; })
// Convert float to fixed24
#define float2fx(x) ({ (fixed24)(x * FIX_SCALEF); })
// Convert fixed24 to unsigned int
#define fx2uint(x) ({ x >> FIX_SHIFT; })
// Get fractional bit from fixed24
#define fx2ufrac(x) ({ x & FIX_MASK; })
// Convert fixed24 to int
#define fx2int(x) ({ x / FIX_SCALE; })
// Convert fixed24 to float
#define fx2float(x) ({ x / FIX_SCALEF; })
// Multiply two fixed24 numbers
#define fxmul(a, b) ({ (a * b) >> FIX_SHIFT; })
// Divide two fixed24 numbers
#define fxdiv(a, b) ({ ((a) * FIX_SCALE) / (b); })

// Convert int to fixed24
// static inline fixed24 int2fx(int d)                 { return d << FIX_SHIFT; }
// Convert float to fixed24
// static inline fixed24 float2fx(float f)             { return (fixed24)(f * FIX_SCALEF); }
// Convert fixed24 to unsigned int
// static inline uint24_t fx2uint(fixed24 fx)          { return fx >> FIX_SHIFT; }
// Get fractional bit from fixed24
// static inline uint24_t fx2ufrac(fixed24 fx)         { return fx & FIX_MASK; }
// Convert fixed24 to int
// static inline int fx2int(fixed24 fx)                { return fx / FIX_SCALE; }
// Convert fixed24 to float
// static inline float fx2float(fixed24 fx)            { return fx / FIX_SCALEF; }
// Multiply two fixed24 numbers
// static inline fixed24 fxmul(fixed24 fa, fixed24 fb) { return (fa * fb) >> FIX_SHIFT; }
// Divide two fixed24 numbers
// static inline fixed24 fxdiv(fixed24 fa, fixed24 fb) { return ((fa) * FIX_SCALE) / (fb); }