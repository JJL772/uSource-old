#pragma once

#include "game/server/weapons.h"

class CCrowbar : public CBasePlayerWeapon
{
public:
	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 1; }
	void EXPORT SwingAgain( void );
	void EXPORT Smack( void );
	int GetItemInfo( ItemInfo *p ) override;
	int AddToPlayer( CBasePlayer *pPlayer ) override;

	void PrimaryAttack( void ) override;
	int Swing( int fFirst );
	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0 ) override;
#ifdef CROWBAR_IDLE_ANIM
	void WeaponIdle();
#endif
	int m_iSwing;
	TraceResult m_trHit;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
private:
	unsigned short m_usCrowbar;
};