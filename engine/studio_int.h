#pragma once

#include "r_studioint.h"
#include "appframework.h"

#define ICLIENTSTUDIO_001 "IClientStudio001"
#define ICLIENTSTUDIO_INTERFACE ICLIENTSTUDIO_001

class IClientStudio : public IAppInterface
{
public:
	virtual int		StudioDrawModel( int flags ) = 0;
	virtual int		StudioDrawPlayer( int flags, struct entity_state_s *pplayer ) = 0;
};

#define IENGINESTUDIO_001 "IEngineStudio001"
#define IENGINESTUDIO_INTERFACE IENGINESTUDIO_001

class IEngineStudio : public IAppInterface
{
public:
	virtual void		*Mem_Calloc ( int number, size_t size ) = 0;
	virtual void		*Cache_Check ( struct cache_user_s *c ) = 0;
	virtual void		LoadCacheFile ( const char *path, struct cache_user_s *cu ) = 0;
	virtual struct model_s	*Mod_ForName ( const char *name, int crash_if_missing ) = 0;
	virtual void		*Mod_Extradata ( struct model_s *mod ) = 0;
	virtual struct model_s	*GetModelByIndex ( int index ) = 0;
	virtual struct cl_entity_s * GetCurrentEntity ( void ) = 0;
	virtual struct player_info_s *PlayerInfo ( int index ) = 0;
	virtual struct entity_state_s *GetPlayerState ( int index ) = 0;
	virtual struct cl_entity_s * GetViewEntity ( void ) = 0;
	virtual void		GetTimes ( int *framecount, double *current, double *old ) = 0;
	virtual struct cvar_s	*GetCvar ( const char *name ) = 0;
	virtual void		GetViewInfo ( float *origin, float *upv, float *rightv, float *vpnv ) = 0;
	virtual struct model_s	*GetChromeSprite ( void ) = 0;
	virtual void		GetModelCounters ( int **s, int **a ) = 0;
	virtual void		GetAliasScale ( float *x, float *y ) = 0;
	virtual float		****StudioGetBoneTransform ( void ) = 0;
	virtual float		****StudioGetLightTransform ( void ) = 0;
	virtual float		***StudioGetAliasTransform ( void ) = 0;
	virtual float		***StudioGetRotationMatrix ( void ) = 0;
	virtual void		StudioSetupModel ( int bodypart, void **ppbodypart, void **ppsubmodel ) = 0;
	virtual int		StudioCheckBBox ( void ) = 0;
	virtual void		StudioDynamicLight ( struct cl_entity_s *ent, struct alight_s *plight ) = 0;
	virtual void		StudioEntityLight ( struct alight_s *plight ) = 0;
	virtual void		StudioSetupLighting ( struct alight_s *plighting ) = 0;
	virtual void		StudioDrawPoints ( void ) = 0;
	virtual void		StudioDrawHulls ( void ) = 0;
	virtual void		StudioDrawAbsBBox ( void ) = 0;
	virtual void		StudioDrawBones ( void ) = 0;
	virtual void		StudioSetupSkin ( void *ptexturehdr, int index ) = 0;
	virtual void		StudioSetRemapColors ( int top, int bottom ) = 0;
	virtual struct model_s	*SetupPlayerModel ( int index ) = 0;
	virtual void		StudioClientEvents ( void ) = 0;
	virtual int		GetForceFaceFlags ( void ) = 0;
	virtual void		SetForceFaceFlags ( int flags ) = 0;
	virtual void		StudioSetHeader ( void *header ) = 0;
	virtual void		SetRenderModel ( struct model_s *model ) = 0;
	virtual void		SetupRenderer ( int rendermode ) = 0;
	virtual void		RestoreRenderer ( void ) = 0;
	virtual void		SetChromeOrigin ( void ) = 0;
	virtual int		IsHardware ( void ) = 0;
	virtual void		GL_StudioDrawShadow ( void ) = 0;
	virtual void		GL_SetRenderMode ( int mode ) = 0;
	virtual void		StudioSetRenderamt ( int iRenderamt ) = 0;
	virtual void		StudioSetCullState ( int iCull ) = 0;
	virtual void		StudioRenderShadow ( int iSprite, float *p1, float *p2, float *p3, float *p4 ) = 0;
};