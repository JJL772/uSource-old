/*

PLANE.H

A plain plane class. Replaces the GoldSrc/Xash3d plane_t

Jeremy Lorelli, Sep. 2019

*/
#pragma once

#include "vector.h"

#include "common/types.h"

typedef struct mplane_s
{
	vec3_t		normal;
	float		dist;
	byte		type;		// for fast side tests
	byte		signbits;		// signx + (signy<<1) + (signz<<1)
	byte		pad[2];
} mplane_t;

