#pragma once

#include "game/server/weapons.h"

class CMP5 : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 3; }
	int GetItemInfo(ItemInfo *p) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	int SecondaryAmmoIndex( void ) override;
	BOOL Deploy( void ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;
	BOOL IsUseable() override;
	float m_flNextAnimTime;
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
	unsigned short m_usMP5;
	unsigned short m_usMP52;
};