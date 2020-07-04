#pragma once

#include "game/server/weapons.h"

class CGauss : public CBasePlayerWeapon
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
	BOOL IsUseable() override;
	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0  ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	void WeaponIdle( void ) override;

	void StartFire( void );
	void Fire( Vector vecOrigSrc, Vector vecDirShooting, float flDamage );
	float GetFullChargeTime( void );
	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState; // don't save this

	// was this weapon just fired primary or secondary?
	// we need to know so we can pick the right set of effects. 
	BOOL m_fPrimaryFire;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usGaussFire;
	unsigned short m_usGaussSpin;
};
