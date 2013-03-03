//-----------------------------------------------------------------------------
// �ļ��� : LuaFnRegister.h
// ģ��	:	Script
// ����	 :  ��Luaע��c��������
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------
#ifndef __LUACFUNREGISTER_H__
#define __LUACFUNREGISTER_H__

#include "Type.h"
#include "LuaLib.h"

class LuaInterface;

typedef INT (*FuncProto)(Lua_State * L);
#define REGISTERFUNCTION(funcname) {#funcname,FuncProto(funcname)},

struct _Str2Func
{
	CHAR* funcname;
	FuncProto proto;
};

class LuaCFuncRegister
{
private:
	LuaInterface* mOwner;
public:
	LuaCFuncRegister();
	~LuaCFuncRegister();

	VOID SetOwner(LuaInterface* pInter);
	LuaInterface* GetOwner();

#ifdef _DEBUG
	//CHAR* GetFuncName(CHAR* pStr1,CHAR* pStr2);
	VOID ValidateFuncs();
#endif

	VOID RegisterCFunction();
};

#endif