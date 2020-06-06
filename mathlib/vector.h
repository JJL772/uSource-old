/*

VECTOR.H

Replacement for Valve's vector types.

*/
#pragma once

/* Redefs of stuff from good ol gold source */
typedef float vec_t;

#include <stdint.h>
#include <stdio.h>

#ifdef NEW_VEC_TYPES

/*=========================================================

Vector2

===========================================================*/

/* 2 component vector that's unaligned. */
/* This is NOT accelerated by SIMD except through compiler optimizations */
class Vector2
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		struct
		{
			uint32_t ux, uy;
		};
		float v[2];
	};
public:
	Vector2();

	Vector2(float v);

	Vector2(float x, float y);

	Vector2(const Vector2& other);

	Vector2(Vector2&& other);

public: /* operators */
	inline Vector2 operator+(float f) const { return Vector2(v[0] + f, v[1] + f); };
	inline Vector2 operator-(float f) const { return Vector2(v[0] - f, v[1] - f); };
	inline Vector2 operator+(const Vector2& other) const { return Vector2(v[0] + other.v[0], v[1] + other.v[1]); };
	inline Vector2 operator-(const Vector2& other) const { return Vector2(v[0] - other.v[0], v[1] - other.v[1]); };
	inline Vector2 operator*(float f) const { return Vector2(v[0] * f, v[1] * f); };
	inline Vector2 operator/(float f) const { return Vector2(v[0] / f, v[1] / f); };
	/* Assignment */
	inline Vector2& operator=(const Vector2& other)
	{
		v[0] = other.v[0];
		v[1] = other.v[1];
		return *this;
	}

public: /* methods */
	float Length() const;

	/* Clear this vector */
	void Clear();

	/* Return a normalizaed version of this vector */
	Vector2 Normalize() const;

	/* Returns a vector with the average of this vector */
	Vector2 AvgVector(const Vector2& other) const;

	/* Returns the average of all components in this vector */
	float Avg() const;

	/* Returns the dot product between this vector and another vector */
	float Dot(const Vector2& other) const;

	/* Linear interpolation */
	Vector2 Lerp(const Vector2& other, float bias) const;

	/* Check if any components are NAN */
	bool IsNAN() const;

public:
	/* Extras for debugging and the likes */

	/* Prints this vector to a stream */
	void FPrintf(FILE* fd) const;

	/* Prints this to stdout */
	void Printf() const;
};

/*=========================================================

Vector3

===========================================================*/

/* 3 component vector aligned to 16-byte boundaries for SIMD stuff */
class alignas(16) Vector3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		struct /* Used for absolute value and such */
		{
			uint32_t ux, uy, uz;
		};
		float v[3];
	};
public:
	Vector3(const Vector3& other);

	Vector3(Vector3&& other);

	Vector3(float f);

	Vector3(float x, float y, float z);

	Vector3();

public:
	Vector3 operator-() const;

	bool operator==(const Vector3& vec) const;

	bool operator!=(const Vector3& vec) const;

	Vector3 operator+(float f) const;

	Vector3 operator-(float f) const;

	Vector3 operator+(const Vector3& vec) const;

	Vector3 operator-(const Vector3& vec) const;

	Vector3 operator*(float v) const;

	Vector3 operator/(float v) const;

	/* Casts */
	operator float*();

	operator const float*() const;

	/* operator= stuff */
	Vector3& operator=(const Vector3& other);

public: /* additional functions */

	/* Set everything to zero */
	void Clear();

	/* copy to unaligned array */
	void CopyToArray(float* arr) const;

	/* copy to aligned array */
	void CopyToArrayAligned(float* arr) const;

	/* returns the length */
	float Length() const;

	/* Normalize vector */
	Vector3 Normalize() const;

	/* Normalize this, return length */
	float NormalizeThis();

	/* Turn this into a 2d vector */
	Vector2 Make2D() const;

	float Length2D() const;

	float DotProductAbs(const Vector3& other) const;

	/* Returns the biggest element in this vector */
	float Max() const;

	/* Average of all components in this vector */
	float Avg() const;

	/* Returns a new vector with the average of all components */
	Vector3 AvgVector(const Vector3& other) const;

	/* Returns dot product between this and another vector */
	float Dot(const Vector3& other) const;

	/* returns cross product between this and another vector */
	Vector3 Cross(const Vector3& other) const;

	/* Distance to another vector */
	float Distance(const Vector3& other) const;

	/* Linear interplolation with another vector */
	Vector3 Lerp(const Vector3& other, float bias) const;

	/* Check if any components are invalid */
	bool IsNAN() const;

	/* Return absolute value of all components */
	Vector3 Abs() const;

	/* Compute absolute value and store result in this vector */
	void AbsThis();

public:
	/* Extras for debugging and the likes */

	/* Prints this vector to a stream */
	void FPrintf(FILE* fd) const;

	/* Prints this to stdout */
	void Printf() const;

};

/*=========================================================

Vector4

===========================================================*/

class alignas(16) Vector4
{
public:
	union
	{
		float v[4];
		struct
		{
			float x,y,z,m;
		};
		struct
		{
			uint32_t ux,uy,uz,um;
		};
	};
public:
	Vector4(float x, float y, float z, float m);

	Vector4(float f);

	Vector4();

	Vector4(const Vector4& vec);

	Vector4(Vector4&& vec);
public:
	Vector4 operator-() const;
	Vector4 operator+(float f) const;
	Vector4 operator-(float f) const;
	Vector4 operator+(const Vector4& other) const;
	Vector4 operator-(const Vector4& other) const;
	Vector4 operator*(float f) const;
	Vector4 operator/(float f) const;
	operator float*();
	operator const float*() const;
	Vector4& operator=(const Vector4& other);

public:
	void CopyToArray(float* arr) const;

	void CopyToArrayAligned(float* arr) const;

	void Clear();

	bool IsNAN() const;

	float Dot(const Vector4& other) const;

	Vector3 ToVector3() const;

	/* Normalize this vector */
	Vector4 Normalize() const;

	/* Normalize this vector and return the magnitude */
	float NormalizeThis();

	/* Return the biggest element in this vector */
	float Max() const;

	/* Average all elements out */
	float Avg() const;

	/* Linear interplolation */
	float Lerp(const Vector4& other, float bias) const;

	/* Distance between this vector and another */
	float Distance(const Vector4& other) const;

	/* Absolute value of all components of this vecotr */
	Vector4 Abs() const;

	/* Store absolute value of all vector components */
	void AbsThis();

	/* Denormalize this and return */
	Vector4 DeNorm() const;

	/* Denormalize this */
	void DeNormThis();

public:
	/* Extras for debugging and the likes */

	/* Prints this vector to a stream */
	void FPrintf(FILE* fd) const;

	/* Prints this to stdout */
	void Printf() const;

};


/* Other functions provided */
float 	DotProduct			(const Vector2& a, const Vector2& b);
inline 	Vector2 operator*	(float f, const Vector2& v) { return v * f; };
inline 	Vector3 operator*	(float f, const Vector3& v) { return v * f; };
float 	DotProduct			(const Vector3& v1, const Vector3& v2);
Vector3 CrossProduct		(const Vector3& v1, const Vector3& v2);
#endif

/* Alias these types to be used in engine code/game code */
#ifdef NEW_VEC_TYPES
#define Vector3D Vector3
#define Vector Vector3
#define Vector2D Vector2
#define vec3_t Vector3
#define Vector4D Vector4
#define vec4_t Vector4
#else
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec2_t[2];
#endif

#ifdef NEW_VEC_TYPES
inline bool VectorIsNAN(const Vector3& v) { return v.IsNAN(); };
//#define VectorIsNAN(v) (IS_NAN(v[0]) || IS_NAN(v[1]) || IS_NAN(v[2]))
//#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])

inline float DotProductAbs(const Vector3& x, const Vector3& y) { return x.DotProductAbs(y); };
//#define DotProductAbs(x,y) (abs((x)[0]*(y)[0])+abs((x)[1]*(y)[1])+abs((x)[2]*(y)[2]))

inline float DotProductFabs(const Vector3& x, const Vector3& y) { return DotProductAbs(x,y); };
//#define DotProductFabs(x,y) (fabs((x)[0]*(y)[0])+fabs((x)[1]*(y)[1])+fabs((x)[2]*(y)[2]))

inline void CrossProduct(const Vector3& x, const Vector3& y, Vector3& z) { z = x.Cross(y); };
//#define CrossProduct(a,b,c) ((c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1],(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2],(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0])

inline void Vector2Subtract(const Vector2& x, const Vector2& y, Vector2& z) { z = x - y; };
//#define Vector2Subtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1])

inline void VectorSubtract(const Vector3& x, const Vector3& y, Vector3& z) { z = x - y; };
//#define VectorSubtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])

inline void Vector2Add(const Vector2& x, const Vector2& y, Vector2& z) { z = x + y; };
//#define Vector2Add(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1])

inline void VectorAdd(const Vector3& x, const Vector3& y, Vector3& z) { z = x + y; };
//#define VectorAdd(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])

inline void VectorAddScalar(const Vector3& x, float y, Vector3& z) { z = x + y; };
//#define VectorAddScalar(a,b,c) ((c)[0]=(a)[0]+(b),(c)[1]=(a)[1]+(b),(c)[2]=(a)[2]+(b))

inline void Vector2Copy(const Vector2& x, Vector2& y) { y = x; };
//#define Vector2Copy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1])

inline void VectorCopy(const Vector3& x, Vector3& y) { y = x; };
//#define VectorCopy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])

inline void Vector4Copy(const Vector3& x, Vector3& y) { y = x; };
//#define Vector4Copy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

inline void VectorScale(const Vector3& x, float s, Vector3& y) { y = x * s; };
//#define VectorScale(in, scale, out) ((out)[0] = (in)[0] * (scale),(out)[1] = (in)[1] * (scale),(out)[2] = (in)[2] * (scale))

inline bool VectorCompare(const Vector3& x, const Vector3& y) { return x==y; };
//#define VectorCompare(v1,v2)	((v1)[0]==(v2)[0] && (v1)[1]==(v2)[1] && (v1)[2]==(v2)[2])

/* Out = x/q */
inline void VectorDivide(const Vector3& x, float q, Vector3& out) { out = x/q; };
//#define VectorDivide( in, d, out ) VectorScale( in, (1.0f / (d)), out )

inline float VectorMax(const Vector3& vec) { return vec.Max(); };
//#define VectorMax(a) ( max((a)[0], max((a)[1], (a)[2])) )

inline float VectorAvg(const Vector3& vec) { return vec.Avg(); };
//#define VectorAvg(a) ( ((a)[0] + (a)[1] + (a)[2]) / 3 )

inline float VectorLength(const Vector3& vec) { return vec.Length(); };
//#define VectorLength(a) ( sqrt( DotProduct( a, a )))

inline float VectorDistance(const Vector3& x, const Vector3& y) { return x.Dot(y); };
//#define VectorLength2(a) (DotProduct( a, a ))

inline float VectorDistance2(const Vector3& x, const Vector3& y) { return x.Distance(y); };
//#define VectorDistance(a, b) (sqrt( VectorDistance2( a, b )))
//#define VectorDistance2(a, b) (((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + ((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + ((a)[2] - (b)[2]) * ((a)[2] - (b)[2]))

inline void Vector2Average(const Vector2& x, const Vector2& y, Vector2& out) { out = x.AvgVector(y); };
//#define Vector2Average(a,b,o)	((o)[0]=((a)[0]+(b)[0])*0.5,(o)[1]=((a)[1]+(b)[1])*0.5)

inline void Vector3Average(const Vector3& x, const Vector3& y, Vector3& out) { out = x.AvgVector(y); };
//#define VectorAverage(a,b,o)	((o)[0]=((a)[0]+(b)[0])*0.5,(o)[1]=((a)[1]+(b)[1])*0.5,(o)[2]=((a)[2]+(b)[2])*0.5)

inline void Vector2Set(Vector2& v, float x, float y) { v = Vector2(x,y); };
//#define Vector2Set(v, x, y) ((v)[0]=(x),(v)[1]=(y))

inline void VectorSet(Vector& v, float x, float y, float z) { v = Vector3(x,y,z); };
//#define VectorSet(v, x, y, z) ((v)[0]=(x),(v)[1]=(y),(v)[2]=(z))

#define Vector4Set(v, a, b, c, d) ((v)[0]=(a),(v)[1]=(b),(v)[2]=(c),(v)[3] = (d))

inline void VectorClear(Vector& vec) { vec.Clear(); };
//#define VectorClear(x) ((x)[0]=(x)[1]=(x)[2]=0)

inline void Vector2Lerp(const Vector2& v1, float bias, const Vector2& v2, Vector2& out) { out = v1.Lerp(v2, bias); };
//#define Vector2Lerp( v1, lerp, v2, c ) ((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]))

inline void VectorLerp(const Vector3& v1, float bias, const Vector3& v2, Vector3& out) { out = v1.Lerp(v2,bias); };
//#define VectorLerp( v1, lerp, v2, c ) ((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]), (c)[2] = (v1)[2] + (lerp) * ((v2)[2] - (v1)[2]))

inline void VectorNormalize(Vector3& vec) { vec = vec.Normalize();};
//#define VectorNormalize( v ) { float ilength = (float)sqrt(DotProduct(v, v));if (ilength) ilength = 1.0f / ilength;v[0] *= ilength;v[1] *= ilength;v[2] *= ilength; }

inline void VectorNormalize2(const Vector3& vec, Vector3& dest) { dest = vec.Normalize(); };
//#define VectorNormalize2( v, dest ) {float ilength = (float)sqrt(DotProduct(v,v));if (ilength) ilength = 1.0f / ilength;dest[0] = v[0] * ilength;dest[1] = v[1] * ilength;dest[2] = v[2] * ilength; }

inline void VectorNormalizeFast(Vector3& vec) { vec = vec.Normalize(); };
//#define VectorNormalizeFast( v ) {float	ilength = (float)rsqrt(DotProduct(v,v)); v[0] *= ilength; v[1] *= ilength; v[2] *= ilength; }

inline void VectorNormalizeLength(Vector3& v) { v.NormalizeThis(); };
//#define VectorNormalizeLength( v ) VectorNormalizeLength2((v), (v))

inline void VectorNegate(const Vector3& x, Vector3& y) { y = -x; };
//#define VectorNegate(x, y) ((y)[0] = -(x)[0], (y)[1] = -(x)[1], (y)[2] = -(x)[2])

#define VectorM(scale1, b1, c) ((c)[0] = (scale1) * (b1)[0],(c)[1] = (scale1) * (b1)[1],(c)[2] = (scale1) * (b1)[2])

#define VectorMA(a, scale, b, c) ((c)[0] = (a)[0] + (scale) * (b)[0],(c)[1] = (a)[1] + (scale) * (b)[1],(c)[2] = (a)[2] + (scale) * (b)[2])

#define VectorMAM(scale1, b1, scale2, b2, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2])

#define VectorMAMAM(scale1, b1, scale2, b2, scale3, b3, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0] + (scale3) * (b3)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1] + (scale3) * (b3)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2] + (scale3) * (b3)[2])

#define VectorIsNull( v ) ((v)[0] == 0.0f && (v)[1] == 0.0f && (v)[2] == 0.0f)

#define MakeRGBA( out, x, y, z, w ) Vector4Set( out, x, y, z, w )

#define PlaneDist(point,plane) ((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal))

#define PlaneDiff(point,plane) (((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal)) - (plane)->dist)

#define bound( min, num, max ) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

#elif !defined(NO_VECTOR_FUNCTIONS)

#define VectorIsNAN(v) (IS_NAN(v[0]) || IS_NAN(v[1]) || IS_NAN(v[2]))
#define DotProduct(x,y) ((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define DotProductAbs(x,y) (abs((x)[0]*(y)[0])+abs((x)[1]*(y)[1])+abs((x)[2]*(y)[2]))
#define DotProductFabs(x,y) (fabs((x)[0]*(y)[0])+fabs((x)[1]*(y)[1])+fabs((x)[2]*(y)[2]))
#define CrossProduct(a,b,c) ((c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1],(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2],(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0])
#define Vector2Subtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1])
#define VectorSubtract(a,b,c) ((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define Vector2Add(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1])
#define VectorAdd(a,b,c) ((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorAddScalar(a,b,c) ((c)[0]=(a)[0]+(b),(c)[1]=(a)[1]+(b),(c)[2]=(a)[2]+(b))
#define Vector2Copy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1])
#define VectorCopy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define Vector4Copy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#define VectorScale(in, scale, out) ((out)[0] = (in)[0] * (scale),(out)[1] = (in)[1] * (scale),(out)[2] = (in)[2] * (scale))
#define VectorCompare(v1,v2)	((v1)[0]==(v2)[0] && (v1)[1]==(v2)[1] && (v1)[2]==(v2)[2])
#define VectorDivide( in, d, out ) VectorScale( in, (1.0f / (d)), out )
#define VectorMax(a) ( max((a)[0], max((a)[1], (a)[2])) )
#define VectorAvg(a) ( ((a)[0] + (a)[1] + (a)[2]) / 3 )
#define VectorLength(a) ( sqrt( DotProduct( a, a )))
#define VectorLength2(a) (DotProduct( a, a ))
#define VectorDistance(a, b) (sqrt( VectorDistance2( a, b )))
#define VectorDistance2(a, b) (((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + ((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + ((a)[2] - (b)[2]) * ((a)[2] - (b)[2]))
#define Vector2Average(a,b,o)	((o)[0]=((a)[0]+(b)[0])*0.5f,(o)[1]=((a)[1]+(b)[1])*0.5f)
#define VectorAverage(a,b,o)	((o)[0]=((a)[0]+(b)[0])*0.5f,(o)[1]=((a)[1]+(b)[1])*0.5f,(o)[2]=((a)[2]+(b)[2])*0.5f)
#define Vector2Set(v, x, y) ((v)[0]=(x),(v)[1]=(y))
#define VectorSet(v, x, y, z) ((v)[0]=(x),(v)[1]=(y),(v)[2]=(z))
#define Vector4Set(v, a, b, c, d) ((v)[0]=(a),(v)[1]=(b),(v)[2]=(c),(v)[3] = (d))
#define VectorClear(x) ((x)[0]=(x)[1]=(x)[2]=0)
#define Vector2Lerp( v1, lerp, v2, c ) ((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]))
#define VectorLerp( v1, lerp, v2, c ) ((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]), (c)[2] = (v1)[2] + (lerp) * ((v2)[2] - (v1)[2]))
#define VectorNormalize( v ) { float ilength = (float)sqrt(DotProduct(v, v));if (ilength) ilength = 1.0f / ilength;v[0] *= ilength;v[1] *= ilength;v[2] *= ilength; }
#define VectorNormalize2( v, dest ) {float ilength = (float)sqrt(DotProduct(v,v));if (ilength) ilength = 1.0f / ilength;dest[0] = v[0] * ilength;dest[1] = v[1] * ilength;dest[2] = v[2] * ilength; }
#define VectorNormalizeFast( v ) {float	ilength = (float)rsqrt(DotProduct(v,v)); v[0] *= ilength; v[1] *= ilength; v[2] *= ilength; }
#define VectorNormalizeLength( v ) VectorNormalizeLength2((v), (v))
#define VectorNegate(x, y) ((y)[0] = -(x)[0], (y)[1] = -(x)[1], (y)[2] = -(x)[2])
#define VectorM(scale1, b1, c) ((c)[0] = (scale1) * (b1)[0],(c)[1] = (scale1) * (b1)[1],(c)[2] = (scale1) * (b1)[2])
#define VectorMA(a, scale, b, c) ((c)[0] = (a)[0] + (scale) * (b)[0],(c)[1] = (a)[1] + (scale) * (b)[1],(c)[2] = (a)[2] + (scale) * (b)[2])
#define VectorMAM(scale1, b1, scale2, b2, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2])
#define VectorMAMAM(scale1, b1, scale2, b2, scale3, b3, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0] + (scale3) * (b3)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1] + (scale3) * (b3)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2] + (scale3) * (b3)[2])
#define VectorIsNull( v ) ((v)[0] == 0.0f && (v)[1] == 0.0f && (v)[2] == 0.0f)
#define MakeRGBA( out, x, y, z, w ) Vector4Set( out, x, y, z, w )
#define PlaneDist(point,plane) ((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal))
#define PlaneDiff(point,plane) (((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal)) - (plane)->dist)
#define bound( min, num, max ) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

#define VectorM(scale1, b1, c) ((c)[0] = (scale1) * (b1)[0],(c)[1] = (scale1) * (b1)[1],(c)[2] = (scale1) * (b1)[2])

#define VectorMA(a, scale, b, c) ((c)[0] = (a)[0] + (scale) * (b)[0],(c)[1] = (a)[1] + (scale) * (b)[1],(c)[2] = (a)[2] + (scale) * (b)[2])

#define VectorMAM(scale1, b1, scale2, b2, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2])

#define VectorMAMAM(scale1, b1, scale2, b2, scale3, b3, c) ((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0] + (scale3) * (b3)[0],(c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1] + (scale3) * (b3)[1],(c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2] + (scale3) * (b3)[2])

#define VectorIsNull( v ) ((v)[0] == 0.0f && (v)[1] == 0.0f && (v)[2] == 0.0f)

#define MakeRGBA( out, x, y, z, w ) Vector4Set( out, x, y, z, w )

#define PlaneDist(point,plane) ((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal))

#define PlaneDiff(point,plane) (((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal)) - (plane)->dist)

#define bound( min, num, max ) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

#endif