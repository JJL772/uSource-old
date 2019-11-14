/*

Vector implementation

*/
#include "vector.h"
#include "mconst.h"

#include <math.h>

#ifdef USE_SSE
#include <xmmintrin.h>
#include <immintrin.h>
#endif

/* Dummy union for vector 3 that allows us to store packed floats */
union alignas(16) UVector3
{
	Vector3 vec;
	float _vec[4];

	UVector3()
	{

	}
};

/* Dummy union for vector 2 that allows us to store packed floats */
union alignas(16) UVector2
{
	Vector2 vec;
	float _vec[4];

	UVector2()
	{

	}
};

Vector2::Vector2()
{
	this->v[0] = 0.0f;
	this->v[1] = 0.0f;
}

Vector2::Vector2(float v)
{
	x = v;
	y = v;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
}

Vector2::Vector2(Vector2&& other)
{
	this->x = other.x;
	this->y = other.y;
}

float Vector2::Length() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_set_ps(x,y,0.0f,0.0f);
	v1 = _mm_mul_ps(v1,v1);
	v1 = _mm_add_ps(v1, _mm_shuffle_ps(v1,v1,0b11100101));
	alignas(16) float res[4];
	_mm_store_ps(res, _mm_sqrt_ps(v1));
	return res[0];
#else
	return sqrtf(x*x + y*y);
#endif
}

Vector2 Vector2::Normalize() const
{
	float coe = sqrtf(x*x+y*y);
	return Vector2(x/coe,y/coe);
}

float Vector2::Dot(const Vector2& other) const
{
	return this->x * other.x + this->y * other.y;
}

/* Linear interp:
	this + bias * (other - this) */
Vector2 Vector2::Lerp(const Vector2& other, float bias) const
{
#ifdef USE_SSE
	__m128 v1 = _mm_set_ps(x,y,0.0f,0.0f);
	__m128 v2 = _mm_set_ps(x,y,0.0f,0.0f);
	__m128 _bias = _mm_set1_ps(bias);
	v2 = _mm_mul_ps(_bias, _mm_sub_ps(v2,v1));
	alignas(16) float res[4];
	_mm_store_ps(res, v2);
	return Vector2(res[0], res[1]);
#else
	return *this + bias * (other - *this);
#endif
}

bool Vector2::IsNAN() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_set_ps(x,y,0.0f,0.0f);
	return (_mm_movemask_ps(_mm_cmpunord_ps(v1, v1))) != 0;
#else
	return (isnanf(x) || isnanf(y));
#endif
}

float Vector2::Avg() const
{
	return (x+y) / 2;
}

Vector2 Vector2::AvgVector(const Vector2& other) const
{
#ifdef USE_SSE
	__m128 v1 = _mm_set_ps(x,y,0.0f,0.0f);
	__m128 v2 = _mm_set_ps(other.x,other.y,0.0f,0.0f);
	alignas(16) float res[4];
	_mm_store_ps(res, _mm_mul_ps(m128_half, _mm_add_ps(v1,v2)));
	return Vector2(res[0],res[1]);
#else
	return Vector2((x+other.x)/2.0f, (y+other.y)/2.0f);
#endif
}

void Vector2::Clear()
{
	x = 0.0f;
	y = 0.0f;
}

void Vector2::FPrintf(FILE* fd) const
{
	fprintf(fd, "Vector2: x=%f,y=%f\n", x,y);
}

void Vector2::Printf() const
{
	printf("Vector2 x=%f,y=%f\n", x,y);
}


/*=========================================

class vector3

===========================================*/
Vector3::Vector3(const Vector3& other)
{
	this->v[0] = other.v[0];
	this->v[1] = other.v[1];
	this->v[2] = other.v[2];
}

Vector3::Vector3(Vector3&& other)
{
	this->v[0] = other.v[0];
	this->v[1] = other.v[1];
	this->v[2] = other.v[2];
}

Vector3::Vector3(float f)
{
	this->v[0] = f;
	this->v[1] = f;
	this->v[2] = f;
}

Vector3::Vector3(float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector3::Vector3()
{
}

Vector3 Vector3::operator-() const
{
#ifdef USE_SSE
	UVector3 vec;
	__m128 _v = _mm_load_ps(v);
	_mm_store_ps(vec.vec.v, _mm_mul_ps(_v, m128_negone));
	return vec.vec;
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
	UVector3 _vec;
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	_mm_store_ps(_vec.vec.v, _mm_add_ps(v1,v2));
	return _vec.vec;
#else
	return Vector3(v[0]+vec.v[0], vec[1]+vec.v[1], vec[2]+vec.v[2]);
#endif
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
#ifdef USE_SSE
	UVector3 _vec;
	__m128 v1 = _mm_load_ps(v);
	__m128 v2 = _mm_load_ps(vec.v);
	_mm_store_ps(_vec.vec.v, _mm_sub_ps(v1,v2));
	return _vec.vec;
#else
	return Vector3(v[0]-vec.v[0], vec[1]-vec.v[1], vec[2]-vec.v[2]);
#endif
}

Vector3 Vector3::operator*(float v) const
{
#ifdef USE_SSE
	UVector3 vec;
	__m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_set_ps1(v);
	_mm_store_ps(vec.vec.v, _mm_mul_ps(_v, c));
	return vec.vec;
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
	UVector3 vec;
	__m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_rcp_ps(_mm_set_ps1(v)); /* Rcp then mul has lower latency than div */
	_mm_store_ps(vec.vec.v, _mm_mul_ps(_v, c));
	return vec.vec;
#else
	Vector3 vec = *this;
	vec.v[0] /= v;
	vec.v[1] /= v;
	vec.v[2] /= v;
	return vec;
#endif
}

/* Assignment */
Vector3& Vector3::operator=(const Vector3& other)
{
	this->v[0] = other.v[0];
	this->v[1] = other.v[1];
	this->v[2] = other.v[2];
	return *this;
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
	arr[0] = v[0];
	arr[1] = v[1];
	arr[2] = v[2];
}

void Vector3::CopyToArrayAligned(float* arr) const
{
	arr[0] = v[0];
	arr[1] = v[1];
	arr[2] = v[2];
}

/* Len = sqrt(x*x + y*y + z*z) */
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
	return sqrtf(x*x + y*y + z*z);
#endif
}

Vector3 Vector3::Normalize() const
{
#ifdef USE_SSE
	UVector3 vec;
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
	_mm_store_ps(vec.vec.v, res);
	return vec.vec;
#else
	Vector3 vec;
	float coe = sqrtf(x*x+y*y+z*z);
	vec.x = x/coe;
	vec.y = y/coe;
	vec.z = z/coe;
	return vec;
#endif
}

Vector2 Vector3::Make2D() const
{
	return Vector2(this->v[0], this->v[1]);
}

float Vector3::Length2D() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(this->v);
	v1 = _mm_mul_ps(v1,v1);
	__m128 v2 = _mm_shuffle_ps(v1, v1, 0b11100101);
	v1 = _mm_sqrt_ps(_mm_add_ps(v1,v2));
	alignas(16) float res[4];
	_mm_store_ps(res, v1);
	return res[0];
#else
	return sqrtf(x*x + y*y);
#endif
}

float Vector3::Dot(const Vector3& other) const
{
	return DotProduct(*this, other);
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return CrossProduct(*this, other);
}

float Vector3::Avg() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_shuffle_ps(v1, v1, 0b11100101);
	__m128 v3 = _mm_shuffle_ps(v1, v1, 0b11100110);
	alignas(16) float res[4];
	_mm_store_ps(res, _mm_mul_ps(m128_third, _mm_add_ps(v3,_mm_add_ps(v2,v1))));
	return res[0];
#else
	return (x+y+z)/3;
#endif
}

Vector3 Vector3::AvgVector(const Vector3& other) const
{
#ifdef USE_SSE
	UVector3 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_load_ps(other.v);
	_mm_store_ps(vec.vec.v, _mm_mul_ps(_mm_add_ps(v1,v2), m128_third));
	return vec.vec;
#else
	return Vector3((x+other.x)/2.0f, (y+other.y)/2.0f, (z+other.z)/2.0f);
#endif
}

float Vector3::Distance(const Vector3& other) const
{
#ifdef USE_SSE

#else
	return sqrt((other.x-x)*(other.x-x) + (other.y-y)*(other.y-y) + (other.z-z)*(other.z-z));
#endif
}

bool Vector3::IsNAN() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(v);
	v1 = _mm_shuffle_ps(v1,v1, 0b10100100); /* Last element can fudge off */
	return (_mm_movemask_ps(_mm_cmpunord_ps(v1,v1)) != 0);
#else
	return (isnanf(v[0]) || isnanf(v[1]) || isnanf(v[2]));
#endif
}

void Vector3::Clear()
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}

float Vector3::DotProductAbs(const Vector3& other) const
{
#if defined(USE_SSE) && !defined(USE_SSE41)
	/* x1*x2 + y1*y2 + z1*z2 */
	__m128 _v1 = _mm_xor_ps(_mm_load_ps(v), m128_abs);
	__m128 _v2 = _mm_xor_ps(_mm_load_ps(other.v), m128_abs);
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
	__m128 _v1 = _mm_load_ps(this->v);
	__m128 _v2 = _mm_load_ps(other.v);
	alignas(16) float ret[4];
	_mm_store_ps(ret, _mm_dp_ps(_v1, _v2, 0b01110000));
	return ret[0];
#else
	return (absf(x*other.x) + absf(y*other.y) + absf(z*other.z));
#endif //USE_SSE
}

/*
this + bias * (other - this)
*/
Vector3 Vector3::Lerp(const Vector3& other, float bias) const
{
#ifdef USE_SSE
	UVector3 vec;
	__m128 v1 = _mm_load_ps(other.v);
	__m128 v2 = _mm_load_ps(this->v);
	__m128 _bias = _mm_set1_ps(bias);
	_mm_store_ps(vec.vec.v, _mm_mul_ps(_bias, _mm_sub_ps(v2,v1)));
	return vec.vec;
#else
	return *this + bias * (other - *this);
#endif
}

float Vector3::Max() const
{
#ifdef USE_SSE
	/* Need to load these into 4 different xmm registers...ugh */
	__m128 v1 = _mm_load_ps(this->v);
	__m128 _2 = _mm_shuffle_ps(v1, v1, 0b11100101);
	__m128 _3 = _mm_shuffle_ps(v1, v1, 0b11100110);
	v1 = _mm_max_ps(v1, _mm_max_ps(_2, _3));
	alignas(16) float ret[4];
	return ret[0];
#else
	return (x > y ? (z > x ? z : x) : (z > y ? z : y));
#endif
}

float Vector3::NormalizeThis()
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(this->v);
	__m128 tmp = v1;
	v1 = _mm_mul_ps(v1,v1);
	__m128 v2 = _mm_add_ps(v1, _mm_shuffle_ps(v1, v1, 0b11100101));
	v2 = _mm_sqrt_ps(_mm_add_ps(v1, _mm_shuffle_ps(v1, v1, 0b11100110)));
	alignas(16) float res[4];
	v2 = _mm_shuffle_ps(v2, v2, 0b00000000); /* Sets all elements to the lowest elem */
	_mm_store_ps(this->v, _mm_mul_ps(tmp, _mm_rcp_ps(v2)));
	return res[0];
#else
	float coe = sqrtf(x*x + y*y + z*z);
	x /= coe;
	y /= coe;
	z /= coe;
	return coe;
#endif
}

Vector3 Vector3::operator+(float f) const
{
#ifdef USE_SSE
	volatile float ____f = 0.0;
	Vector3 vec;
	__m128 v1 = _mm_load_ps(v);
	v1 = _mm_add_ps(v1, _mm_set1_ps(f));
	_mm_store_ps(vec.v, v1);
	return vec;
#else
	return Vector3(v[0] + f, v[1] + f, v[2] + f);
#endif
}

Vector3 Vector3::operator-(float f) const
{
#ifdef USE_SSE
	volatile float ____f = 0.0;
	Vector3 vec;
	__m128 v1 = _mm_load_ps(v);
	v1 = _mm_sub_ps(v1, _mm_set1_ps(f));
	_mm_store_ps(vec.v, v1);
	return vec;
#else
	return Vector3(v[0] - f, v[1] - f, v[2] - f);
#endif
}

Vector3 Vector3::Abs() const
{
#ifdef USE_SSE
	volatile float f = 0.0f;
	Vector3 vec;
	__m128 v1 = _mm_load_ps(this->v);
	_mm_store_ps(vec.v, _mm_xor_ps(v1, m128_abs));
	return vec;
#else
	Vector3 vec;
	vec.v[0] = absf(this->v[0]);
	vec.v[1] = absf(this->v[1]);
	vec.v[2] = absf(this->v[2]);
	return vec;
#endif
}

void Vector3::AbsThis() const
{
#ifdef USE_SSE
	static const __m128 abs_tmp = _mm_set_ps(-0.0f, -0.0f, -0.0f, 0.0f);
	_mm_store_ps((float*)this->v, _mm_xor_ps(_mm_load_ps(this->v), abs_tmp));
#else
	this->x = absf(this->x);
	this->y = absf(this->y);
	this->z = absf(this->z);
#endif
}

void Vector3::FPrintf(FILE* fd) const
{
	fprintf(fd, "Vector3: x=%f,y=%f,z=%f\n", x, y, z);
}

void Vector3::Printf() const
{
	printf("Vector3: x=%f,y=%f,z=%f\n", x, y, z);
}

/*


Vector4 stuff


*/

/* Default constructor does nothing */
Vector4::Vector4()
{
}

Vector4::Vector4(const Vector4& vec)
{
#ifdef USE_SSE
	_mm_store_ps(v, _mm_load_ps(vec.v));
#else
	this->v[0] = other.v[0];
	this->v[1] = other.v[1];
	this->v[2] = other.v[2];
	this->v[3] = other.v[3];
#endif
}


Vector4::Vector4(float f)
{
#ifdef USE_SSE
	_mm_store_ps(v, _mm_set1_ps(f));
#else
	this->v[0] = f;
	this->v[1] = f;
	this->v[2] = f;
	this->v[3] = f;
#endif
}

Vector4::Vector4(Vector4 && vec)
{
#ifdef USE_SSE
	_mm_store_ps(v, _mm_load_ps(vec.v));
#else
	this->v[0] = other.v[0];
	this->v[1] = other.v[1];
	this->v[2] = other.v[2];
	this->v[3] = other.v[3];
#endif
}

Vector4::Vector4(float x, float y, float z, float m)
{
#ifdef USE_SSE
	_mm_store_ps(v, _mm_set_ps(x,y,z,m));
#else
	this->v[0] = x;
	this->v[1] = y;
	this->v[2] = z;
	this->v[3] = m;
#endif
}

Vector4 Vector4::Abs() const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	_mm_store_ps(vec.v, _mm_xor_ps(v1, m128_abs));
	return vec;
#else
	return Vector4(absf(x), absf(y), absf(z), absf(m));
#endif
}

void Vector4::AbsThis()
{
#ifdef USE_SSE
	_mm_store_ps(this->v, _mm_xor_ps(_mm_load_ps(this->v), m128_abs));
#else
	x = absf(x);
	y = absf(y);
	z = absf(z);
	m = absf(m);
#endif
}

float Vector4::Avg() const
{
#ifdef USE_SSE

#else
	return (x+y+z)/3.0f;
#endif
}

void Vector4::Clear()
{
#ifdef USE_SSE
	_mm_store_ps(this->v, m128_vec4clear);
#else
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	m = 1.0f;
#endif
}

void Vector4::CopyToArray(float* arr) const
{
#ifdef USE_SSE
	_mm_storeu_ps(arr, _mm_load_ps(this->v));
#else
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	arr[3] = m;
#endif
}

void Vector4::CopyToArrayAligned(float* arr) const
{
#ifdef USE_SSE
	_mm_store_ps(arr, _mm_load_ps(this->v));
#else
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	arr[3] = m;
#endif
}

float Vector4::Distance(const Vector4& other) const
{
}

float Vector4::Dot(const Vector4& other) const
{
}

bool Vector4::IsNAN() const
{
#ifdef USE_SSE

#else
	return isnanf(x) || isnanf(y) || isnanf(z);
#endif
}

float Vector4::Lerp(const Vector4& other, float bias) const
{
}

float Vector4::Max() const
{
#ifdef USE_SSE

#else
	return ((x > y ? (x > z ? x : z) : (y > z ? y : z)))*m;
#endif
}

Vector4 Vector4::DeNorm() const
{
#ifdef USE_SSE
	__m128 v1 = _mm_load_ps(this->v);
	__m128 c = _mm_shuffle_ps(v1,v1,0b11111111);
	__m128 res = _mm_and_ps(_mm_mul_ps(v1,c), m128_vec4low3hi0);
	Vector4 vec;
	_mm_store_ps(vec.v, res);
#else
	return Vector4(x*m,y*m,z*m,1.0f);
#endif //USE_SSE
}

void Vector4::DeNormThis() const
{
#ifdef USE_SSE

#else
	this->x = x*m;
	this->y = y*m;
	this->z = z*m;
	this->m = 1.0f;
#endif
}

Vector4 Vector4::Normalize() const
{
#ifdef USE_SSE
	const __m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_shuffle_ps(_v,_v,0b11111111);
	__m128 v2 = _mm_mul_ps(_v,c);
	__m128 v3 = _mm_mul_ps(v2,v3);
	__m128 v1 = _mm_shuffle_ps(v3,v3,0b11100101);
	v2 = _mm_shuffle_ps(v2,v2,0b11100110);
	c = _mm_sqrt_ps(_mm_add_ps(_mm_add_ps(v1,v2),v3));
	v1 = _mm_mul_ps(_mm_rcp_ps(c), _v);
	Vector4 vec;
	_mm_store_ps(vec.v, v1);
	return vec;
#else
	float coe = sqrtf((x*x+y*y+z*z) * m);
	return Vector4(x/coe,y/coe,z/coe,coe);
#endif
}

float Vector4::NormalizeThis() const
{
#ifdef USE_SSE
	const __m128 _v = _mm_load_ps(this->v);
	__m128 c = _mm_shuffle_ps(_v,_v,0b11111111);
	__m128 v2 = _mm_mul_ps(_v,c);
	__m128 v3 = _mm_mul_ps(v2,v3);
	__m128 v1 = _mm_shuffle_ps(v3,v3,0b11100101);
	v2 = _mm_shuffle_ps(v2,v2,0b11100110);
	c = _mm_sqrt_ps(_mm_add_ps(_mm_add_ps(v1,v2),v3));
	v1 = _mm_mul_ps(_mm_rcp_ps(c), _v);
	_mm_store_ps((float*)this->v, v1);
#else
	float c = sqrtf((x*x+y*y+z*z) * m);
	x /= c;
	y /= c;
	z /= c;
	m = 1.0f;
#endif
}

Vector4 Vector4::operator*(float f) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_set1_ps(f);
	_mm_store_ps(vec.v, _mm_mul_ps(v1,v2));
	return vec;
#else
	return Vector4(x*f,y*f,z*f,m*f);
#endif
}

Vector4 Vector4::operator+(const Vector4& other) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_load_ps(other.v);
	_mm_store_ps(vec.v, _mm_add_ps(v1,v2));
	return vec;
#else
	return Vector4(x+other.x,y+other.y,z+other.z,m+other.m);
#endif
}

Vector4 Vector4::operator+(float f) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	_mm_store_ps(vec.v, _mm_add_ps(v1, _mm_set1_ps(f)));
	return vec;
#else
	return Vector4(x+f,y+f,z+f,m+f);
#endif
}

Vector4 Vector4::operator-() const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	_mm_store_ps(vec.v, _mm_mul_ps(v1, m128_negone));
	return vec;
#else
	return Vector4(-x,-y,-z,-m);
#endif
}

Vector4 Vector4::operator-(const Vector4& other) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_load_ps(other.v);
	_mm_store_ps(vec.v, _mm_sub_ps(v1,v2));
	return vec;
#else
	return Vector4(v[0]-other.v[0], v[1]-other.v[1], v[2]-other.v[2], v[3]-other.v[3]);
#endif
}

Vector4 Vector4::operator-(float f) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_set1_ps(f);
	_mm_store_ps(vec.v, _mm_sub_ps(v1,v2));
	return vec;
#else
	return Vector4(x-f,y-f,z-f,m-f);
#endif
}

Vector4 Vector4::operator/(float f) const
{
#ifdef USE_SSE
	Vector4 vec;
	__m128 v1 = _mm_load_ps(this->v);
	__m128 v2 = _mm_set1_ps(f);
	_mm_store_ps(vec.v, _mm_mul_ps(_mm_rcp_ps(v2), v1));
	return vec;
#else
	return Vector4(x/f,y/f,z/f,m/f);
#endif
}

Vector3 Vector4::ToVector3() const
{
#ifdef USE_SSE
	Vector3 vec;
#else
	return Vector3(x*m,y*m,z*m);
#endif
}

void Vector4::Printf() const
{
	printf("Vector4: x=%f,y=%f,z=%f,m=%f\n", x,y,z,m);
}

void Vector4::FPrintf(FILE* fd) const
{
	fprintf(fd, "Vector4: x=%f,y=%f,z=%f,m=%f\n", x,y,z,m);
}

/*


Other functions provided


*/
float DotProduct(const Vector2& a, const Vector2& b)
{
	return (a.x*b.x + a.y*b.y);
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
	_mm_store_ps(ret, _mm_dp_ps(_v1, _v2, 0b01110000));
	return ret[0];
#elif defined(USE_NEON)

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
	UVector3 vec;
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
	return vec;
#elif defined(USE_NEON)

#else
	return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
#endif
}
