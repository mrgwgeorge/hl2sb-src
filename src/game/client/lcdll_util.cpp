//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//

#define lcdll_util_cpp

#include "cbase.h"
#include "luamanager.h"
#include "lbaseplayer_shared.h"
#include "lbaseentity_shared.h"
#include "mathlib/lvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



static int luasrc_IsPlayerIndex (lua_State *L) {
  lua_pushboolean(L, IsPlayerIndex(luaL_checkinteger(L, 1)));
  return 1;
}

static int luasrc_GetLocalPlayerIndex (lua_State *L) {
  lua_pushinteger(L, GetLocalPlayerIndex());
  return 1;
}

static int luasrc_IsLocalPlayerSpectator (lua_State *L) {
  lua_pushboolean(L, IsLocalPlayerSpectator());
  return 1;
}

static int luasrc_GetSpectatorMode (lua_State *L) {
  lua_pushinteger(L, GetSpectatorMode());
  return 1;
}

static int luasrc_GetSpectatorTarget (lua_State *L) {
  lua_pushinteger(L, GetSpectatorTarget());
  return 1;
}

static int luasrc_GetLocalPlayerTeam (lua_State *L) {
  lua_pushinteger(L, GetLocalPlayerTeam());
  return 1;
}

static int luasrc_NormalizeAngles (lua_State *L) {
  QAngle *angles = luaL_checkangle(L, 1);
  NormalizeAngles(*(QAngle *)angles);
  lua_pushangle(L, angles);
  return 1;
}

static int luasrc_InterpolateAngles (lua_State *L) {
  QAngle output;
  InterpolateAngles(*(QAngle *)luaL_checkangle(L, 1), *(QAngle *)luaL_checkangle(L, 2), output, luaL_checknumber(L, 3));
  lua_pushangle(L, &output);
  return 1;
}

static int luasrc_ScreenHeight (lua_State *L) {
  lua_pushinteger(L, ScreenHeight());
  return 1;
}

static int luasrc_ScreenWidth (lua_State *L) {
  lua_pushinteger(L, ScreenWidth());
  return 1;
}

static int luasrc_UTIL_AngleDiff (lua_State *L) {
  lua_pushnumber(L, UTIL_AngleDiff(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}

static int luasrc_UTIL_Bubbles (lua_State *L) {
  UTIL_Bubbles(*(Vector *)luaL_checkvector(L, 1), *(Vector *)luaL_checkvector(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

static int luasrc_UTIL_ScreenShake (lua_State *L) {
  UTIL_ScreenShake(*(Vector *)luaL_checkvector(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), (ShakeCommand_t)luaL_checkinteger(L, 6), luaL_optboolean(L, 7, 0));
  return 0;
}

static int luasrc_UTIL_Tracer (lua_State *L) {
  UTIL_Tracer(*(Vector *)luaL_checkvector(L, 1), *(Vector *)luaL_checkvector(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checknumber(L, 5), luaL_checkboolean(L, 6), luaL_checkstring(L, 7));
  return 0;
}

static int luasrc_UTIL_Smoke (lua_State *L) {
  UTIL_Smoke(*(Vector *)luaL_checkvector(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  return 0;
}

static int luasrc_UTIL_SetOrigin (lua_State *L) {
  UTIL_SetOrigin(luaL_checkentity(L, 1), *(Vector *)luaL_checkvector(L, 2));
  return 0;
}

static int luasrc_UTIL_PrecacheOther (lua_State *L) {
  UTIL_PrecacheOther(luaL_checkstring(L, 1));
  return 0;
}

static int luasrc_ClientPrint (lua_State *L) {
  ClientPrint(luaL_checkplayer(L, 1), luaL_checkinteger(L, 2), luaL_checkstring(L, 3), luaL_optstring(L, 4, 0), luaL_optstring(L, 5, 0), luaL_optstring(L, 6, 0), luaL_optstring(L, 7, 0));
  return 0;
}

#define MAX_ENTITYARRAY 1024

static int luasrc_UTIL_EntitiesInBox (lua_State *L) {
  C_BaseEntity *pList[MAX_ENTITYARRAY];

  int count = UTIL_EntitiesInBox(pList, luaL_checkinteger(L, 1), *(Vector *)luaL_checkvector(L, 2), *(Vector *)luaL_checkvector(L, 3), luaL_checkinteger(L, 4), luaL_optint(L, 5, PARTITION_CLIENT_NON_STATIC_EDICTS));
  lua_pushinteger(L, count);
  lua_newtable(L);
  for( int i = 0 ; i < count ; i++ )
  {
	  lua_pushinteger(L, i);
	  lua_pushentity(L, pList[ i ]);
	  lua_settable(L, -3);
  }
  return 2;
}

static int luasrc_UTIL_EntitiesInSphere (lua_State *L) {
  C_BaseEntity *pList[MAX_ENTITYARRAY];

  int count = UTIL_EntitiesInSphere(pList, luaL_checkinteger(L, 1), *(Vector *)luaL_checkvector(L, 2), luaL_checknumber(L, 3), luaL_checkinteger(L, 4), luaL_optint(L, 5, PARTITION_CLIENT_NON_STATIC_EDICTS));
  lua_pushinteger(L, count);
  lua_newtable(L);
  for( int i = 0 ; i < count ; i++ )
  {
	  lua_pushinteger(L, i);
	  lua_pushentity(L, pList[ i ]);
	  lua_settable(L, -3);
  }
  return 2;
}

static int luasrc_UTIL_SafeName (lua_State *L) {
  lua_pushstring(L, UTIL_SafeName(luaL_checkstring(L, 1)));
  return 1;
}

static int luasrc_UTIL_BoundToWorldSize (lua_State *L) {
  Vector pVecPos = *(Vector *)luaL_checkvector(L, 1);
  UTIL_BoundToWorldSize(&pVecPos);
  lua_pushvector(L, &pVecPos);
  return 1;
}


static const luaL_Reg util_funcs[] = {
  {"IsPlayerIndex",  luasrc_IsPlayerIndex},
  {"GetLocalPlayerIndex",  luasrc_GetLocalPlayerIndex},
  {"IsLocalPlayerSpectator",  luasrc_IsLocalPlayerSpectator},
  {"GetSpectatorMode",  luasrc_GetSpectatorMode},
  {"GetSpectatorTarget",  luasrc_GetSpectatorTarget},
  {"NormalizeAngles",  luasrc_NormalizeAngles},
  {"InterpolateAngles",  luasrc_InterpolateAngles},
  {"ScreenHeight",  luasrc_ScreenHeight},
  {"ScreenWidth",  luasrc_ScreenWidth},
  // {"UTIL_AngleDiff",  luasrc_UTIL_AngleDiff},
  {"AngleDiff",  luasrc_UTIL_AngleDiff},
  // {"UTIL_Bubbles",  luasrc_UTIL_Bubbles},
  {"Bubbles",  luasrc_UTIL_Bubbles},
  // {"UTIL_ScreenShake",  luasrc_UTIL_ScreenShake},
  {"ScreenShake",  luasrc_UTIL_ScreenShake},
  // {"UTIL_Tracer",  luasrc_UTIL_Tracer},
  {"Tracer",  luasrc_UTIL_Tracer},
  // {"UTIL_Smoke",  luasrc_UTIL_Smoke},
  {"Smoke",  luasrc_UTIL_Smoke},
  // {"UTIL_SetOrigin",  luasrc_UTIL_SetOrigin},
  {"SetOrigin",  luasrc_UTIL_SetOrigin},
  // {"UTIL_PrecacheOther",  luasrc_UTIL_PrecacheOther},
  {"PrecacheOther",  luasrc_UTIL_PrecacheOther},
  {"ClientPrint",  luasrc_ClientPrint},
  // {"UTIL_EntitiesInBox",  luasrc_UTIL_EntitiesInBox},
  {"EntitiesInBox",  luasrc_UTIL_EntitiesInBox},
  // {"UTIL_EntitiesInSphere",  luasrc_UTIL_EntitiesInSphere},
  {"EntitiesInSphere",  luasrc_UTIL_EntitiesInSphere},
  // {"UTIL_SafeName",  luasrc_UTIL_SafeName},
  {"SafeName",  luasrc_UTIL_SafeName},
  // {"UTIL_BoundToWorldSize",  luasrc_UTIL_BoundToWorldSize},
  {"BoundToWorldSize",  luasrc_UTIL_BoundToWorldSize},
  {NULL, NULL}
};


int luaopen_UTIL (lua_State *L) {
  // luaL_register(L, "_G", util_funcs);
  luaL_register(L, "UTIL", util_funcs);
  lua_pop(L, 1);
  return 1;
}
