/**
 * 
 * Functions and definitions shared between the client and server
 * 
 */ 
#pragma once

#include "common.h"
#include "mathlib/mathlib.h"
#include "public/appframework.h"
#include "engine_int.h"
#include "log_int.h"
#include "tier1/tier1.h"

#ifdef CLIENT_DLL
#include "client_int.h"
#else
#include "server_int.h"
#endif

/* Called to initialize the shared stuff */
extern void GameSharedInit();