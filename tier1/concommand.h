/**
 * 
 * concommand.h
 * 	Console command helpers
 * 
 */ 
#pragma once

#include "tier1.h"

class ConCommand
{
private:
	const char* const name;
	const char* const desc;
	const unsigned int flags;
	void(*command)();
public:
	ConCommand(const char* _name, const char* _desc, unsigned int _flags, void(*_command)());

	static void RegisterAllCommands();
};

#define CONCOMMAND(_name, _desc, _flags) \
namespace __func_command_ ## _name ## _namespace { \
int argc = 0; const char** argv = nullptr; \
void __func_command_ ## _name ();\
} \
ConCommand _name(#_name, _desc, _flags, []() {\
	__func_command_ ## _name ## _namespace::argc = g_pEngineCvar->CmdArgc();\
	__func_command_ ## _name ## _namespace::argv = g_pEngineCvar->CmdArgv();\
	__func_command_ ## _name ## _namespace::__func_command_ ## _name (); \
}); \
void __func_command_ ## _name ## _namespace::__func_command_ ## _name ()