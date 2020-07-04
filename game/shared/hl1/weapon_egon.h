#pragma once

#include "game/server/weapons.h"

class CEgon : public CBasePlayerWeapon
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

	BOOL Deploy( void ) override;
	void Holster( int skiplocal = 0 ) override;

	void UpdateEffect( const Vector &startPoint, const Vector &endPoint, float timeBlend );

	void CreateEffect ( void );
	void DestroyEffect ( void );

	void EndAttack( void );
	void Attack( void );
	void PrimaryAttack( void ) override;
	void WeaponIdle( void ) override;

	float m_flAmmoUseTime;// since we use < 1 point of ammo per update, we subtract ammo on a timer.

	float GetPulseInterval( void );
	float GetDischargeInterval( void );

	void Fire( const Vector &vecOrigSrc, const Vector &vecDir );

	BOOL HasAmmo( void );

	void UseAmmo( int count );

	enum EGON_FIREMODE { FIRE_NARROW, FIRE_WIDE};

	CBeam				*m_pBeam;
	CBeam				*m_pNoise;
	CSprite				*m_pSprite;

	virtual BOOL UseDecrement( void )
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

	unsigned short m_usEgonStop;

private:
#ifndef CLIENT_DLL
	float				m_shootTime;
#endif
	EGON_FIREMODE		m_fireMode;
	float				m_shakeTime;
	BOOL				m_deployed;

	unsigned short m_usEgonFire;
};