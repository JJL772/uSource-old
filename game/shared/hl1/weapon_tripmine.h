#pragma once

#include "game/server/weapons.h"

class CTripmine : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 5; }
	int GetItemInfo(ItemInfo *p) override;
	void SetObjectCollisionBox( void ) override
	{
		//!!!BUGBUG - fix the model!
		pev->absmin = pev->origin + Vector(-16, -16, -5);
		pev->absmax = pev->origin + Vector(16, 16, 28); 
	}

	void PrimaryAttack( void ) override;
	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0 ) override;
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
	unsigned short m_usTripFire;
};