/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  cdll_int.c
//
// this implementation handles the linking of the engine to the DLL
//

#include "hud.h"
#include "cl_util.h"
#include "netadr.h"
#include "parsemsg.h"

#if defined(GOLDSOURCE_SUPPORT) && (defined(_WIN32) || defined(__linux__) || defined(__APPLE__)) && (defined(__i386) || defined(_M_IX86))
#define USE_VGUI_FOR_GOLDSOURCE_SUPPORT
#include "VGUI_Panel.h"
#include "VGUI_App.h"
#endif

#include "pm_shared.h"
#include "ref_params.h"
#include "client_int.h"

#include <string.h>

IEngineDemo* g_pDemoAPI = nullptr;
IEngineEvents* g_pEventAPI = nullptr;
IEngineEfx* g_pEfxAPI = nullptr;
IEngineNetAPI* g_pNetAPI = nullptr;

cl_enginefunc_t gEngfuncs;
CHud gHUD;
mobile_engfuncs_t *gMobileEngfuncs = NULL;

extern "C" int g_bhopcap;
void InitInput( void );
void EV_HookEvents( void );
void IN_Commands( void );

int __MsgFunc_Bhopcap( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	g_bhopcap = READ_BYTE();

	return 1;
}

extern "C" 
{
int	DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
int	DLLEXPORT HUD_VidInit( void );
void	DLLEXPORT HUD_Init( void );
int	DLLEXPORT HUD_Redraw( float flTime, int intermission );
int	DLLEXPORT HUD_UpdateClientData( client_data_t *cdata, float flTime );
void	DLLEXPORT HUD_Reset ( void );
void	DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server );
void	DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove );
char	DLLEXPORT HUD_PlayerMoveTexture( char *name );
int	DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
int	DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs );
void	DLLEXPORT HUD_Frame( double time );
void	DLLEXPORT HUD_VoiceStatus(int entindex, qboolean bTalking);
void	DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf );
void 	DLLEXPORT HUD_MobilityInterface( mobile_engfuncs_t *gpMobileEngfuncs );

/* From in_camera.cpp */
extern void 	DLLEXPORT CAM_Think( void );
extern int 	DLLEXPORT CL_IsThirdPerson( void );
extern void 	DLLEXPORT CL_CameraOffset( float *ofs );

extern int DLLEXPORT HUD_AddEntity( int type, struct cl_entity_s *ent, const char *modelname );
extern void DLLEXPORT HUD_CreateEntities( void );
extern void DLLEXPORT HUD_StudioEvent( const struct mstudioevent_s *event, const struct cl_entity_s *entity );
extern void DLLEXPORT HUD_TxferLocalOverrides( struct entity_state_s *state, const struct clientdata_s *client );
extern void DLLEXPORT HUD_ProcessPlayerState( struct entity_state_s *dst, const struct entity_state_s *src );
extern void DLLEXPORT HUD_TxferPredictionData ( struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd );
extern void DLLEXPORT HUD_TempEntUpdate( double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int ( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void ( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
extern struct cl_entity_s DLLEXPORT *HUD_GetUserEntity( int index );

extern void DLLEXPORT HUD_DrawNormalTriangles( void );
extern void DLLEXPORT HUD_DrawTransparentTriangles( void );

extern void DLLEXPORT V_CalcRefdef( struct ref_params_s *pparams );

extern void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer );

}

class CHL1Client : public IClientInterface
{
public:
	virtual const char* GetParentInterface() { return ICLIENTINTERFACE_INTERFACE; };
	virtual const char* GetName() { return "CHL1Client001"; };
	virtual bool PreInit() { return true; };

	virtual bool Init()
	{
		g_pDemoAPI = (IEngineDemo*)AppFramework::FindInterface(IDEMOAPI_INTERFACE);
		g_pEfxAPI = (IEngineEfx*)AppFramework::FindInterface(IEFX_INTERFACE);
		g_pEventAPI = (IEngineEvents*)AppFramework::FindInterface(IEVENTINTERFACE_INTERFACE);
		g_pNetAPI = (IEngineNetAPI*)AppFramework::FindInterface(INETAPI_INTERFACE);
		return true;
	}

	virtual void InitHud()
	{
		HUD_Init();
	}

	virtual void Shutdown() {};

	virtual int Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
	{	
		return ::Initialize(pEnginefuncs, iVersion);
	}

	virtual int VidInit( void )
	{
		return HUD_VidInit();
	}

	virtual int Redraw( float flTime, int intermission )
	{
		return HUD_Redraw(flTime, intermission);
	}

	virtual int UpdateClientData( client_data_t *cdata, float flTime )
	{
		return HUD_UpdateClientData(cdata, flTime);
	}

	virtual void Reset( void )
	{
		HUD_Reset();
	}

	virtual void PlayerMove( struct playermove_s *ppmove, int server )
	{
		HUD_PlayerMove(ppmove, server);
	}

	virtual void PlayerMoveInit( struct playermove_s *ppmove )
	{
		HUD_PlayerMoveInit(ppmove);
	}

	virtual char PlayerMoveTexture( char *name )
	{
		return HUD_PlayerMoveTexture(name);
	}

	virtual void CL_CreateMove( float frametime, struct usercmd_s *cmd, int active )
	{
		
	}

	virtual int CL_IsThirdPerson( void )
	{
		return ::CL_IsThirdPerson();
	}

	virtual void CL_CameraOffset( float *ofs )
	{
		return ::CL_CameraOffset(ofs);
	}

	virtual void CAM_Think( void )
	{
		::CAM_Think();
	}

	virtual void CalcRefdef( ref_params_t *pparams )
	{
		::V_CalcRefdef(pparams);
	}

	virtual int AddEntity( int type, cl_entity_t *ent, const char *modelname )
	{
		return ::HUD_AddEntity(type, ent, modelname);
	}

	virtual void CreateEntities( void )
	{
		::HUD_CreateEntities();
	}

	virtual void DrawNormalTriangles( void )
	{
		::HUD_DrawNormalTriangles();
	}

	virtual void DrawTransparentTriangles( void )
	{
		::HUD_DrawTransparentTriangles();
	}

	virtual void StudioEvent( const struct mstudioevent_s *event, const cl_entity_t *entity )
	{
		::HUD_StudioEvent(event, entity);
	}

	virtual void PostRunCmd( struct local_state_s *from, struct local_state_s *to, usercmd_t *cmd, int runfuncs, double time, unsigned int random_seed )
	{
		
	}

	virtual void GameShutdown( void )
	{
		
	}

	virtual void TxferLocalOverrides( entity_state_t *state, const clientdata_t *client )
	{
		::HUD_TxferLocalOverrides(state, client);
	}

	virtual void ProcessPlayerState( entity_state_t *dst, const entity_state_t *src )
	{
		::HUD_ProcessPlayerState(dst, src);
	}

	virtual void TxferPredictionData( entity_state_t *ps, const entity_state_t *pps, clientdata_t *pcd, const clientdata_t *ppcd, weapon_data_t *wd, const weapon_data_t *pwd )
	{
		::HUD_TxferPredictionData(ps, pps, pcd, ppcd, wd, pwd);
	}

	virtual void Demo_ReadBuffer( int size, byte *buffer )
	{
		::Demo_ReadBuffer(size, buffer);
	}

	virtual int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *buffer, int *size )
	{
		return HUD_ConnectionlessPacket(net_from, args, buffer, size);
	}

	virtual int GetHullBounds( int hullnumber, float *mins, float *maxs )
	{
		return HUD_GetHullBounds(hullnumber, mins, maxs);
	}

	virtual void Frame( double time )
	{
		HUD_Frame(time);
	}

	virtual void TempEntUpdate( double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int ( *Callback_AddVisibleEntity )( cl_entity_t *pEntity ), void ( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ))
	{
		::HUD_TempEntUpdate(frametime, client_time, cl_gravity, ppTempEntFree, ppTempEntActive, Callback_AddVisibleEntity, Callback_TempEntPlaySound);
	}

	virtual cl_entity_t *GetUserEntity( int index )
	{
		return ::HUD_GetUserEntity(index);
	}

	virtual void VoiceStatus( int entindex, qboolean bTalking )
	{
		HUD_VoiceStatus(entindex, bTalking);
	}

	virtual void DirectorMessage( int iSize, void *pbuf )
	{
		HUD_DirectorMessage(iSize, pbuf);
	}

	virtual void ChatInputPosition( int *x, int *y )
	{
		
	}

	virtual void ClipMoveToEntity( struct physent_s *pe, const vec3_t start, vec3_t mins, vec3_t maxs, const vec3_t end, struct pmtrace_s *tr )
	{
		
	}

};

EXPOSE_INTERFACE(CHL1Client);
MODULE_INTERFACE_IMPL();

/*
========================== 
    Initialize

Called when the DLL is first loaded.
==========================
*/


/*
================================
HUD_GetHullBounds

  Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int DLLEXPORT HUD_GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	int iret = 0;

	switch( hullnumber )
	{
	case 0:				// Normal player
		Vector( -16, -16, -36 ).CopyToArray(mins);
		Vector( 16, 16, 36 ).CopyToArray(maxs);
		iret = 1;
		break;
	case 1:				// Crouched player
		Vector( -16, -16, -18 ).CopyToArray(mins);
		Vector( 16, 16, 18 ).CopyToArray(maxs);
		iret = 1;
		break;
	case 2:				// Point based hull
		Vector( 0, 0, 0 ).CopyToArray(mins);
		Vector( 0, 0, 0 ).CopyToArray(maxs);
		iret = 1;
		break;
	}

	return iret;
}

/*
================================
HUD_ConnectionlessPacket

 Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
  size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int DLLEXPORT HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	// Parse stuff from args
	int max_buffer_size = *response_buffer_size;

	// Zero it out since we aren't going to respond.
	// If we wanted to response, we'd write data into response_buffer
	*response_buffer_size = 0;

	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead.
	return 0;
}

void DLLEXPORT HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
	PM_Init( ppmove );
}

char DLLEXPORT HUD_PlayerMoveTexture( char *name )
{
	return PM_FindTextureType( name );
}

void DLLEXPORT HUD_PlayerMove( struct playermove_s *ppmove, int server )
{
	PM_Move( ppmove, server );
}

int DLLEXPORT Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	gEngfuncs = *pEnginefuncs;

	if( iVersion != CLDLL_INTERFACE_VERSION )
		return 0;

	memcpy( &gEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t) );

	EV_HookEvents();

	return 1;
}

/*
=================
HUD_GetRect

VGui stub
=================
*/
int *HUD_GetRect( void )
{
	static int extent[4];

	extent[0] = gEngfuncs.GetWindowCenterX() - ScreenWidth / 2;
	extent[1] = gEngfuncs.GetWindowCenterY() - ScreenHeight / 2;
	extent[2] = gEngfuncs.GetWindowCenterX() + ScreenWidth / 2;
	extent[3] = gEngfuncs.GetWindowCenterY() + ScreenHeight / 2;

	return extent;
}

#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
class TeamFortressViewport : public vgui::Panel
{
public:
	TeamFortressViewport(int x,int y,int wide,int tall);
	void Initialize( void );

	virtual void paintBackground();
	void *operator new( size_t stAllocateBlock );
};

static TeamFortressViewport* gViewPort = NULL;

TeamFortressViewport::TeamFortressViewport(int x, int y, int wide, int tall) : Panel(x, y, wide, tall)
{
	gViewPort = this;
	Initialize();
}

void TeamFortressViewport::Initialize()
{
	//vgui::App::getInstance()->setCursorOveride( vgui::App::getInstance()->getScheme()->getCursor(vgui::Scheme::scu_none) );
}

void TeamFortressViewport::paintBackground()
{
//	int wide, tall;
//	getParent()->getSize( wide, tall );
//	setSize( wide, tall );
	int extents[4];
	getParent()->getAbsExtents(extents[0],extents[1],extents[2],extents[3]);
	gEngfuncs.VGui_ViewportPaintBackground(extents);
}

void *TeamFortressViewport::operator new( size_t stAllocateBlock )
{
	void *mem = ::operator new( stAllocateBlock );
	memset( mem, 0, stAllocateBlock );
	return mem;
}
#endif

/*
==========================
	HUD_VidInit

Called when the game initializes
and whenever the vid_mode is changed
so the HUD can reinitialize itself.
==========================
*/

int DLLEXPORT HUD_VidInit( void )
{
	gHUD.VidInit();
#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
	vgui::Panel* root=(vgui::Panel*)gEngfuncs.VGui_GetPanel();
	if (root) {
		gEngfuncs.Con_Printf( "Root VGUI panel exists\n" );
		root->setBgColor(128,128,0,0);

		if (gViewPort != NULL)
		{
			gViewPort->Initialize();
		}
		else
		{
			gViewPort = new TeamFortressViewport(0,0,root->getWide(),root->getTall());
			gViewPort->setParent(root);
		}
	} else {
		gEngfuncs.Con_Printf( "Root VGUI panel does not exist\n" );
	}
#endif
	return 1;
}

/*
==========================
	HUD_Init

Called whenever the client connects
to a server.  Reinitializes all 
the hud variables.
==========================
*/

void DLLEXPORT HUD_Init( void )
{
	InitInput();
	gHUD.Init();

	gEngfuncs.pfnHookUserMsg( "Bhopcap", __MsgFunc_Bhopcap );
}

/*
==========================
	HUD_Redraw

called every screen frame to
redraw the HUD.
===========================
*/

int DLLEXPORT HUD_Redraw( float time, int intermission )
{
	gHUD.Redraw( time, intermission );

	return 1;
}

/*
==========================
	HUD_UpdateClientData

called every time shared client
dll/engine data gets changed,
and gives the cdll a chance
to modify the data.

returns 1 if anything has been changed, 0 otherwise.
==========================
*/

int DLLEXPORT HUD_UpdateClientData( client_data_t *pcldata, float flTime )
{
	IN_Commands();

	return gHUD.UpdateClientData( pcldata, flTime );
}

/*
==========================
	HUD_Reset

Called at start and end of demos to restore to "non"HUD state.
==========================
*/

void DLLEXPORT HUD_Reset( void )
{
	gHUD.VidInit();
}

/*
==========================
HUD_Frame

Called by engine every frame that client .dll is loaded
==========================
*/

void DLLEXPORT HUD_Frame( double time )
{
#ifdef USE_VGUI_FOR_GOLDSOURCE_SUPPORT
	if (!gViewPort)
		gEngfuncs.VGui_ViewportPaintBackground(HUD_GetRect());
#else
	gEngfuncs.VGui_ViewportPaintBackground(HUD_GetRect());
#endif
}

/*
==========================
HUD_VoiceStatus

Called when a player starts or stops talking.
==========================
*/

void DLLEXPORT HUD_VoiceStatus( int entindex, qboolean bTalking )
{

}

/*
==========================
HUD_DirectorEvent

Called when a director event message was received
==========================
*/

void DLLEXPORT HUD_DirectorMessage( int iSize, void *pbuf )
{
	 gHUD.m_Spectator.DirectorMessage( iSize, pbuf );
}

void DLLEXPORT HUD_MobilityInterface( mobile_engfuncs_t *gpMobileEngfuncs )
{
	if( gpMobileEngfuncs->version != MOBILITY_API_VERSION )
		return;
	gMobileEngfuncs = gpMobileEngfuncs;
}

bool HUD_MessageBox( const char *msg )
{
	gEngfuncs.Con_Printf( msg ); // just in case

	if( IsXashFWGS() )
	{
		gMobileEngfuncs->pfnSys_Warn( msg );
		return true;
	}

	// TODO: Load SDL2 and call ShowSimpleMessageBox

	return false;
}

bool IsXashFWGS()
{
	return gMobileEngfuncs != NULL;
}
