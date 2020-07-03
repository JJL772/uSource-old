/**
 * 
 * Functions and definitions shared between the client and server
 * 
 */ 

#include "game_shared.h"

#include "public/appframework.h"

IEngineFilesystem* g_pFilesystem;
ILogSystem* g_pLoggingSystem;

void GameSharedInit() 
{
	/* Load our global interfaces from the engine */
	g_pFilesystem = (IEngineFilesystem*)AppFramework::FindInterface(IENGINEFILESYSTEM_INTERFACE);
	g_pLoggingSystem = (ILogSystem*)AppFramework::FindInterface(ILOGSYSTEM_INTERFACE);
}