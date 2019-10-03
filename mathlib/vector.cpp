/*

Vector implementation

*/
#include "vector.h"
#include "mconst.h"

#ifdef USE_SSE
#include <xmmintrin.h>
#include <immintrin.h>
#endif 

Vector2::Vector2()
{
	this->v[0] = 0.0f;
	this->v[1] = 0.0f;
}

Vector2::Vector2(float v)
{

}

Vector2::Vector2(float x, float y)
{

}

Vector2::Vector2(const Vector2& other)
{

}

Vector2::Vector2(Vector2&& other)
{

}

float Vector2::Length() const
{

}

Vector2 Vector2::Normalize() const
{

}

float Vector2::Dot(const Vector2& other) const
{

}

/*=========================================

class vector3 

===========================================*/
Vector3::Vector3(const Vector3& other)
{
#ifdef USE_SSE
	_mm_store_ps(this->v, _mm_load_ps(other.v));
#else

#endif
}

Vector3::Vector3(Vector3&& other)
{
#ifdef USE_SSE
	_mm_store_ps(this->v, _mm_load_ps(other.v));
	_mm_store_ps(other.v, m128_zero);
#else

#endif
}

Vector3::Vector3(float f)
{
#ifdef USE_SSE
	_mm_store_ps(this->v, _mm_set_ps1(0.0f));
#else
	this->v[0] = f;
	this->v[1] = f;
	this->v[2] = f;
	this->v[3] = f;
#endif //USE_SSE
}

Vector3::Vector3(float x, float y, float z)
{
#ifdef USE_SSE
	_mm_store_ps(this->v, _mm_set_ps(x, y, z, 0.0f));
#else
	v[0] = x;
	v[1] = y;
	v[2] = z;
#endif
}

Vector3::Vector3()
{
}

Vector3 Vector3::operator-() const
{
#ifdef USE_SSE
	Vector3 vec;
	__m128 _v = _mm_load_ps(v);
	_mm_store_ps(vec.v, _mm_mul_ps(_v, m128_negone));
	return vec;
#else
	return Vector3(-v[0], -v[1], -v[2]);
#endif
}

bool Vector3::operator==(const Vector3& vec) const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	/* Shuffle because v[4] is undefined */
	v1 = _mm_shuffle_ps(v1, v1, 0b10100100);
	v2 = _mm_shuffle_ps(v2, v2, 0b10100100);
	/* Compare */
	return (_mm_movemask_ps(_mm_cmpeq_ps(v1,v2))==0);
#else
	return (v[0] == vec.v[0]) && (v[1] == vec.v[1]) && (v[2] == vec.v[2]);
#endif
}

bool Vector3::operator!=(const Vector3& vec) const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	/* Shuffle because v[4] is undefined */
	v1 = _mm_shuffle_ps(v1, v1, 0b10100100);
	v2 = _mm_shuffle_ps(v2, v2, 0b10100100);
	/* Compare */
	return (_mm_movemask_ps(_mm_cmpeq_ps(v1,v2))!=0);
#else
	return (v[0] != vec.v[0]) || (v[1] != vec.v[1]) || (v[2] != vec.v[2]);
#endif
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
#ifdef USE_SSE
	Vector3 _vec;
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	_mm_store_ps(_vec.v, _mm_add_ps(v1,v2));
#else
	return Vector3(v[0]+vec.v[0], vec[1]+vec.v[1], vec[2]+vec.v[2]);
#endif
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
#ifdef USE_SSE
	Vector3 _vec;
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	_mm_store_ps(_vec.v, _mm_sub_ps(v1,v2));
#else
	return Vector3(v[0]-vec.v[0], vec[1]-vec.v[1], vec[2]-vec.v[2]);
#endif
}

Vector3 Vector3::operator*(float v) const
{
#ifdef USE_SSE
	Vector3 vec;
	__m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_set_ps1(v);
	_mm_store_ps(vec.v, _mm_mul_ps(_v, c));
	return vec;
#else
	Vector3 vec = *this;
	vec.v[0] *= v;
	vec.v[1] *= v;
	vec.v[2] *= v;
	return vec;
#endif
}

Vector3 Vector3::operator/(float v) const
{
#ifdef USE_SSE
	Vector3 vec;
	__m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_rcp_ps(_mm_set_ps1(v)); /* Rcp then mul has lower latency than div */
	_mm_store_ps(vec.v, _mm_mul_ps(_v, c));
	return vec;
#else
	Vector3 vec = *this;
	vec.v[0] /= v;
	vec.v[1] /= v;
	vec.v[2] /= v;
	return vec;
#endif
}

Vector3::operator float *()
{
	return this->v;
}

Vector3::operator const float *() const
{
	return this->v;
}

void Vector3::CopyToArray(float* arr) const
{
#ifdef USE_SSE
	_mm_storeu_ps(arr, _mm_load_ps(v));
#else
	arr[0] = v[0];
	arr[1] = v[1];
	arr[2] = v[2];
#endif
}

void Vector3::CopyToArrayAligned(float* arr) const
{
#ifdef USE_SSE
	_mm_store_ps(arr, _mm_load_ps(v));
#else
	arr[0] = v[0];
	arr[1] = v[1];
	arr[2] = v[2];
#endif
}

float Vector3::Length() const
{
#ifdef USE_SSE
	__m128 _v = _mm_load_ps(v);
	/* Square em */
	_v = _mm_mul_ps(_v, _v);
	__m128 res = _v;
	/* Shuffle once so that v[0] + v[1] */
	_v = _mm_shuffle_ps(_v, _v, 0b11100101);
	res = _mm_add_ps(res, _v);
	/* Shuffle again so that v[1] + v[2] */
	_v = _mm_shuffle_ps(_v, _v, 0b11101010);
	res = _mm_add_ps(res, _v);
	res = _mm_sqrt_ps(res);
	alignas(16) float ret[4]; /* NOTE: Alignment must be enforced here or else we could have crashes */
	_mm_storeu_ps(ret, res);
	return ret[0];
#else

#endif
}

Vector3 Vector3::Normalize() const
{
#ifdef USE_SSE
	/* sqrt(x*x+y*y+z*z) */
	__m128 _v = _mm_load_ps(v);
	__m128 _tmp = _v;
	/* Square em */
	_v = _mm_mul_ps(_v, _v);
	__m128 res = _v;
	/* Shuffle once so that v[0] + v[1] */
	_v = _mm_shuffle_ps(_v, _v, 0b11100101);
	res = _mm_add_ps(res, _v);
	/* Shuffle again so that v[1] + v[2] */
	_v = _mm_shuffle_ps(_v, _v, 0b11101010);
	res = _mm_add_ps(res, _v);
	res = _mm_sqrt_ps(res);
	/* The result will be in the lowest position here */
	res = _mm_shuffle_ps(res, res, 0b00000000);
	res = _mm_mul_ps(_mm_rcp_ps(res), _tmp);
	Vector3 vec;
	_mm_store_ps(vec.v, res);
	return vec;
#else

#endif
}

Vector2 Vector3::Make2D() const
{

}

float Vector3::Length2D() const
{

}

float Vector3::Dot(const Vector3& other) const
{
	return DotProduct(*this, other);
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return CrossProduct(*this, other);
}



/* Other functions provided */
float DotProduct(const Vector2& a, const Vector2& b)
{
	return (a.v[0]*b.v[0] + a.v[1]*b.v[1]);
}

float DotProduct(const Vector3& v1, const Vector3& v2)
{
#if defined(USE_SSE) && !defined(USE_SSE41)
	/* x1*x2 + y1*y2 + z1*z2 */
	__m128 _v1 = _mm_load_ps(v1.v);
	__m128 _v2 = _mm_load_ps(v2.v);
	__m128 res = _mm_mul_ps(_v1, _v2);
	_v1 = res;
	_v1 = _mm_shuffle_ps(_v1, _v1, 0b11100101);
	res = _mm_add_ps(res, _v1);
	_v1 = _mm_shuffle_ps(_v1, _v1, 0b11100110);
	res = _mm_add_ps(res, _v1);
	alignas(16) float ret[4]; /* KEEP ALIGNED */
	_mm_store_ps(ret, res);
	return ret[0];
#elif defined(USE_SSE41)
	__m128 _v1 = _mm_load_ps(v1.v);
	__m128 _v2 = _mm_load_ps(v2.v);
	alignas(16) float ret[4];
	_mm_store_ps(ret, _mm_dp_ps(v1, v2, 0b01110000));
	return ret[0];
#else
	return (v1.v[0]*v2.v[0] + v1.v[1]*v2.v[1] + v1.v[2]*v2.v[2]);
#endif //USE_SSE
}

/*	x = y1*z2 - z1*y2
	y = z1*x2 - x1*z2
	z = x1*y2 - y1*x2 */
Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
{
#ifdef USE_SSE
	Vector3 vec;
	__m128 _v1 = _mm_load_ps(v1.v);
	__m128 _v2 = _mm_load_ps(v2.v);
	__m128 res = _mm_set1_ps(0.0f);
	/* First pass will do the move y,z,x then z,x,y */
	res = _mm_mul_ps(_mm_shuffle_ps(_v1, _v1, 0b11001001),
		_mm_shuffle_ps(_v2, _v2, 0b11100001));
	/* Noew we move z,x,y and y,z,x */
	res = _mm_sub_ps(res, _mm_mul_ps(_mm_shuffle_ps(_v1, _v1, 0b11010010),
		_mm_shuffle_ps(_v2, _v2, 0b11011000)));
	_mm_store_ps(vec.v, res);
#else

#endif
}
