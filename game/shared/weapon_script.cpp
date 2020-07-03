#include "weapon_script.h"

#include "game_shared.h"

CWeaponScript::CWeaponScript(const char *filename) 
{
	this->m_kv = new KeyValues();
	this->m_kv->ParseFile(filename);
	
}

CWeaponScript::~CWeaponScript() 
{

}

float CWeaponScript::GetFloatProp(const char *property, float _default = 0.0f) 
{

}

int CWeaponScript::GetIntProp(const char *property, int _default = 0) 
{

}

const char *CWeaponScript::GetStringProp(const char *property, const char *_default = "")  
{

}

List<const char *> CWeaponScript::GetArrayProp() 
{

}

List<const char *> CWeaponScript::GetHitSounds() 
{

}

List<const char *> CWeaponScript::GetPrimaryFireSounds() 
{

}

List<const char *> CWeaponScript::GetSecondaryFireSounds() 
{

}

const char *CWeaponScript::GetViewModel() 
{

}

const char *CWeaponScript::GetWorldModel() 
{

}

const char *CWeaponScript::GetWeaponModel() 
{

}

const char *CWeaponScript::GetWeaponName() 
{

}

const char *CWeaponScript::GetEvenScript() 
{

}
