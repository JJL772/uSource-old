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
#include "public/containers/array.h"

#define WEAPON_SCRIPT_NAME "name"
#define WEAPON_SCRIPT_WORLD_MODEL "world_model"
#define WEAPON_SCRIPT_VIEW_MODEL "view_model"
#define WEAPON_SCRIPT_EVENT_SCRIPT "event_script"
#define WEAPON_SCRIPT_WPN_MODEL "weapon_model"
#define WEAPON_SCRIPT_SOUNDS "sounds"
#define WEAPON_SCRIPT_PROPS "properties"

struct weapon_script_sound_t
{
	const char* name;
	const char* snd;
};

class CWeaponScript
{
public:
	KeyValues* m_kv;
	KeyValues* m_props;
	KeyValues* m_sounds;

	CWeaponScript(const char* filename);

	~CWeaponScript();

	const KeyValues& GetProperties() const;

	const KeyValues& GetSounds() const;

	Array<weapon_script_sound_t> GetSounds();

	const char* GetViewModel();

	const char* GetWorldModel();

	const char* GetWeaponModel();

	const char* GetWeaponName();

	const char* GetEventScript();

	bool Good() const;
};