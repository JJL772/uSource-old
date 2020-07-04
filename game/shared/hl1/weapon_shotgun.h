#pragma once

#include "game/server/weapons.h"

class CShotgun : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	static	TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( ) override { return 3; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	BOOL Deploy( ) override;
	void Reload( void ) override;
	void WeaponTick() override;
	void WeaponIdle( void ) override;
	int m_fInReload;
	float m_flNextReload;
	int m_iShell;

	virtual BOOL UseDecrement( void )
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usDoubleFire;
	unsigned short m_usSingleFire;
};