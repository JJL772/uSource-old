#pragma once

#include "game/server/weapons.h"


class CSqueak : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 5; }
	int GetItemInfo(ItemInfo *p) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0 ) override;
	void WeaponIdle( void ) override;
	int m_fJustThrown;

	virtual BOOL UseDecrement( void )
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usSnarkFire;
};