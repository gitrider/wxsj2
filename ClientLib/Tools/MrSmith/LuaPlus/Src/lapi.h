/*
** $Id: lapi.h,v 1.1 2006/03/17 08:47:34 chaojin Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h


#include "lobject.h"


void luaA_pushobject (lua_State *L, const TObject *o);

#endif
