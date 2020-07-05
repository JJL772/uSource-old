/**
 * 
 * convar.h
 * 
 * Near little adapters for console commands and vars 
 * 
 */ 
#pragma once 

#include "cvardef.h"

class Convar
{
private:
	const char* const name;
	const char* const _default;
	const unsigned int flags;
	const char* const desc;
public:
	Convar(const char* name, const char* _default, unsigned int flags = 0, const char* description = "");

	float GetFloat() const;
	int GetInt() const;
	const char* GetString() const;
	bool GetBool() const;

	void SetFloat(float f);
	void SetInt(int i);
	void SetString(const char* s);
	void Set(const char* s);
	void SetBool(bool b);

	/* Resets to default */
	void Reset();

	const char* Default() { return _default; };
	const char* Name() { return name; };
	const char* Description() { return desc; };
	unsigned int Flags() { return flags; };

	static void RegisterAllCvars();
};