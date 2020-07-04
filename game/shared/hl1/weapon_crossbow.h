#pragma once

#include "game/server/weapons.h"

class CCrossbow : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( ) override { return 3; }
	int GetItemInfo(ItemInfo *p) override;

	void FireBolt( void );
	void FireSniperBolt( void );
	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;
	BOOL Deploy( ) override;
	void Holster( int skiplocal = 0 ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;

	int m_fInZoom; // don't save this

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usCrossbow;
	unsigned short m_usCrossbow2;
};