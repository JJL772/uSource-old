#pragma once

#include "game/server/weapons.h"

class CHgun : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save ) override;
	int		Restore( CRestore &restore ) override;
	static	TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 4; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	BOOL Deploy( void ) override;
	BOOL IsUseable( void ) override;
	void Holster( int skiplocal = 0 ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;
	float m_flNextAnimTime;

	float m_flRechargeTime;

	int m_iFirePhase;// don't save me.

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
private:
	unsigned short m_usHornetFire;
};
