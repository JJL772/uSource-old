#pragma once

#include "game/server/weapons.h"
#include "basegrenade.h"

class CRpgRocket : public CGrenade
{
public:
	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	static	TYPEDESCRIPTION m_SaveData[];
	void Spawn( void ) override;
	void Precache( void ) override;

	void EXPORT FollowThink( void );
	void EXPORT IgniteThink( void );
	void EXPORT RocketTouch( CBaseEntity *pOther );
	static CRpgRocket *CreateRpgRocket( Vector vecOrigin, Vector vecAngles, CBaseEntity *pOwner, class CRpg *pLauncher );

	int m_iTrail;
	float m_flIgniteTime;
	EHANDLE m_hLauncher; // handle back to the launcher that fired me. 
};

class CRpg : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	static	TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn( void ) override;
	void Precache( void ) override;
	void Reload( void ) override;
	int iItemSlot( void ) override { return 4; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	BOOL Deploy( void ) override;
	BOOL CanHolster( void ) override;
	void Holster( int skiplocal = 0 ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	void WeaponIdle( void ) override;

	void UpdateSpot( void );
	BOOL ShouldWeaponIdle( void ) override { return TRUE; };

	CLaserSpot *m_pSpot;
	int m_fSpotActive;
	int m_cActiveRockets;// how many missiles in flight from this launcher right now?

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usRpg;
};