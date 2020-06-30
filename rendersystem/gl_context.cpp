/*
vid_sdl.c - SDL vid component
Copyright (C) 2018 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "gl_local.h"
#include "gl_export.h"

ref_api_t      gEngfuncs;
ref_globals_t *gpGlobals;

static void R_ClearScreen( void )
{
	pglClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	pglClear( GL_COLOR_BUFFER_BIT );
}

static const byte *R_GetTextureOriginalBuffer( unsigned int idx )
{
	gl_texture_t *glt = R_GetTexture( idx );

	if( !glt || !glt->original || !glt->original->buffer )
		return NULL;

	return glt->original->buffer;
}

/*
=============
CL_FillRGBA

=============
*/
static void CL_FillRGBA( float _x, float _y, float _w, float _h, int r, int g, int b, int a )
{
	pglDisable( GL_TEXTURE_2D );
	pglEnable( GL_BLEND );
	pglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	pglBlendFunc( GL_SRC_ALPHA, GL_ONE );
	pglColor4f( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );

	pglBegin( GL_QUADS );
		pglVertex2f( _x, _y );
		pglVertex2f( _x + _w, _y );
		pglVertex2f( _x + _w, _y + _h );
		pglVertex2f( _x, _y + _h );
	pglEnd ();

	pglColor3f( 1.0f, 1.0f, 1.0f );
	pglEnable( GL_TEXTURE_2D );
	pglDisable( GL_BLEND );
}

/*
=============
pfnFillRGBABlend

=============
*/
static void GAME_EXPORT CL_FillRGBABlend( float _x, float _y, float _w, float _h, int r, int g, int b, int a )
{
	pglDisable( GL_TEXTURE_2D );
	pglEnable( GL_BLEND );
	pglTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	pglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	pglColor4f( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f );

	pglBegin( GL_QUADS );
		pglVertex2f( _x, _y );
		pglVertex2f( _x + _w, _y );
		pglVertex2f( _x + _w, _y + _h );
		pglVertex2f( _x, _y + _h );
	pglEnd ();

	pglColor3f( 1.0f, 1.0f, 1.0f );
	pglEnable( GL_TEXTURE_2D );
	pglDisable( GL_BLEND );
}

void Mod_BrushUnloadTextures( model_t *mod )
{
	int i;

	for( i = 0; i < mod->numtextures; i++ )
	{
		texture_t *tx = mod->textures[i];
		if( !tx || tx->gl_texturenum == tr.defaultTexture )
			continue; // free slot

		GL_FreeTexture( tx->gl_texturenum );    // main texture
		GL_FreeTexture( tx->fb_texturenum );    // luma texture
	}
}

void Mod_UnloadTextures( model_t *mod )
{
	Assert( mod != NULL );

	switch( mod->type )
	{
	case mod_studio:
		Mod_StudioUnloadTextures( mod->cache.data );
		break;
	case mod_alias:
		Mod_AliasUnloadTextures( mod->cache.data );
		break;
	case mod_brush:
		Mod_BrushUnloadTextures( mod );
		break;
	case mod_sprite:
		Mod_SpriteUnloadTextures( mod->cache.data );
		break;
	default:
		ASSERT( 0 );
		break;
	}
}

qboolean Mod_ProcessRenderData( model_t *mod, qboolean create, const byte *buf )
{
	qboolean loaded = true;

	if( create )
	{
		switch( mod->type )
		{
			case mod_studio:
				// Mod_LoadStudioModel( mod, buf, loaded );
				break;
			case mod_sprite:
				Mod_LoadSpriteModel( mod, buf, &loaded, mod->numtexinfo );
				break;
			case mod_alias:
				Mod_LoadAliasModel( mod, buf, &loaded );
				break;
			case mod_brush:
				// Mod_LoadBrushModel( mod, buf, loaded );
				break;
			default: gEngfuncs.Host_Error( "Mod_LoadModel: unsupported type %d\n", mod->type );
		}
	}

	if( loaded && gEngfuncs.drawFuncs->Mod_ProcessUserData )
		gEngfuncs.drawFuncs->Mod_ProcessUserData( mod, create, buf );

	if( !create )
		Mod_UnloadTextures( mod );

	return loaded;
}

static int GL_RefGetParm( int parm, int arg )
{
	gl_texture_t *glt;

	switch( parm )
	{
	case PARM_TEX_WIDTH:
		glt = R_GetTexture( arg );
		return glt->width;
	case PARM_TEX_HEIGHT:
		glt = R_GetTexture( arg );
		return glt->height;
	case PARM_TEX_SRC_WIDTH:
		glt = R_GetTexture( arg );
		return glt->srcWidth;
	case PARM_TEX_SRC_HEIGHT:
		glt = R_GetTexture( arg );
		return glt->srcHeight;
	case PARM_TEX_GLFORMAT:
		glt = R_GetTexture( arg );
		return glt->format;
	case PARM_TEX_ENCODE:
		glt = R_GetTexture( arg );
		return glt->encode;
	case PARM_TEX_MIPCOUNT:
		glt = R_GetTexture( arg );
		return glt->numMips;
	case PARM_TEX_DEPTH:
		glt = R_GetTexture( arg );
		return glt->depth;
	case PARM_TEX_SKYBOX:
		Assert( arg >= 0 && arg < 6 );
		return tr.skyboxTextures[arg];
	case PARM_TEX_SKYTEXNUM:
		return tr.skytexturenum;
	case PARM_TEX_LIGHTMAP:
		arg = bound( 0, arg, MAX_LIGHTMAPS - 1 );
		return tr.lightmapTextures[arg];
	case PARM_WIDESCREEN:
		return gpGlobals->wideScreen;
	case PARM_FULLSCREEN:
		return gpGlobals->fullScreen;
	case PARM_SCREEN_WIDTH:
		return gpGlobals->width;
	case PARM_SCREEN_HEIGHT:
		return gpGlobals->height;
	case PARM_TEX_TARGET:
		glt = R_GetTexture( arg );
		return glt->target;
	case PARM_TEX_TEXNUM:
		glt = R_GetTexture( arg );
		return glt->texnum;
	case PARM_TEX_FLAGS:
		glt = R_GetTexture( arg );
		return glt->flags;
	case PARM_ACTIVE_TMU:
		return glState.activeTMU;
	case PARM_LIGHTSTYLEVALUE:
		arg = bound( 0, arg, MAX_LIGHTSTYLES - 1 );
		return tr.lightstylevalue[arg];
	case PARM_MAX_IMAGE_UNITS:
		return GL_MaxTextureUnits();
	case PARM_REBUILD_GAMMA:
		return glConfig.softwareGammaUpdate;
	case PARM_SURF_SAMPLESIZE:
		if( arg >= 0 && arg < WORLDMODEL->numsurfaces )
			return gEngfuncs.Mod_SampleSizeForFace( &WORLDMODEL->surfaces[arg] );
		return LM_SAMPLE_SIZE;
	case PARM_GL_CONTEXT_TYPE:
		return glConfig.context;
	case PARM_GLES_WRAPPER:
		return glConfig.wrapper;
	case PARM_STENCIL_ACTIVE:
		return glState.stencilEnabled;
	case PARM_SKY_SPHERE:
		return ENGINE_GET_PARM_( parm, arg ) && !tr.fCustomSkybox;
	default:
		return ENGINE_GET_PARM_( parm, arg );
	}
	return 0;
}

static void R_GetDetailScaleForTexture( int texture, float *xScale, float *yScale )
{
	gl_texture_t *glt = R_GetTexture( texture );

	if( xScale ) *xScale = glt->xscale;
	if( yScale ) *yScale = glt->yscale;
}

static void R_GetExtraParmsForTexture( int texture, byte *red, byte *green, byte *blue, byte *density )
{
	gl_texture_t *glt = R_GetTexture( texture );

	if( red ) *red = glt->fogParams[0];
	if( green ) *green = glt->fogParams[1];
	if( blue ) *blue = glt->fogParams[2];
	if( density ) *density = glt->fogParams[3];
}


static void R_SetCurrentEntity( cl_entity_t *ent )
{
	RI.currententity = ent;

	// set model also
	if( RI.currententity != NULL )
	{
		RI.currentmodel = RI.currententity->model;
	}
}

static void R_SetCurrentModel( model_t *mod )
{
	RI.currentmodel = mod;
}

static float R_GetFrameTime( void )
{
	return tr.frametime;
}

static const char *GL_TextureName( unsigned int texnum )
{
	return R_GetTexture( texnum )->name;
}

const byte *GL_TextureData( unsigned int texnum )
{
	rgbdata_t *pic = R_GetTexture( texnum )->original;

	if( pic != NULL )
		return pic->buffer;
	return NULL;
}

void R_ProcessEntData( qboolean allocate )
{
	if( !allocate )
	{
		tr.draw_list->num_solid_entities = 0;
		tr.draw_list->num_trans_entities = 0;
		tr.draw_list->num_beam_entities = 0;
	}

	if( gEngfuncs.drawFuncs->R_ProcessEntData )
		gEngfuncs.drawFuncs->R_ProcessEntData( allocate );
}

static const char *R_GetConfigName( void )
{
	return "opengl";
}

ref_interface_t gReffuncs =
{
	R_Init,
	R_Shutdown,
	R_GetConfigName,

	GL_SetupAttributes,
	GL_InitExtensions,
	GL_ClearExtensions,

	R_BeginFrame,
	R_RenderScene,
	R_EndFrame,
	R_PushScene,
	R_PopScene,
	GL_BackendStartFrame,
	GL_BackendEndFrame,

	R_ClearScreen,
	R_AllowFog,
	GL_SetRenderMode,

	R_AddEntity,
	CL_AddCustomBeam,
	R_ProcessEntData,

	R_ShowTextures,

	R_GetTextureOriginalBuffer,
	GL_LoadTextureFromBuffer,
	GL_ProcessTexture,
	R_SetupSky,

	R_Set2DMode,
	R_DrawStretchRaw,
	R_DrawStretchPic,
	R_DrawTileClear,
	CL_FillRGBA,
	CL_FillRGBABlend,

	VID_ScreenShot,
	VID_CubemapShot,

	R_LightPoint,

	R_DecalShoot,
	R_DecalRemoveAll,
	R_CreateDecalList,
	R_ClearAllDecals,

	R_StudioEstimateFrame,
	R_StudioLerpMovement,
	CL_InitStudioAPI,

	R_InitSkyClouds,
	GL_SubdivideSurface,
	CL_RunLightStyles,

	R_GetSpriteParms,
	R_GetSpriteTexture,

	Mod_LoadMapSprite,
	Mod_ProcessRenderData,
	Mod_StudioLoadTextures,

	CL_DrawParticles,
	CL_DrawTracers,
	CL_DrawBeams,
	R_BeamCull,

	GL_RefGetParm,
	R_GetDetailScaleForTexture,
	R_GetExtraParmsForTexture,
	R_GetFrameTime,

	R_SetCurrentEntity,
	R_SetCurrentModel,

	GL_FindTexture,
	GL_TextureName,
	GL_TextureData,
	GL_LoadTexture,
	GL_CreateTexture,
	GL_LoadTextureArray,
	GL_CreateTextureArray,
	GL_FreeTexture,

	DrawSingleDecal,
	R_DecalSetupVerts,
	R_EntityRemoveDecals,

	R_UploadStretchRaw,

	GL_Bind,
	GL_SelectTexture,
	GL_LoadTexMatrixExt,
	GL_LoadIdentityTexMatrix,
	GL_CleanUpTextureUnits,
	GL_TexGen,
	GL_TextureTarget,
	GL_SetTexCoordArrayMode,
	GL_UpdateTexSize,
	NULL,
	NULL,

	CL_DrawParticlesExternal,
	R_LightVec,
	R_StudioGetTexture,

	R_RenderFrame,
	Mod_SetOrthoBounds,
	R_SpeedsMessage,
	Mod_GetCurrentVis,
	R_NewMap,
	R_ClearScene,

	TriRenderMode,
	TriBegin,
	TriEnd,
	_TriColor4f,
	_TriColor4ub,
	TriTexCoord2f,
	TriVertex3fv,
	TriVertex3f,
	TriWorldToScreen,
	TriFog,
	R_ScreenToWorld,
	TriGetMatrix,
	TriFogParams,
	TriCullFace,

	VGUI_DrawInit,
	VGUI_DrawShutdown,
	VGUI_SetupDrawingText,
	VGUI_SetupDrawingRect,
	VGUI_SetupDrawingImage,
	VGUI_BindTexture,
	VGUI_EnableTexture,
	VGUI_CreateTexture,
	VGUI_UploadTexture,
	VGUI_UploadTextureBlock,
	VGUI_DrawQuad,
	VGUI_GetTextureSizes,
	VGUI_GenerateTexture,
};

extern "C" int EXPORT GetRefAPI( int version, ref_interface_t *funcs, ref_api_t *engfuncs, ref_globals_t *globals )
{
	if( version != REF_API_VERSION )
		return 0;

	// fill in our callbacks
	memcpy( funcs, &gReffuncs, sizeof( ref_interface_t ));
	memcpy( &gEngfuncs, engfuncs, sizeof( ref_api_t ));
	gpGlobals = globals;

	return REF_API_VERSION;
}

extern "C" void EXPORT GetRefHumanReadableName( char *out, size_t size )
{
#if defined XASH_NANOGL
	Q_strncpy( out, "GLES1(NanoGL)", size );
#elif defined XASH_WES
	Q_strncpy( out, "GLES2(gl-wes-v2)", size );
#else
	Q_strncpy( out, "OpenGL", size );
#endif
}


class CGLRenderBackend : public IRenderBackend 
{
public:
	virtual bool Init() { return true; }
	virtual bool PreInit() { return true; }
	virtual void Shutdown() {}
	virtual const char* GetName() { return "CGLRenderBackend001"; };
	virtual const char* GetParentInterface() { return IREFINT_INTERFACE; };

	virtual qboolean 		R_Init( void ) 
	{
		return ::R_Init();
	}

	virtual void 			R_Shutdown( void ) 
	{
		::R_Shutdown();
	}

	virtual const char *		R_GetConfigName( void ) 
	{
		return ::R_GetConfigName();
	}

	virtual void 			GL_SetupAttributes( int safegl ) 
	{
		::GL_SetupAttributes(safegl);
	}

	virtual void 			GL_InitExtensions( void ) 
	{
		::GL_InitExtensions();
	}

	virtual void 			GL_ClearExtensions( void ) 
	{
		::GL_ClearExtensions();
	}

	virtual void 			R_BeginFrame( qboolean clearScene ) 
	{
		::R_BeginFrame(clearScene);
	}

	virtual void 			R_RenderScene( void ) 
	{
		::R_RenderScene();
	}

	virtual void 			R_EndFrame( void ) 
	{
		::R_EndFrame();
	}

	virtual void 			R_PushScene( void ) 
	{
		::R_PushScene();
	}

	virtual void 			R_PopScene( void ) 
	{
		::R_PopScene();
	}

	virtual void 			GL_BackendStartFrame( void ) 
	{
		::GL_BackendStartFrame();
	}

	virtual void 			GL_BackendEndFrame( void ) 
	{
		::GL_BackendEndFrame();
	}

	virtual void 			R_ClearScreen( void ) 
	{
		::R_ClearScreen();
	}

	virtual void 			R_AllowFog( qboolean allow ) 
	{
		::R_AllowFog(allow);
	}

	virtual void 			GL_SetRenderMode( int renderMode ) 
	{
		::GL_SetRenderMode(renderMode);
	}

	virtual qboolean 		R_AddEntity( struct cl_entity_s *clent, int type ) 
	{
		return ::R_AddEntity(clent, type);
	}

	virtual void 			CL_AddCustomBeam( cl_entity_t *pEnvBeam ) 
	{
		::CL_AddCustomBeam(pEnvBeam);
	}

	virtual void 			R_ProcessEntData( qboolean allocate ) 
	{
		::R_ProcessEntData(allocate);
	}

	virtual void 			R_ShowTextures( void ) 
	{
		::R_ShowTextures();
	}

	virtual const byte *		R_GetTextureOriginalBuffer( unsigned int idx ) 
	{
		return ::R_GetTextureOriginalBuffer(idx);
	}

	virtual int 			GL_LoadTextureFromBuffer( const char *name, rgbdata_t *pic, texFlags_t flags, qboolean update ) 
	{
		return ::GL_LoadTextureFromBuffer(name, pic, flags, update);
	}

	virtual void 			GL_ProcessTexture( int texnum, float gamma, int topColor, int bottomColor ) 
	{
		return ::GL_ProcessTexture(texnum, gamma, topColor, bottomColor);
	}

	virtual void 			R_SetupSky( const char *skyname ) 
	{
		::R_SetupSky(skyname);
	}

	virtual void 			R_Set2DMode( qboolean enable ) 
	{
		::R_Set2DMode(enable);
	}

	virtual void 			R_DrawStretchRaw( float x, float y, float w, float h, int cols, int rows, const byte *data, qboolean dirty ) 
	{
		::R_DrawStretchRaw(x, y, w, h, cols, rows, data, dirty);
	}

	virtual void 			R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, int texnum ) 
	{
		::R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, texnum);
	}

	virtual void 			R_DrawTileClear( int texnum, int x, int y, int w, int h ) 
	{
		::R_DrawTileClear(texnum, x, y, w, h);
	}

	virtual void 			FillRGBA( float x, float y, float w, float h, int r, int g, int b, int a ) 
	{
		::CL_FillRGBA(x, y, w, h, r, g, b, a);
	}

	virtual void 			FillRGBABlend( float x, float y, float w, float h, int r, int g, int b, int a ) 
	{
		::CL_FillRGBABlend(x, y, w, h, r, g, b, a);
	}

	virtual qboolean 		VID_ScreenShot( const char *filename, int shot_type ) 
	{
		return ::VID_ScreenShot(filename, shot_type);
	}

	virtual qboolean 		VID_CubemapShot( const char *base, uint size, const float *vieworg, qboolean skyshot ) 
	{
		return ::VID_CubemapShot(base, size, vieworg, skyshot);
	}

	virtual colorVec 		R_LightPoint( const float *p ) 
	{
		return ::R_LightPoint(p);
	}

	virtual void 			R_DecalShoot( int textureIndex, int entityIndex, int modelIndex, vec3_t pos, int flags, float scale ) 
	{
		::R_DecalShoot(textureIndex, entityIndex, modelIndex, pos, flags, scale);
	}

	virtual void 			R_DecalRemoveAll( int texture ) 
	{
		::R_DecalRemoveAll(texture);
	}

	virtual int 			R_CreateDecalList( struct decallist_s *pList ) 
	{
		return ::R_CreateDecalList(pList);
	}

	virtual void 			R_ClearAllDecals( void ) 
	{
		::R_ClearAllDecals();
	}

	virtual float 			R_StudioEstimateFrame( cl_entity_t *e, mstudioseqdesc_t *pseqdesc ) 
	{
		return ::R_StudioEstimateFrame(e, pseqdesc);
	}

	virtual void 			R_StudioLerpMovement( cl_entity_t *e, double time, vec3_t origin, vec3_t angles ) 
	{
		::R_StudioLerpMovement(e, time, origin, angles);
	}

	virtual void 			CL_InitStudioAPI( void ) 
	{
		::CL_InitStudioAPI();
	}

	virtual void 			R_InitSkyClouds( struct mip_s *mt, struct texture_s *tx, qboolean custom_palette ) 
	{
		::R_InitSkyClouds(mt, tx, custom_palette);
	}

	virtual void 			GL_SubdivideSurface( msurface_t *fa ) 
	{
		::GL_SubdivideSurface(fa);
	}

	virtual void 			CL_RunLightStyles( void ) 
	{
		::CL_RunLightStyles();
	}

	virtual void 			R_GetSpriteParms( int *frameWidth, int *frameHeight, int *numFrames, int currentFrame, const model_t *pSprite ) 
	{
		::R_GetSpriteParms(frameWidth, frameHeight, numFrames, currentFrame, pSprite);
	}

	virtual int 			R_GetSpriteTexture( const model_t *m_pSpriteModel, int frame ) 
	{
		return ::R_GetSpriteTexture(m_pSpriteModel, frame);
	}

	virtual void 			Mod_LoadMapSprite( struct model_s *mod, const void *buffer, size_t size, qboolean *loaded ) 
	{
		::Mod_LoadMapSprite(mod, buffer, size, loaded);
	}

	virtual qboolean 		Mod_ProcessRenderData( model_t *mod, qboolean create, const byte *buffer ) 
	{
		return ::Mod_ProcessRenderData(mod, create, buffer);
	}

	virtual void 			Mod_StudioLoadTextures( model_t *mod, void *data ) 
	{
		::Mod_StudioLoadTextures(mod, data);
	}

	virtual void 			CL_DrawParticles( double frametime, particle_t *particles, float partsize ) 
	{
		::CL_DrawParticles(frametime, particles, partsize);
	}

	virtual void 			CL_DrawTracers( double frametime, particle_t *tracers ) 
	{
		::CL_DrawTracers(frametime, tracers);
	}

	virtual void 			CL_DrawBeams( int fTrans , BEAM *beams ) 
	{
		::CL_DrawBeams(fTrans, beams);
	}

	virtual qboolean 		R_BeamCull( const vec3_t start, const vec3_t end, qboolean pvsOnly ) 
	{
		return ::R_BeamCull(start, end, pvsOnly);
	}

	virtual int			RefGetParm( int parm, int arg ) 
	{
		return ::GL_RefGetParm(parm, arg);
	}

	virtual void			GetDetailScaleForTexture( int texture, float *xScale, float *yScale ) 
	{
		::R_GetDetailScaleForTexture(texture, xScale, yScale);
	}

	virtual void			GetExtraParmsForTexture( int texture, byte *red, byte *green, byte *blue, byte *alpha ) 
	{
		::R_GetExtraParmsForTexture(texture, red, green, blue, alpha);
	}

	virtual float			GetFrameTime( void ) 
	{
		return ::R_GetFrameTime();
	}

	virtual void			R_SetCurrentEntity( struct cl_entity_s *ent ) 
	{
		::R_SetCurrentEntity(ent);
	}

	virtual void			R_SetCurrentModel( struct model_s *mod ) 
	{
		::R_SetCurrentModel(mod);
	}

	virtual int			GL_FindTexture( const char *name ) 
	{
		return ::GL_FindTexture(name);
	}

	virtual const char*		GL_TextureName( unsigned int texnum ) 
	{
		return ::GL_TextureName(texnum);
	}

	virtual const byte*		GL_TextureData( unsigned int texnum ) 
	{
		return ::GL_TextureData(texnum);
	}

	virtual int			GL_LoadTexture( const char *name, const byte *buf, size_t size, int flags ) 
	{
		return ::GL_LoadTexture(name, buf, size, flags);
	}

	virtual int			GL_CreateTexture( const char *name, int width, int height, const void *buffer, texFlags_t flags ) 
	{
		return ::GL_CreateTexture(name, width, height, buffer, flags);
	}

	virtual int			GL_LoadTextureArray( const char **names, int flags ) 
	{
		return ::GL_LoadTextureArray(names, flags);
	}

	virtual int			GL_CreateTextureArray( const char *name, int width, int height, int depth, const void *buffer, texFlags_t flags ) 
	{
		return ::GL_CreateTextureArray(name, width, height, depth, buffer, flags);
	}

	virtual void			GL_FreeTexture( unsigned int texnum ) 
	{
		return ::GL_FreeTexture(texnum);
	}

	virtual void			DrawSingleDecal( struct decal_s *pDecal, struct msurface_s *fa ) 
	{
		::DrawSingleDecal(pDecal, fa);
	}

	virtual float*			R_DecalSetupVerts( struct decal_s *pDecal, struct msurface_s *surf, int texture, int *outCount ) 
	{
		return ::R_DecalSetupVerts(pDecal, surf, texture, outCount);
	}

	virtual void			R_EntityRemoveDecals( struct model_s *mod ) 
	{
		::R_EntityRemoveDecals(mod);
	}

	virtual void			AVI_UploadRawFrame( int texture, int cols, int rows, int width, int height, const byte *data ) 
	{
		::R_UploadStretchRaw(texture, cols, rows, width, height, data);
	}

	virtual void			GL_Bind( int tmu, unsigned int texnum ) 
	{
		::GL_Bind(tmu, texnum);
	}

	virtual void			GL_SelectTexture( int tmu ) 
	{
		::GL_SelectTexture(tmu);
	}

	virtual void			GL_LoadTextureMatrix( const float *glmatrix ) 
	{
		::GL_LoadTexMatrix((matrix4x4)glmatrix);
	}

	virtual void			GL_TexMatrixIdentity( void ) 
	{
		::GL_LoadIdentityTexMatrix();
	}

	virtual void			GL_CleanUpTextureUnits( int last ) 
	{
		::GL_CleanUpTextureUnits(last);
	}

	virtual void			GL_TexGen( unsigned int coord, unsigned int mode ) 
	{
		::GL_TexGen(coord, mode);
	}

	virtual void			GL_TextureTarget( unsigned int target ) 
	{
		::GL_TextureTarget(target);
	}

	virtual void			GL_TexCoordArrayMode( unsigned int texmode ) 
	{
		::GL_SetTexCoordArrayMode(texmode);
	}

	virtual void			GL_UpdateTexSize( int texnum, int width, int height, int depth ) 
	{
		::GL_UpdateTexSize(texnum, width, height, depth);
	}

	virtual void			GL_Reserved0( void ) 
	{

	}

	virtual void			GL_Reserved1( void ) 
	{

	}

	virtual void			GL_DrawParticles( const struct ref_viewpass_s *rvp, qboolean trans_pass, float frametime ) 
	{
		::CL_DrawParticlesExternal(rvp, trans_pass, frametime);
	}

	virtual colorVec		LightVec( const float *start, const float *end, float *lightspot, float *lightvec ) 
	{
		return ::R_LightVec(start, end, lightspot, lightvec);
	}

	virtual struct mstudiotex_s *	StudioGetTexture ( struct cl_entity_s *e ) 
	{
		return ::R_StudioGetTexture(e);
	}

	virtual void			GL_RenderFrame( const struct ref_viewpass_s *rvp ) 
	{
		::R_RenderFrame(rvp);
	}

	virtual void			GL_OrthoBounds( const float *mins, const float *maxs ) 
	{
		::Mod_SetOrthoBounds(mins, maxs);
	}

	virtual qboolean		R_SpeedsMessage( char *out, size_t size ) 
	{
		return ::R_SpeedsMessage(out, size);
	}

	virtual byte*			Mod_GetCurrentVis( void ) 
	{
		return ::Mod_GetCurrentVis();
	}

	virtual void			R_NewMap( void ) 
	{
		::R_NewMap();
	}

	virtual void			R_ClearScene( void ) 
	{
		::R_ClearScene();
	}

	virtual void			TriRenderMode( int mode ) 
	{
		::TriRenderMode(mode);
	}

	virtual void			Begin( int primitiveCode ) 
	{
		::TriBegin(primitiveCode);
	}

	virtual void			End( void ) 
	{
		::TriEnd();
	}

	virtual void			Color4f( float r, float g, float b, float a ) 
	{
		::TriColor4f(r, g, b, a);
	}

	virtual void			Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a ) 
	{
		::TriColor4ub(r, g, b, a);
	}

	virtual void			TexCoord2f( float u, float v ) 
	{
		::TriTexCoord2f(u, v);
	}

	virtual void			Vertex3fv( const float *worldPnt ) 
	{
		::TriVertex3fv(worldPnt);
	}

	virtual void			Vertex3f( float x, float y, float z ) 
	{
		::TriVertex3f(x, y, z);
	}

	virtual int			WorldToScreen( const float *world, float *screen )
	{
		return ::R_WorldToScreen(world, screen);
	}

	virtual void			Fog( float flFogColor[3], float flStart, float flEnd, int bOn ) 
	{
		::TriFog(flFogColor, flStart, flEnd, bOn);
	}

	virtual void			ScreenToWorld( const float *screen, float *world  ) 
	{
		::R_ScreenToWorld(screen, world);
	}

	virtual void			GetMatrix( const int pname, float *matrix ) 
	{
		::TriGetMatrix(pname, matrix);
	}

	virtual void			FogParams( float flDensity, int iFogSkybox ) 
	{
		::TriFogParams(flDensity, iFogSkybox);
	}

	virtual void    		CullFace( TRICULLSTYLE mode ) 
	{
		::TriCullFace(mode);
	}

	virtual void			VGUI_DrawInit( void ) 
	{
		::VGUI_DrawInit();
	}

	virtual void			VGUI_DrawShutdown( void ) 
	{
		::VGUI_DrawShutdown();
	}

	virtual void			VGUI_SetupDrawingText( int *pColor ) 
	{
		::VGUI_SetupDrawingText(pColor);
	}

	virtual void			VGUI_SetupDrawingRect( int *pColor ) 
	{
		::VGUI_SetupDrawingRect(pColor);
	}

	virtual void			VGUI_SetupDrawingImage( int *pColor ) 
	{
		::VGUI_SetupDrawingImage(pColor);
	}

	virtual void			VGUI_BindTexture( int id ) 
	{
		::VGUI_BindTexture(id);
	}

	virtual void			VGUI_EnableTexture( qboolean enable ) 
	{
		::VGUI_EnableTexture(enable);
	}

	virtual void			VGUI_CreateTexture( int id, int width, int height ) 
	{
		::VGUI_CreateTexture(id, width, height);
	}

	virtual void			VGUI_UploadTexture( int id, const char *buffer, int width, int height ) 
	{
		::VGUI_UploadTexture(id, buffer, width, height);
	}

	virtual void			VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight ) 
	{
		::VGUI_UploadTextureBlock(id, drawX, drawY, rgba, blockWidth, blockHeight);
	}

	virtual void			VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr ) 
	{
		::VGUI_DrawQuad(ul, lr);
	}

	virtual void			VGUI_GetTextureSizes( int *width, int *height ) 
	{
		::VGUI_GetTextureSizes(width, height);
	}

	virtual int			VGUI_GenerateTexture( void ) 
	{
		return ::VGUI_GenerateTexture();
	}

};

EXPOSE_INTERFACE(CGLRenderBackend);