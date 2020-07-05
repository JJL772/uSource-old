#include "tier1.h"
#include "tier1/convar.h"

IEngineCvar* g_pEngineCvar;
ILogSystem* g_pLogSystem;
IEngineFilesystem* g_pFilesystem;

void ConnectTier1Libraries()
{
	g_pFilesystem = (IEngineFilesystem*)AppFramework::FindInterface(IENGINEFILESYSTEM_INTERFACE);
	g_pLogSystem = (ILogSystem*)AppFramework::FindInterface(ILOGSYSTEM_INTERFACE);
	g_pEngineCvar = (IEngineCvar*)AppFramework::FindInterface(IENGINECVAR_INTERFACE);

	Convar::RegisterAllCvars();
}