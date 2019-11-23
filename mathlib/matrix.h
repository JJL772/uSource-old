/*

MATRIX.H

Matrix math stuff!

*/
#pragma once

#include "vector.h"
#include "plane.h"

class alignas(16) Matrix3x4
{
private:
	float mat[3][4];
public:
};

class alignas(16) Matrix4x4
{
private:
	float mat[4][4];
public:
};

#define matrix3x4 Matrix3x4
#define matrix4x4 Matrix4x4

#define Matrix3x4_LoadIdentity( mat )		Matrix3x4_Copy( mat, matrix3x4_identity )
#define Matrix3x4_Copy( out, in )		memcpy( out, in, sizeof( matrix3x4 ))

void Matrix3x4_VectorTransform( const matrix3x4& in, const float v[3], float out[3] );
void Matrix3x4_VectorITransform( const matrix3x4& in, const float v[3], float out[3] );
void Matrix3x4_VectorRotate( const matrix3x4& in, const float v[3], float out[3] );
void Matrix3x4_VectorIRotate( const matrix3x4& in, const float v[3], float out[3] );
void Matrix3x4_ConcatTransforms( matrix3x4& out, const matrix3x4& in1, const matrix3x4& in2 );
void Matrix3x4_FromOriginQuat( matrix3x4& out, const vec4_t& quaternion, const vec3_t& origin );
void Matrix3x4_CreateFromEntity( matrix3x4& out, const vec3_t& angles, const vec3_t& origin, float scale );
void Matrix3x4_TransformPositivePlane( const matrix3x4& in, const vec3_t& normal, float d, vec3_t& out, float *dist );
void Matrix3x4_TransformAABB( const matrix3x4& world, const vec3_t& mins, const vec3_t& maxs, vec3_t& absmin, vec3_t& absmax );
void Matrix3x4_SetOrigin( matrix3x4& out, float x, float y, float z );
void Matrix3x4_Invert_Simple( matrix3x4& out, const matrix3x4& in1 );
void Matrix3x4_OriginFromMatrix( const matrix3x4& in, float *out );
void Matrix3x4_AnglesFromMatrix( const matrix3x4& in, vec3_t& out );
void Matrix3x4_Transpose( matrix3x4& out, const matrix3x4& in1 );

#define Matrix4x4_LoadIdentity( mat )	Matrix4x4_Copy( mat, matrix4x4_identity )
#define Matrix4x4_Copy( out, in )	memcpy( out, in, sizeof( matrix4x4 ))

void Matrix4x4_VectorTransform( const matrix4x4& in, const float v[3], float out[3] );
void Matrix4x4_VectorITransform( const matrix4x4& in, const float v[3], float out[3] );
void Matrix4x4_VectorRotate( const matrix4x4& in, const float v[3], float out[3] );
void Matrix4x4_VectorIRotate( const matrix4x4& in, const float v[3], float out[3] );
void Matrix4x4_ConcatTransforms( matrix4x4& out, const matrix4x4& in1, const matrix4x4& in2 );
void Matrix4x4_FromOriginQuat( matrix4x4& out, const vec4_t& quaternion, const vec3_t& origin );
void Matrix4x4_CreateFromEntity( matrix4x4& out, const vec3_t& angles, const vec3_t& origin, float scale );
void Matrix4x4_TransformPositivePlane( const matrix4x4& in, const vec3_t& normal, float d, vec3_t& out, float *dist );
void Matrix4x4_TransformStandardPlane( const matrix4x4& in, const vec3_t& normal, float d, vec3_t& out, float *dist );
void Matrix4x4_ConvertToEntity( const matrix4x4& in, vec3_t& angles, vec3_t& origin );
void Matrix4x4_SetOrigin( matrix4x4& out, float x, float y, float z );
void Matrix4x4_Invert_Simple( matrix4x4& out, const matrix4x4& in1 );
void Matrix4x4_OriginFromMatrix( const matrix4x4& in, float *out );
void Matrix4x4_Transpose( matrix4x4& out, const matrix4x4& in1 );
bool Matrix4x4_Invert_Full( matrix4x4& out, const matrix4x4& in1 );

float V_CalcFov( float *fov_x, float width, float height );
void V_AdjustFov( float *fov_x, float *fov_y, float width, float height, bool lock_x );

int BoxOnPlaneSide( const vec3_t& emins, const vec3_t& emaxs, const mplane_t *p );
#define BOX_ON_PLANE_SIDE( emins, emaxs, p )			\
	((( p )->type < 3 ) ?				\
	(						\
		((p)->dist <= (emins)[(p)->type]) ?		\
			1				\
		:					\
		(					\
			((p)->dist >= (emaxs)[(p)->type]) ?	\
				2			\
			:				\
				3			\
		)					\
	)						\
	:						\
		BoxOnPlaneSide(( emins ), ( emaxs ), ( p )))
