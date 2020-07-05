#include "public/appframework.h"
#include "tier1/tier1.h"
#include "tier1/convar.h"
#include "tier1/concommand.h"

#include "common.h"
#include "cl_dll.h"
#include "hud.h"
#include "cl_util.h"

CONCOMMAND(getpos, "Returns the position of the player", 0)
{
	Vector origin = gEngfuncs.GetLocalPlayer()->origin;
	Vector angles = gEngfuncs.GetLocalPlayer()->angles;
	gEngfuncs.Con_Printf("Pos: (%f,%f,%f) Angles: (%f,%f,%f)\n", origin.x, origin.y, origin.z,
		angles.x, angles.y, angles.z);
}