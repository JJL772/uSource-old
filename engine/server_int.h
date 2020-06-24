/**
 * 
 * server_int.h
 * 
 * Interfaces implemented by the game server
 * 
 */ 
#pragma once

#include "eiface.h"
#include "appframework.h"

#define ISERVER_001 "IServer001"
#define ISERVER_INTERFACE ISERVER_001

class IServerInterface : public IAppInterface
{
public:
		// Initialize/shutdown the game (one-time call after loading of game .dll )
	virtual void	GameInit( void ) = 0; 				
	virtual int	Spawn( edict_t *pent ) = 0; 
	virtual void	Think( edict_t *pent ) = 0; 
	virtual void	Use( edict_t *pentUsed, edict_t *pentOther ) = 0; 
	virtual void	Touch( edict_t *pentTouched, edict_t *pentOther ) = 0; 
	virtual void	Blocked( edict_t *pentBlocked, edict_t *pentOther ) = 0; 
	virtual void	KeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd ) = 0; 
	virtual void	Save( edict_t *pent, SAVERESTOREDATA *pSaveData ) = 0; 
	virtual int 	Restore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity ) = 0; 
	virtual void	SetAbsBox( edict_t *pent ) = 0; 
 
	virtual void	SaveWriteFields( SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int ) = 0; 
	virtual void	SaveReadFields( SAVERESTOREDATA*, const char*, void*, TYPEDESCRIPTION*, int ) = 0; 
	virtual void	SaveGlobalState( SAVERESTOREDATA * ) = 0; 
	virtual void	RestoreGlobalState( SAVERESTOREDATA * ) = 0; 
	virtual void	ResetGlobalState( void ) = 0; 
 
	virtual qboolean	ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128] ) = 0; 
 
	virtual void	ClientDisconnect( edict_t *pEntity ) = 0; 
	virtual void	ClientKill( edict_t *pEntity ) = 0; 
	virtual void	ClientPutInServer( edict_t *pEntity ) = 0; 
	virtual void	ClientCommand( edict_t *pEntity ) = 0; 
	virtual void	ClientUserInfoChanged( edict_t *pEntity, char *infobuffer ) = 0; 
	virtual void	ServerActivate( edict_t *pEdictList, int edictCount, int clientMax ) = 0; 
	virtual void	ServerDeactivate( void ) = 0; 
	virtual void	PlayerPreThink( edict_t *pEntity ) = 0; 
	virtual void	PlayerPostThink( edict_t *pEntity ) = 0; 
 
	virtual void	StartFrame( void ) = 0; 
	virtual void	ParmsNewLevel( void ) = 0; 
	virtual void	ParmsChangeLevel( void ) = 0; 
 
	// Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
	virtual const char     *GetGameDescription( void ) = 0; 
 
	// Notify dll about a player customization.
	virtual void	PlayerCustomization( edict_t *pEntity, customization_t *pCustom ) = 0; 
 
	// Spectator funcs
	virtual void	SpectatorConnect( edict_t *pEntity ) = 0; 
	virtual void	SpectatorDisconnect( edict_t *pEntity ) = 0; 
	virtual void	SpectatorThink( edict_t *pEntity ) = 0; 
 
	// Notify game .dll that engine is going to shut down. Allows mod authors to set a breakpoint.
	virtual void	Sys_Error( const char *error_string ) = 0; 
 
	virtual void	PM_Move( struct playermove_s *ppmove, qboolean server ) = 0; 
	virtual void	PM_Init( struct playermove_s *ppmove ) = 0; 
	virtual char	PM_FindTextureType( char *name ) = 0; 
	virtual void	SetupVisibility( struct edict_s *pViewEntity, struct edict_s *pClient, unsigned char **pvs, unsigned char **pas ) = 0; 
	virtual void	UpdateClientData( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd ) = 0; 
	virtual int	AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet ) = 0; 
	virtual void	CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, float* player_mins, float* player_maxs ) = 0; 
	virtual void	RegisterEncoders( void ) = 0; 
	virtual int	GetWeaponData( struct edict_s *player, struct weapon_data_s *info ) = 0; 
 
	virtual void	CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed ) = 0; 
	virtual void	CmdEnd( const edict_t *player ) = 0; 
 
	// Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
	//  size of the response_buffer, so you must zero it out if you choose not to respond.
	virtual int	ConnectionlessPacket ( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size ) = 0; 
 
	// Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
	virtual int	GetHullBounds( int hullnumber, float *mins, float *maxs ) = 0; 
 
	// Create baselines for certain "unplaced" items.
	virtual void	CreateInstancedBaselines ( void ) = 0; 
 
	// One of the pfnForceUnmodified files failed the consistency check for the specified player
	// Return 0 to allow the client to continue, 1 to force immediate disconnection ( with an optional disconnect message of up to 256 characters )
	virtual int	InconsistentFile( const struct edict_s *player, const char *filename, char *disconnect_message ) = 0; 
 
	// The game .dll should return 1 if lag compensation should be allowed ( could also just set
	//  the sv_unlag cvar.
	// Most games right now should return 0, until client-side weapon prediction code is written
	//  and tested for them.
	virtual int	AllowLagCompensation( void ) = 0; 
}; 