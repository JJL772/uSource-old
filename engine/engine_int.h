/**
 * 
 * engine_int.h
 * 
 * Various interfaces used and exported by the engine itself
 * 
 */
#pragma once

#include "appframework.h"
#include "eiface.h"

#define IENGINETRACE_001 "IEngineTrace001"
#define IENGINETRACE_INTERFACE IENGINETRACE_001

#define IENGINECVAR_001 "IEngineCvar001"
#define IENGINECVAR_INTERFACE IENGINECVAR_001

#define IENGINEINTERFACE_001 "IEngineInterface001"
#define IENGINEINTERFACE_INTERFACE IENGINEINTERFACE_001

class IEngineTrace : public IAppInterface
{
public:
	virtual void TraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) = 0;
	virtual void TraceToss(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr) = 0;
	virtual int TraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr) = 0;
	virtual void TraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr) = 0;
	virtual void TraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr) = 0;
	virtual const char *TraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2) = 0;
	virtual void TraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr) = 0;
};

class IEngineCvar : public IAppInterface
{
public:
	virtual void CvarRegister(cvar_t *pCvar) = 0;
	virtual float CvarGetFloat(const char *szVarName) = 0;
	virtual const char *CvarGetString(const char *szVarName) = 0;
	virtual void CvarSetFloat(const char *szVarName, float flValue) = 0;
	virtual void CvarSetString(const char *szVarName, const char *szValue) = 0;
	virtual void Cvar_RegisterVariable(cvar_t *variable) = 0;
	virtual void QueryClientCvarValue(const edict_t *player, const char *cvarName) = 0;
	virtual void QueryClientCvarValue2(const edict_t *player, const char *cvarName, int requestID) = 0;
	virtual cvar_t *CVarGetPointer(const char *szVarName) = 0;
	virtual void Cvar_DirectSet(struct cvar_s *var, const char *value) = 0;
	virtual void AddServerCommand(const char *cmd_name, void (*function)(void)) = 0;
};

class IEngineInterface : public IAppInterface
{
public:
	virtual int PrecacheModel(const char *s) = 0;
	virtual int PrecacheSound(const char *s) = 0;
	virtual void SetModel(edict_t *e, const char *m) = 0;
	virtual int ModelIndex(const char *m) = 0;
	virtual int ModelFrames(int modelIndex) = 0;
	virtual void SetSize(edict_t *e, const float *rgflMin, const float *rgflMax) = 0;
	virtual void ChangeLevel(const char *s1, const char *s2) = 0;
	virtual void GetSpawnParms(edict_t *ent) = 0;
	virtual void SaveSpawnParms(edict_t *ent) = 0;
	virtual float VecToYaw(const float *rgflVector) = 0;
	virtual void VecToAngles(const float *rgflVectorIn, float *rgflVectorOut) = 0;
	virtual void MoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType) = 0;
	virtual void ChangeYaw(edict_t *ent) = 0;
	virtual void ChangePitch(edict_t *ent) = 0;
	virtual edict_t *FindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue) = 0;
	virtual int GetEntityIllum(edict_t *pEnt) = 0;
	virtual edict_t *FindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad) = 0;
	virtual edict_t *FindClientInPVS(edict_t *pEdict) = 0;
	virtual edict_t *EntitiesInPVS(edict_t *pplayer) = 0;
	virtual void MakeVectors(const float *rgflVector) = 0;
	virtual void AngleVectors(const float *rgflVector, float *forward, float *right, float *up) = 0;
	virtual edict_t *CreateEntity(void) = 0;
	virtual void RemoveEntity(edict_t *e) = 0;
	virtual edict_t *CreateNamedEntity(int className) = 0;
	virtual void MakeStatic(edict_t *ent) = 0;
	virtual int EntIsOnFloor(edict_t *e) = 0;
	virtual int DropToFloor(edict_t *e) = 0;
	virtual int WalkMove(edict_t *ent, float yaw, float dist, int iMode) = 0;
	virtual void SetOrigin(edict_t *e, const float *rgflOrigin) = 0;
	virtual void EmitSound(edict_t *entity, int channel, const char *sample, /*int*/ float volume, float attenuation, int fFlags, int pitch) = 0;
	virtual void EmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch) = 0;
	virtual void GetAimVector(edict_t *ent, float speed, float *rgflReturn) = 0;
	virtual void ServerCommand(const char *str) = 0;
	virtual void ServerExecute(void) = 0;
	virtual void ClientCommand(edict_t *pEdict, char *szFmt, ...) = 0;
	virtual void ParticleEffect(const float *org, const float *dir, float color, float count) = 0;
	virtual void LightStyle(int style, const char *val) = 0;
	virtual int DecalIndex(const char *name) = 0;
	virtual int PointContents(const float *rgflVector) = 0;
	virtual void MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) = 0;
	virtual void MessageEnd(void) = 0;
	virtual void WriteByte(int iValue) = 0;
	virtual void WriteChar(int iValue) = 0;
	virtual void WriteShort(int iValue) = 0;
	virtual void WriteLong(int iValue) = 0;
	virtual void WriteAngle(float flValue) = 0;
	virtual void WriteCoord(float flValue) = 0;
	virtual void WriteString(const char *sz) = 0;
	virtual void WriteEntity(int iValue) = 0;
	virtual void AlertMessage(ALERT_TYPE atype, char *szFmt, ...) = 0;
	virtual void EngineFprintf(FILE *pfile, char *szFmt, ...) = 0;
	virtual void *PvAllocEntPrivateData(edict_t *pEdict, long cb) = 0;
	virtual void *PvEntPrivateData(edict_t *pEdict) = 0;
	virtual void FreeEntPrivateData(edict_t *pEdict) = 0;
	virtual const char *SzFromIndex(int iString) = 0;
	virtual int AllocString(const char *szValue) = 0;
	virtual struct entvars_s *GetVarsOfEnt(edict_t *pEdict) = 0;
	virtual edict_t *PEntityOfEntOffset(int iEntOffset) = 0;
	virtual int EntOffsetOfPEntity(const edict_t *pEdict) = 0;
	virtual int IndexOfEdict(const edict_t *pEdict) = 0;
	virtual edict_t *EntityOfEntIndex(int iEntIndex) = 0;
	virtual edict_t *FindEntityByVars(struct entvars_s *pvars) = 0;
	virtual void *GetModelPtr(edict_t *pEdict) = 0;
	virtual int RegUserMsg(const char *pszName, int iSize) = 0;
	virtual void AnimationAutomove(const edict_t *pEdict, float flTime) = 0;
	virtual void GetBonePosition(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles) = 0;
	virtual unsigned long FunctionFromName(const char *pName) = 0;
	virtual const char *NameForFunction(unsigned long function) = 0;
	virtual void ClientPrintf(edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg) = 0; // JOHN: engine callbacks so game DLL can print messages to individual clients
	virtual void ServerPrint(const char *szMsg) = 0;
	virtual const char *Cmd_Args(void) = 0;	    // these 3 added
	virtual const char *Cmd_Argv(int argc) = 0; // so game DLL can easily
	virtual int Cmd_Argc(void) = 0;		    // access client 'cmd' strings
	virtual void GetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles) = 0;
	virtual void CRC32_Init(CRC32_t *pulCRC) = 0;
	virtual void CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *p, int len) = 0;
	virtual void CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch) = 0;
	virtual CRC32_t CRC32_Final(CRC32_t pulCRC) = 0;
	virtual int RandomLong(int lLow, int lHigh) = 0;
	virtual float RandomFloat(float flLow, float flHigh) = 0;
	virtual void SetView(const edict_t *pClient, const edict_t *pViewent) = 0;
	virtual float Time(void) = 0;
	virtual void CrosshairAngle(const edict_t *pClient, float pitch, float yaw) = 0;
	virtual byte *LoadFileForMe(const char *filename, int *pLength) = 0;
	virtual void FreeFile(void *buffer) = 0;
	virtual void EndSection(const char *pszSectionName) = 0; // trigger_endsection
	virtual int CompareFileTime(const char *filename1, const char *filename2, int *iCompare) = 0;
	virtual void GetGameDir(char *szGetGameDir) = 0;
	virtual void FadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds) = 0;
	virtual void SetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed) = 0;
	virtual edict_t *CreateFakeClient(const char *netname) = 0; // returns NULL if fake client can't be created
	virtual void RunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec) = 0;
	virtual int NumberOfEntities(void) = 0;
	virtual char *GetInfoKeyBuffer(edict_t *e) = 0; // passing in NULL gets the serverinfo
	virtual const char *InfoKeyValue(const char *infobuffer, const char *key) = 0;
	virtual void SetKeyValue(char *infobuffer, char *key, char *value) = 0;
	virtual void SetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value) = 0;
	virtual int IsMapValid(char *filename) = 0;
	virtual void StaticDecal(const float *origin, int decalIndex, int entityIndex, int modelIndex) = 0;
	virtual int PrecacheGeneric(const char *s) = 0;
	virtual int GetPlayerUserId(edict_t *e) = 0; // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	virtual void BuildSoundMsg(edict_t *entity, int channel, const char *sample, /*int*/ float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) = 0;
	virtual int IsDedicatedServer(void) = 0; // is this a dedicated server?
	virtual unsigned int GetPlayerWONId(edict_t *e) = 0; // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	virtual void Info_RemoveKey(char *s, const char *key) = 0;
	virtual const char *GetPhysicsKeyValue(const edict_t *pClient, const char *key) = 0;
	virtual void SetPhysicsKeyValue(const edict_t *pClient, const char *key, const char *value) = 0;
	virtual const char *GetPhysicsInfoString(const edict_t *pClient) = 0;
	virtual unsigned short PrecacheEvent(int type, const char *psz) = 0;
	virtual void PlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;
	virtual unsigned char *SetFatPVS(const float *org) = 0;
	virtual unsigned char *SetFatPAS(const float *org) = 0;
	virtual int CheckVisibility(const edict_t *entity, unsigned char *pset) = 0;
	virtual void DeltaSetField(struct delta_s *pFields, const char *fieldname) = 0;
	virtual void DeltaUnsetField(struct delta_s *pFields, const char *fieldname) = 0;
	virtual void DeltaAddEncoder(char *name, void (*conditionalencode)(struct delta_s *pFields, const unsigned char *from, const unsigned char *to)) = 0;
	virtual int GetCurrentPlayer(void) = 0;
	virtual int CanSkipPlayer(const edict_t *player) = 0;
	virtual int DeltaFindField(struct delta_s *pFields, const char *fieldname) = 0;
	virtual void DeltaSetFieldByIndex(struct delta_s *pFields, int fieldNumber) = 0;
	virtual void DeltaUnsetFieldByIndex(struct delta_s *pFields, int fieldNumber) = 0;
	virtual void SetGroupMask(int mask, int op) = 0;
	virtual int CreateInstancedBaseline(int classname, struct entity_state_s *baseline) = 0;
	virtual void ForceUnmodified(FORCE_TYPE type, float *mins, float *maxs, const char *filename) = 0;
	virtual void GetPlayerStats(const edict_t *pClient, int *ping, int *packet_loss) = 0;
	virtual qboolean Voice_GetClientListening(int iReceiver, int iSender) = 0;
	virtual qboolean Voice_SetClientListening(int iReceiver, int iSender, qboolean bListen) = 0;
	virtual const char *GetPlayerAuthId(edict_t *e) = 0;
	virtual int CheckParm(char *parm, char **ppnext) = 0;
};