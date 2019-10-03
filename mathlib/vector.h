/*

VECTOR.H

Replacement for Valve's vector types.

*/
#pragma once

/* 2 component vector that's unaligned. */
/* This is NOT accelerated by SIMD except through compiler optimizations */
class Vector2
{
public:
	float v[2];
public:
	Vector2();

	Vector2(float v);

	Vector2(float x, float y);

	Vector2(const Vector2& other);

	Vector2(Vector2&& other);

public: /* operators */
	inline Vector2 operator+(const Vector2& other) const { return Vector2(v[0] + other.v[0], v[1] + other.v[1]); };
	inline Vector2 operator-(const Vector2& other) const { return Vector2(v[0] - other.v[0], v[1] - other.v[1]); };
	inline Vector2 operator*(float f) const { return Vector2(v[0] * f, v[1] * f); };
	inline Vector2 operator/(float f) const { return Vector2(v[0] / f, v[1] / f); };

public: /* methods */
	float Length() const;

	Vector2 Normalize() const;

public: /* extensions to this */
	float Dot(const Vector2& other) const;
};

/* 3 component vector aligned to 16-byte boundaries for SIMD stuff */
class alignas(16) Vector3
{
public:
	float v[3];
private:
	float _r; /* This is an extra to allow for padding */
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
	Vector3 operator+(const Vector3& vec) const;
	Vector3 operator-(const Vector3& vec) const;
	Vector3 operator*(float v) const;
	Vector3 operator/(float v) const;
	/* Casts */
	operator float*();
	operator const float*() const;

public: /* additional functions */
	
	/* copy to unaligned array */
	void CopyToArray(float* arr) const;
	
	/* copy to aligned array */
	void CopyToArrayAligned(float* arr) const;
	
	/* returns the length */
	float Length() const;

	/* Normalize vector */
	Vector3 Normalize() const;

	/* Turn this into a 2d vector */
	Vector2 Make2D() const;

	float Length2D() const;

public: /* extensions to this class */

	/* Returns dot product between this and another vector */
	float Dot(const Vector3& other) const;

	/* returns cross product between this and another vector */
	Vector3 Cross(const Vector3& other) const;

	/* Sine of all elements in this class */
	Vector3 Sin() const;

	/* Cosine of all elements in this class */
	Vector3 Cos() const;

	/* Tangent of all elements in this class */
	Vector3 Tan() const;
};

/* Other functions provided */
float DotProduct(const Vector2& a, const Vector2& b);
inline Vector2 operator*(float f, const Vector2& v) { return v * f; };

inline Vector3 operator*(float f, const Vector3& v) { return v * f; };
float DotProduct(const Vector3& v1, const Vector3& v2);
Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

/* Alias these types to be used in engine code/game code */
#define Vector3D Vector3
#define Vector Vector3
#define Vector2D Vector2

/* Structures for fast SIMD vector processing */
class alignas(16) Fast4Vector
{
public:
	float x[4];
	float y[4];
	float z[4];

public:
	Fast4Vector();
	Fast4Vector(const Fast4Vector& other);
	Fast4Vector(Fast4Vector&& other);
	Fast4Vector(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4);
	Fast4Vector(const Vector3& v);
public:
	/*==============================
	Basic data handling 
	================================*/
	/* Swap a vector */
	Vector3 SwapVector(int slot, const Vector3& _new);
	/* Clear a vector */
	inline void ClearVector(int slot);
	/* Insert a vector */
	inline void SetVector(int slot, const Vector3& _new);
	/* Clear all slots */
	inline void Clear();
	/* Set all vectors */
	inline void SetVectors(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4);
	/* Get vector in slot */
	inline Vector3 GetVector(int slot) const;
	/* Get all vectors */
	inline void GetVector(Vector3& out1, Vector3& out2, Vector3& out3, Vector3& out4) const;
	/*==============================
	Transformations and stuff
	================================*/
	/* Apply a scale in x y and z directions */
	inline void ApplyScale(float x, float y, float z);
	/* Apply a scale provided by the vector */
	inline void ApplyScale(const Vector3& scale);
	/* Apply an offset to all x, y and z vectors */
	inline void ApplyOffset(float x, float y, float z);
	/* Apply an offset provided by the vector */
	inline void ApplyOffset(const Vector3& offsets);
	/* Apply a transformation matrix */
	inline void ApplyTransform(const class matrix3x3 mat);
};

class alignas(16) Fast8Vector
{
public:
	float x[8];
	float y[8];
	float z[8];

public:
	Fast8Vector();
	Fast8Vector(const Fast8Vector& other);
	Fast8Vector(Fast8Vector&& other);
};