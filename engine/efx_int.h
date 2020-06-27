#pragma once

#include "r_efx.h"
#include "appframework.h"

#define IEFX_001 "IEngineEFX001"
#define IEFX_INTERFACE IEFX_001

class IEngineEfx : public IAppInterface 
{
public:

	virtual particle_t	*AllocParticle( void (*callback)( struct particle_s *particle, float frametime )) = 0;
	virtual void		BlobExplosion( const float *org ) = 0;
	virtual void		Blood( const float *org, const float *dir, int pcolor, int speed ) = 0;
	virtual void		BloodSprite( const float *org, int colorindex, int modelIndex, int modelIndex2, float size ) = 0;
	virtual void		BloodStream( const float *org, const float *dir, int pcolor, int speed ) = 0;
	virtual void		BreakModel( const float *pos, const float *size, const float *dir, float random, float life, int count, int modelIndex, char flags ) = 0;
	virtual void		Bubbles( const float *mins, const float *maxs, float height, int modelIndex, int count, float speed ) = 0;
	virtual void		BubbleTrail( const float *start, const float *end, float height, int modelIndex, int count, float speed ) = 0;
	virtual void		BulletImpactParticles( const float *pos ) = 0;
	virtual void		EntityParticles( struct cl_entity_s *ent ) = 0;
	virtual void		Explosion( float *pos, int model, float scale, float framerate, int flags ) = 0;
	virtual void		FizzEffect( struct cl_entity_s *pent, int modelIndex, int density ) = 0;
	virtual void		FireField( float *org, int radius, int modelIndex, int count, int flags, float life ) = 0;
	virtual void		FlickerParticles( const float *org ) = 0;
	virtual void		FunnelSprite( const float *org, int modelIndex, int reverse ) = 0;
	virtual void		Implosion( const float *end, float radius, int count, float life ) = 0;
	virtual void		LargeFunnel( const float *org, int reverse ) = 0;
	virtual void		LavaSplash( const float *org ) = 0;
	virtual void		MultiGunshot( const float *org, const float *dir, const float *noise, int count, int decalCount, int *decalIndices ) = 0;
	virtual void		MuzzleFlash( const float *pos1, int type ) = 0;
	virtual void		ParticleBox( const float *mins, const float *maxs, unsigned char r, unsigned char g, unsigned char b, float life ) = 0;
	virtual void		ParticleBurst( const float *pos, int size, int color, float life ) = 0;
	virtual void		ParticleExplosion( const float *org ) = 0;
	virtual void		ParticleExplosion2( const float *org, int colorStart, int colorLength ) = 0;
	virtual void		ParticleLine( const float *start, const float *end, unsigned char r, unsigned char g, unsigned char b, float life ) = 0;
	virtual void		PlayerSprites( int client, int modelIndex, int count, int size ) = 0;
	virtual void		Projectile( const float *origin, const float *velocity, int modelIndex, int life, int owner, void (*hitcallback)( struct tempent_s *ent, struct pmtrace_s *ptr ) ) = 0;
	virtual void		RicochetSound( const float *pos ) = 0;
	virtual void		RicochetSprite( const float *pos, struct model_s *pmodel, float duration, float scale ) = 0;
	virtual void		RocketFlare( const float *pos ) = 0;
	virtual void		RocketTrail( float *start, float *end, int type ) = 0;
	virtual void		RunParticleEffect( const float *org, const float *dir, int color, int count ) = 0;
	virtual void		ShowLine( const float *start, const float *end ) = 0;
	virtual void		SparkEffect( const float *pos, int count, int velocityMin, int velocityMax ) = 0;
	virtual void		SparkShower( const float *pos ) = 0;
	virtual void		SparkStreaks( const float *pos, int count, int velocityMin, int velocityMax ) = 0;
	virtual void		Spray( const float *pos, const float *dir, int modelIndex, int count, int speed, int spread, int rendermode ) = 0;
	virtual void		Sprite_Explode( TEMPENTITY *pTemp, float scale, int flags ) = 0;
	virtual void		Sprite_Smoke( TEMPENTITY *pTemp, float scale ) = 0;
	virtual void		Sprite_Spray( const float *pos, const float *dir, int modelIndex, int count, int speed, int iRand ) = 0;
	virtual void		Sprite_Trail( int type, float *start, float *end, int modelIndex, int count, float life, float size, float amplitude, int renderamt, float speed ) = 0;
	virtual void		Sprite_WallPuff( TEMPENTITY *pTemp, float scale ) = 0;
	virtual void		StreakSplash( const float *pos, const float *dir, int color, int count, float speed, int velocityMin, int velocityMax ) = 0;
	virtual void		TracerEffect( const float *start, const float *end ) = 0;
	virtual void		UserTracerParticle( float *org, float *vel, float life, int colorIndex, float length, unsigned char deathcontext, void (*deathfunc)( struct particle_s *particle )) = 0;
	virtual particle_t	*TracerParticles( float *org, float *vel, float life ) = 0;
	virtual void		TeleportSplash( const float *org ) = 0;
	virtual void		TempSphereModel( const float *pos, float speed, float life, int count, int modelIndex ) = 0;
	virtual TEMPENTITY	*TempModel( const float *pos, const float *dir, const float *angles, float life, int modelIndex, int soundtype ) = 0;
	virtual TEMPENTITY	*DefaultSprite( const float *pos, int spriteIndex, float framerate ) = 0;
	virtual TEMPENTITY	*TempSprite( float *pos, const float *dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags ) = 0;
	virtual int		Draw_DecalIndex( int id ) = 0;
	virtual int		Draw_DecalIndexFromName( const char *name ) = 0;
	virtual void		DecalShoot( int textureIndex, int entity, int modelIndex, float *position, int flags ) = 0;
	virtual void		AttachTentToPlayer( int client, int modelIndex, float zoffset, float life ) = 0;
	virtual void		KillAttachedTents( int client ) = 0;
	virtual BEAM		*BeamCirclePoints( int type, float *start, float *end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual BEAM		*BeamEntPoint( int startEnt, float *end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual BEAM		*BeamEnts( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual BEAM		*BeamFollow( int startEnt, int modelIndex, float life, float width, float r, float g, float b, float brightness ) = 0;
	virtual void		BeamKill( int deadEntity ) = 0;
	virtual BEAM		*BeamLightning( float *start, float *end, int modelIndex, float life, float width, float amplitude, float brightness, float speed ) = 0;
	virtual BEAM		*BeamPoints( float *start, float *end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual BEAM		*BeamRing( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b ) = 0;
	virtual dlight_t	*CL_AllocDlight( int key ) = 0;
	virtual dlight_t	*CL_AllocElight( int key ) = 0;
	virtual TEMPENTITY	*CL_TempEntAlloc( const float *org, struct model_s *model ) = 0;
	virtual TEMPENTITY	*CL_TempEntAllocNoModel( const float *org ) = 0;
	virtual TEMPENTITY	*CL_TempEntAllocHigh( const float *org, struct model_s *model ) = 0;
	virtual TEMPENTITY	*CL_TentEntAllocCustom( const float *origin, struct model_s *model, int high, void (*callback)( struct tempent_s *ent, float frametime, float currenttime )) = 0;
	virtual void		GetPackedColor( short *packed, short color ) = 0;
	virtual short		LookupColor( unsigned char r, unsigned char g, unsigned char b ) = 0;
	virtual void		DecalRemoveAll( int textureIndex ) = 0;
	virtual void		FireCustomDecal( int textureIndex, int entity, int modelIndex, float *position, int flags, float scale ) = 0;
};