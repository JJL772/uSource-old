#pragma once

#include "game/server/weapons.h"

class CPython : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 2; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;
	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0 ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;
	float m_flSoundDelay;

	BOOL m_fInZoom;// don't save this. 

	virtual BOOL UseDecrement( void )
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usFirePython;
};