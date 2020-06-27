#pragma once

#include "appframework.h"
#include "event_api.h"

#define IEVENTINTERFACE_001 "IEngineEvent001"
#define IEVENTINTERFACE_INTERFACE IEVENTINTERFACE_001

class IEngineEvents : public IAppInterface
{
public:
	virtual void	PlaySound ( int ent, float *origin, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch ) = 0;
	virtual void	StopSound ( int ent, int channel, const char *sample ) = 0;
	virtual int	FindModelIndex ( const char *pmodel ) = 0;
	virtual int	IsLocal ( int playernum ) = 0;
	virtual int	LocalPlayerDucking ( void ) = 0;
	virtual void	LocalPlayerViewheight ( float * ) = 0;
	virtual void	LocalPlayerBounds ( int hull, float *mins, float *maxs ) = 0;
	virtual int	IndexFromTrace( struct pmtrace_s *pTrace ) = 0;
	virtual struct physent_s *GetPhysent ( int idx ) = 0;
	virtual void	SetUpPlayerPrediction ( int dopred, int bIncludeLocalClient ) = 0;
	virtual void	PushPMStates ( void ) = 0;
	virtual void	PopPMStates ( void ) = 0;
	virtual void	SetSolidPlayers ( int playernum ) = 0;
	virtual void	SetTraceHull ( int hull ) = 0;
	virtual void	PlayerTrace ( float *start, float *end, int traceFlags, int ignore_pe, struct pmtrace_s *tr ) = 0;
	virtual void	WeaponAnimation ( int sequence, int body ) = 0;
	virtual unsigned short PrecacheEvent ( int type, const char* psz ) = 0;
	virtual void	PlaybackEvent ( int flags, const struct edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 ) = 0;
	virtual const char *TraceTexture ( int ground, float *vstart, float *vend ) = 0;
	virtual void	StopAllSounds ( int entnum, int entchannel ) = 0;
	virtual void	KillEvents ( int entnum, const char *eventname ) = 0;
 
	// Xash3D extension
	virtual void	PlayerTraceExt ( float *start, float *end, int traceFlags, int (*pfnIgnore) ( struct physent_s *pe ), struct pmtrace_s *tr ) = 0;
	virtual const char *SoundForIndex( int index ) = 0;
	virtual struct msurface_s *TraceSurface ( int ground, float *vstart, float *vend ) = 0;
	virtual struct movevars_s *GetMovevars ( void ) = 0;
	virtual struct pmtrace_s *VisTraceLine ( float *start, float *end, int flags ) = 0;
	virtual struct physent_s *GetVisent ( int idx ) = 0;
	virtual int	TestLine( const vec3_t start, const vec3_t end, int flags ) = 0;
	virtual void	PushTraceBounds( int hullnum, const float *mins, const float *maxs ) = 0;
	virtual void	PopTraceBounds( void ) = 0;
};