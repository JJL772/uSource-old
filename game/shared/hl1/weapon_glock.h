#pragma once

#include "game/server/weapons.h"

class CGlock : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 2; }
	int GetItemInfo( ItemInfo *p ) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	void GlockFire( float flSpread, float flCycleTime, BOOL fUseAutoAim );
	BOOL Deploy( void ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	int m_iShell;

	unsigned short m_usFireGlock1;
	unsigned short m_usFireGlock2;
};