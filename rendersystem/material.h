#pragma once

#include "mathlib/mathlib.h"
#include "public/containers/list.h"
#include "imaterial.h"
#include "ibuffer.h"

/* Simple texture class */
/* Basically an array of pixels in some format */
class CTexture : ITexture
{
public:
	void* buffer;
	size_t size;
	ETextureFormat format;
};

/* Buffer formats for the texture class */
static CBufferFormat g_TexFormat_RGBA8888 = { {1}, {1}, {1}, {1} };
static CBufferFormat g_TexFormat_RGBA16161616 = { {2}, {2}, {2}, {2} };
static CBufferFormat g_TexFormat_RGB8888 = { {1}, {1}, {1} };
static CBufferFormat g_TexFormat_RGB161616 = { {2}, {2}, {2} };
static CBufferFormat g_TexFormat_Gray8 = { {1} };
static CBufferFormat g_TexFormat_Gray16 = { {2} };

class CMaterial : public IMaterial
{
public:
	List<material_param_t> params;
};

