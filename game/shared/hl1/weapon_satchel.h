#pragma once

#include "game/server/weapons.h"

class CSatchel : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	static	TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 5; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;
	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	int AddDuplicate( CBasePlayerItem *pOriginal ) override;
	BOOL CanDeploy( void ) override;
	BOOL Deploy( void ) override;
	BOOL IsUseable( void ) override;

	void Holster( int skiplocal = 0 ) override;
	void WeaponIdle( void ) override;
	void Throw( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
};
