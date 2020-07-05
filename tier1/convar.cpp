#include "convar.h"
#include "tier1.h"

#include "public/containers/array.h"
#include "crtlib.h"

/**
 * getting around the static initialization fiasco...
 *  the Convar constructor will be called whenever a convar is going to be registered, and that will generally happen
 *  during static initialization. However, during static init g_pEngineCvar is not valid yet (it's set in ConnectTier1Libraries)
 *  so, instead of calling g_pEngineCvar procedures, we will add the cvars to a list, and register them after g_pEngineCvar becomes valid.
 *  Convar::RegisterAllCvars() will be called from ConnectTier1Libraries()
 */ 

static Array<Convar*>* g_pConvarRegistrationQueue = nullptr;

void Convar::RegisterAllCvars()
{
	if(!g_pEngineCvar || !g_pConvarRegistrationQueue) return;
	g_pEngineCvar->CvarInit();

	for(auto x : *g_pConvarRegistrationQueue)
	{
		g_pEngineCvar->RegisterCvar(x->name, x->_default, x->desc, x->flags);
	}
}

Convar::Convar(const char *_name, const char *def, unsigned int _flags, const char *description) :
	name(_name),
	_default(def),
	flags(_flags),
	desc(description)
{
	if(!g_pConvarRegistrationQueue) 
		g_pConvarRegistrationQueue = new Array<Convar*>();
	g_pConvarRegistrationQueue->push_back(this);
}

float Convar::GetFloat() const 
{
	const char* v = g_pEngineCvar->CvarGetString(this->name);
	if(!v) return 0.0f;
	return Q_atof(v);
}

int Convar::GetInt() const 
{
	const char* v = g_pEngineCvar->CvarGetString(this->name);
	if(!v) return -1;
	return Q_atoi(v);
}

const char *Convar::GetString() const 
{
	const char* v = g_pEngineCvar->CvarGetString(this->name);
	return v ? v : "";
}

bool Convar::GetBool() const 
{
	if(!this->name) return false;
	const char* b = g_pEngineCvar->CvarGetString(this->name);
	if(!b) return false;
	if(Q_strcasecmp(b, "false") == 0 || Q_strcmp(b, "0") == 0)
		return false;
	return true;
}


void Convar::SetFloat(float f) 
{
	if(!this->name) return;
	char buf[128];
	Q_snprintf(buf, sizeof(buf), "%f", f);
	g_pEngineCvar->CvarSetString(this->name, buf);
}

void Convar::SetInt(int i) 
{
	if(!this->name) return;
	char buf[64];
	Q_snprintf(buf, sizeof(buf), "%i", i);
	g_pEngineCvar->CvarSetString(this->name, buf);
}

void Convar::SetString(const char *s) 
{
	if(!this->name || !s) return;
	g_pEngineCvar->CvarSetString(this->name, s);
}

void Convar::Set(const char *s) 
{
	if(!this->name || !s) return;
	g_pEngineCvar->CvarSetString(this->name, s);
}

void Convar::SetBool(bool b) 
{
	if(!this->name) return;
	g_pEngineCvar->CvarSetString(this->name, b ? "1" : "0");
}

void Convar::Reset()
{
	if(!this->name || !this->_default) return;
	g_pEngineCvar->CvarSetString(this->name, this->_default);
}