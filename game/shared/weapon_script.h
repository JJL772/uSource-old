/**
 * 
 * weapon_script.h
 * 
 * Weapon scripts are keyvalues scripts that let you dynamically define properties of a weapon. 
 * CWeaponScript is a thing wrapper around the KeyValues class provided by libpublic 
 * 
 */ 


#pragma once

#include "public/keyvalues.h"
#include "public/containers/list.h"

class CWeaponScript
{
public:
	KeyValues* m_kv;

	CWeaponScript(const char* filename);

	~CWeaponScript();

	float GetFloatProp(const char* property, float _default = 0.0f);

	int GetIntProp(const char* property, int _default = 0);

	const char* GetStringProp(const char* property, const char* _default = "");

	List<const char*> GetArrayProp();

	List<const char*> GetHitSounds();

	List<const char*> GetPrimaryFireSounds();

	List<const char*> GetSecondaryFireSounds();

	const char* GetViewModel();

	const char* GetWorldModel();

	const char* GetWeaponModel();

	const char* GetWeaponName();

	const char* GetEvenScript();
};