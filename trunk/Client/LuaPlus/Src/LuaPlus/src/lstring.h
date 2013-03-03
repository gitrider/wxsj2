/*
** $Id: lstring.h,v 1.1 2007/09/26 03:09:36 zyh Exp $
** String table (keep all strings handled by Lua)
** See Copyright Notice in lua.h
*/

#ifndef lstring_h
#define lstring_h


#include "lobject.h"
#include "lstate.h"



#define sizestring(l)	(cast(lu_mem, sizeof(union TString))+ \
                         (cast(lu_mem, l)+1)*sizeof(char))
#define sizewstring(l)	(cast(lu_mem, sizeof(union TString))+ \
                         (cast(lu_mem, l)+1)*sizeof(lua_WChar))

#define sizeudata(l)	(cast(lu_mem, sizeof(union Udata))+(l))

#define luaS_new(L, s)	(luaS_newlstr(L, s, strlen(s)))
#define luaWS_new(L, s)	(luaS_newlwstr(L, s, wcslen(s)))
#define luaS_newliteral(L, s)	(luaS_newlstr(L, "" s, \
                                 (sizeof(s)/sizeof(char))-1))

#define luaS_fix(s)	((s)->tsv.marked |= (1<<4))

void luaS_resize (lua_State *L, int newsize);
Udata *luaS_newudata (lua_State *L, size_t s);
void luaS_freeall (lua_State *L);
TString *luaS_newlstr (lua_State *L, const char *str, size_t l);
TString *luaS_newlwstr (lua_State *L, const lua_WChar *str, size_t l);

#endif
