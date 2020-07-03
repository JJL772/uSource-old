#include "weapon_script.h"

#include "game_shared.h"

CWeaponScript::CWeaponScript(const char *filename) 
{
	this->m_kv = new KeyValues();

	String fullFile = g_pFilesystem->GetFullPath(filename);
	if(!fullFile.empty())
		m_kv->ParseFile(fullFile.c_str());

	/* NOTE: m_kv is not deleted here because we want it to be valid always */
	this->m_props = this->m_kv->GetChild(WEAPON_SCRIPT_PROPS);
	this->m_sounds = this->m_kv->GetChild(WEAPON_SCRIPT_SOUNDS);

	this->m_props = this->m_props ? this->m_props : new KeyValues();
	this->m_sounds = this->m_sounds ? this->m_sounds : new KeyValues();
}

CWeaponScript::~CWeaponScript() 
{
	delete m_kv;
	delete m_props;
	delete m_sounds;
}

const KeyValues& CWeaponScript::GetSounds() const
{
	return *m_sounds;
}

const KeyValues& CWeaponScript::GetProperties() const
{
	return *m_props;
}

Array<weapon_script_sound_t> CWeaponScript::GetSounds()
{
	Array<weapon_script_sound_t> soundlist;

	for(auto kv : m_sounds->keys)
	{
		soundlist.push_back({
			kv.key, kv.value,
		});
	}
	return soundlist;
}

const char *CWeaponScript::GetViewModel() 
{
	const char* model = m_kv->GetString(WEAPON_SCRIPT_VIEW_MODEL);
	return model ? model : "";
}

const char *CWeaponScript::GetWorldModel() 
{
	const char* model = m_kv->GetString(WEAPON_SCRIPT_WORLD_MODEL);
	return model ? model : "";
}

const char *CWeaponScript::GetWeaponModel() 
{
	const char* model = m_kv->GetString(WEAPON_SCRIPT_WPN_MODEL);
	return model ? model : "";
}

const char *CWeaponScript::GetWeaponName() 
{
	const char* name = m_kv->GetString(WEAPON_SCRIPT_NAME);
	return name ? name : "";
}

const char *CWeaponScript::GetEventScript() 
{
	const char* script = m_kv->GetString(WEAPON_SCRIPT_EVENT_SCRIPT);
	return script ? script : "";
}

bool CWeaponScript::Good() const 
{
	return this->m_kv && this->m_kv->IsGood();
}