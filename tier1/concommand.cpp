#include "concommand.h"

#include "public/containers/array.h"

static Array<ConCommand*>* g_CommandsForRegistration = nullptr;

ConCommand::ConCommand(const char *_name, const char *_desc, unsigned int _flags, void (*_command)()) :
	name(_name),
	desc(_desc),
	flags(_flags),
	command(_command)
{
	if(!g_CommandsForRegistration)
		g_CommandsForRegistration = new Array<ConCommand*>();
	g_CommandsForRegistration->push_back(this);
}

void ConCommand::RegisterAllCommands()
{
	if(!g_pEngineCvar || !g_CommandsForRegistration) return;

	for(auto x : *g_CommandsForRegistration)
	{
		g_pEngineCvar->AddCommand(x->name, x->command, x->desc, x->flags);
	}
}