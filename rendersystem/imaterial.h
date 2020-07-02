#pragma once

#include "mathlib/mathlib.h"

enum class ETextureFormat
{
	RGB888 = 0,
	RGB161616,
	RGBA8888,
	RGBA16161616,
	GRAY16, 	/* 16 bits per pixel grayscale */
	GRAY8,		/* 8 bits per pixel grayscale */
};

class ITexture
{
public:
	virtual ETextureFormat GetFormat() const = 0;
	
	virtual ITexture* Convert(ETextureFormat fmt) = 0;

	virtual void ConvertInPlace(ETextureFormat fmt) = 0;

	virtual void* GetData() = 0;

	virtual size_t GetSize() const = 0;
};

enum class EMaterialParamType
{
	FLOAT = 0,
	INT,
	VEC3,
	VEC2,
	VEC4,
	TEXTURE,
};

struct material_param_t
{
	char* key;
	EMaterialParamType type;
	union {
		float _float;
		int _int;
		vec3_t _vec3;
		vec2_t _vec2;
		vec4_t _vec4;
		ITexture* _texval;
	} value;
};

class IMaterial
{
public:
	virtual const char* GetName() const = 0;

	virtual int AddTexture(ITexture* tex) = 0;

	virtual void AddParam(const material_param_t& param) = 0;

	virtual material_param_t& GetParam(const char* name) const = 0;

	virtual void RemoveTexture(ITexture* tex) = 0;

	virtual void RemoveTexture(int texindex) = 0;

	virtual IMaterial* Copy() const = 0;
};

