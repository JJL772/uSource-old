/**
 * 
 * ishader.h
 * 
 * Basic definitions for an extensible shader framework
 * 
 */ 
#pragma once 

#include "ibuffer.h"
#include "imaterial.h"

struct shader_param_t 
{
	char* name;
};

enum class ESourceCodeType
{
	GLSL = 0,
	HLSL,
	SPIRV,
	FXC,		/* FXC (directx) bytecode */
};

/**
 * This is the base class for all shaders. 
 * Note that this is pretty similar to OpenGL's shader pipeline. APIs that use pre-compiled bytecode, 
 * such as Vulkan and DirectX will basically just stub out Compile().
 * SetSourceCode should still be implemented as the rendersystem will still pass it a buffer containing the
 * bytecode. 
 */ 
class IShader
{
public:

	virtual const char* GetName() const = 0;

	virtual void SetSourceCode(void* pbuf, size_t buflen, ESourceCodeType type = ESourceCodeType::GLSL) = 0;

	virtual bool Compile() = 0;


};