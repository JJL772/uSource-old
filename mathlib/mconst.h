/*

Mathematical constants!

*/
#pragma once

#ifdef USE_SSE
#include <xmmintrin.h>
#include <immintrin.h>
#endif

#ifdef USE_SSE
#define _DEFINE_CONST128(name, val) const static __m128 name = _mm_set_ps1(val)
#define _DEFINE_CONST128D(name, val) const static __m128d name = _mm_set_pd1(val)
#define _DEFINE_CONSTVEC128(name, x,y,z,m) const static __m128 name = _mm_set_ps(x,y,z,m)
#define _DEFINE_CONSTVEC128D(name, x,y) const static __m128 name = _mm_set_pd(x,y)
#elif defined(USE_NEON)
#define _DEFINE_CONST128(name, val) /* stub for now */
#define _DEFINE_CONST128D(name, val) /* stub for now */
#define _DEFINE_CONSTVEC128(name, x,y,z,m) 
#define _DEFINE_CONSTVEC128D(name, x,y) 
#endif

#define DEFINE_CONSTANT128(name, val) _DEFINE_CONST128(name, val)
#define DEFINE_CONSTANT128D(name, val) _DEFINE_CONST128D(name, val)
#define DEFINE_CONSTVEC128(name, x, y, z, m) _DEFINE_CONSTVEC128(name, x,y,z,m)
#define DEFINE_CONSTVEC128D(name, x, y) _DEFINE_CONSTVEC128D(name,x,y)

/* Doing an xorps with this constant will clear the sign */
DEFINE_CONSTANT128	(m128_abs,	-0.0f);
DEFINE_CONSTANT128D	(m128d_abs,	-0.0);
DEFINE_CONSTANT128	(m128_zero, 0.0f);
DEFINE_CONSTANT128D (m128d_zero,0.0);
DEFINE_CONSTANT128	(m128_one,	1.0f);
DEFINE_CONSTANT128D	(m128d_one,	1.0);
DEFINE_CONSTANT128	(m128_negone, -1.0f);
DEFINE_CONSTANT128D	(m128d_negone, -1.0);
DEFINE_CONSTANT128	(m128_pi, 	3.141592653589793);
DEFINE_CONSTANT128D	(m128d_pi, 	3.14159265358979323846264338327950288);
DEFINE_CONSTANT128	(m128_e, 	2.718281828459045);
DEFINE_CONSTANT128D	(m128d_e, 	2.71828182845904523536028747135266250);
DEFINE_CONSTANT128	(m128_ten,	10.0f);
DEFINE_CONSTANT128D	(m128d_ten,	10.0);
DEFINE_CONSTANT128	(m128_third,1.0f/3.0f);
DEFINE_CONSTANT128D	(m128d_third,1.0/3.0);
DEFINE_CONSTANT128	(m128_half, 1.0f/2.0f);
DEFINE_CONSTANT128D (m128d_half, 1.0/2.0);
DEFINE_CONSTANT128	(m128_fourth, 1.0f/4.0f);
DEFINE_CONSTANT128D (m128d_fourth, 1.0/4.0);

DEFINE_CONSTVEC128	(m128_vec4clear, 0.0f,0.0f,0.0f,1.0f);