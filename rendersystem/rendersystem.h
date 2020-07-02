/**
 * 
 * rendersystem.h
 * 
 * Rendersystem is an advanced rendering backend for xash3d. It's based off of xash3d's original ref_gl renderer, but with some pretty significant changes.
 * 
 * The rendersystem abstracts graphics API calls in such a way that you can write platform agnostic rendering code
 * quite easily. The goal of this is primarily to ease shader creation and make extending the graphics system much much easier. 
 * 
 */
#pragma once

#include "imesh.h"
#include "ishader.h"
#include "ibuffer.h"
#include "imaterial.h"

#include "gl_local.h"

struct render_params_t
{
	int width;
	int height;
	int bpp;
	int refresh_rate;
};

/* You must implement this in your own backend */
class IRenderSystem
{
public:
	virtual void Init(const render_params_t& params) = 0;
	virtual void Shutdown() = 0;

	virtual void OnParamsChanged(const render_params_t& params) = 0;

	virtual IStaticMesh* CreateEmptyStaticMesh() = 0;
	virtual IVertexBuffer* CreateEmptyVertexBuffer(EVertexFormat fmt, EBufferType type) = 0;
	virtual IIndexBuffer* CreateEmptyIndexBuffer(EIndexFormat fmt, EBufferType type) = 0;
	virtual IBuffer* CreateEmptyBuffer(EBufferType type) = 0;
	virtual ITexture* CreateEmptyTexture(ETextureFormat fmt) = 0;

	/* Creates a new image from the rgb data that has been specified */
	/* This won't be using the same buffers as the rgbdata, so you will want to call FreeImage on the rgbdata after this */
	virtual ITexture* CreateTextureFromImage(const rgbdata_t* img) = 0;

	/* Creates a new render target which will reside in shared memory */
	virtual ITexture* CreateRenderTarget(ETextureFormat fmt) = 0;

	/* Creates an empty material */
	virtual IMaterial* CreateEmptyMaterial() = 0;
};

/* This is the only function you must implement within your backend implementation */
/* Internally you'll need to implement the Ixxxx classes, but those implementations should not be exposed */
IRenderSystem* CreateRenderSystem(); 