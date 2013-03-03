///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif _MSC_VER
#ifndef LUAOBJECT_H
#define LUAOBJECT_H

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

class LuaCall;
struct LuaArgNil {};

struct LuaRun
{
	LuaRun(int numResults = -1) throw()
	{
		m_numResults = numResults;
	}

	int m_numResults;
};

/**
	Representation of a Lua object.
**/
class LUAPLUS_CLASS LuaObject
{
public:
	LuaObject();
	LuaObject(LuaState* state) throw();
	LuaObject(LuaState* state, int stackIndex) throw();
	LuaObject(LuaState* state, const lua_TObject* obj);
	LuaObject(const LuaObject& src) throw();
	LuaObject(const LuaStackObject& src) throw();
	LuaObject& operator=(const LuaObject& src) throw();
	LuaObject& operator=(const LuaStackObject& src) throw();

/*	template <typename T>
	LuaObject& operator=(const T& value)
	{
		assert(m_state);
		LCD::Assign(*this, value);
		return *this;
	}*/

	~LuaObject();

	void Reset();

	/**
		Retrieves the LuaState object associated with this LuaObject.
	**/
	LuaState* GetState() const;
	lua_State* GetCState() const;
	operator lua_State*() const;
	operator LuaState*() const;

	bool operator==(const LuaObject& right) const;
	bool operator<(const LuaObject& right) const;

	const char* TypeName() const;
	int Type() const;

	bool IsNil() const;
	bool IsTable() const;
	bool IsUserData() const;
	bool IsCFunction() const;
	bool IsInteger() const;
	bool IsNumber() const;
	bool IsString() const;
	bool IsWString() const;
	bool IsConvertibleToInteger() const;
	bool IsConvertibleToNumber() const;
	bool IsConvertibleToString() const;
	bool IsConvertibleToWString() const;
	bool IsFunction() const;
	bool IsNone() const;
	bool IsLightUserData() const;
	bool IsBoolean() const;

	int ToInteger();
	lua_Number ToNumber();
	const char* ToString();
	const lua_WChar* ToWString();
	size_t ToStrLen();

	int GetInteger() const;
	float GetFloat() const;
	double GetDouble() const;
	lua_Number GetNumber() const;
	const char* GetString() const;
	const lua_WChar* GetWString() const;
	int StrLen();
	lua_CFunction GetCFunction() const;
	void* GetUserData();
	const void* GetLuaPointer();
	void* GetLightUserData() const;
	bool GetBoolean() const;

	LuaStackObject PushStack();

	LuaObject GetMetaTable();
	void SetMetaTable(const LuaObject& valueObj);

	int GetN();
	void SetN(int n);

	void Insert(LuaObject& obj);
	void Insert(int index, LuaObject& obj);
	void Remove(int index = -1);
	void Sort();

	int GetCount();
	int GetTableCount();

	LuaObject Clone();

	LuaObject CreateTable(const char* key, int narray = 0, int lnhash = 0);
	LuaObject CreateTable(int key, int narray = 0, int lnhash = 0);
	LuaObject CreateTable(LuaObject& key, int narray = 0, int lnhash = 0);

	LuaObject& SetNil(const char* key);
	LuaObject& SetNil(int key);
	LuaObject& SetNil(LuaObject& key);
	LuaObject& SetBoolean(const char* key, bool value);
	LuaObject& SetBoolean(int key, bool value);
	LuaObject& SetBoolean(LuaObject& key, bool value);
	LuaObject& SetInteger(const char* key, int value);
	LuaObject& SetInteger(int key, int value);
	LuaObject& SetInteger(LuaObject& key, int value);
	LuaObject& SetNumber(const char* key, lua_Number value);
	LuaObject& SetNumber(int key, lua_Number value);
	LuaObject& SetNumber(LuaObject& key, lua_Number value);
	LuaObject& SetString(const char* key, const char* value);
	LuaObject& SetString(int key, const char* value);
	LuaObject& SetString(LuaObject& key, const char* value);
	LuaObject& SetWString(const char* key, const lua_WChar* value);
	LuaObject& SetWString(int key, const lua_WChar* value);
	LuaObject& SetWString(LuaObject& key, const lua_WChar* value);
	LuaObject& SetUserData(const char* key, void* value);
	LuaObject& SetUserData(int key, void* value);
	LuaObject& SetUserData(LuaObject& key, void* value);
	LuaObject& SetLightUserData(const char* key, void* value);
	LuaObject& SetLightUserData(int key, void* value);
	LuaObject& SetLightUserData(LuaObject& key, void* value);
	LuaObject& SetObject(const char* key, LuaObject& value);
	LuaObject& SetObject(int key, LuaObject& value);
	LuaObject& SetObject(LuaObject& key, LuaObject& value);

	void AssignNil(LuaState* state);
	void AssignBoolean(LuaState* state, bool value);
	void AssignInteger(LuaState* state, int value);
	void AssignNumber(LuaState* state, lua_Number value);
	void AssignString(LuaState* state, const char* value);
	void AssignWString(LuaState* state, const lua_WChar* value);
	void AssignUserData(LuaState* state, void* value);
	void AssignLightUserData(LuaState* state, void* value);
	void AssignObject(LuaObject& value);		// Should this function be removed??
	void AssignNewTable(LuaState* state, int narray = 0, int numhash = 0);
	void AssignTObject(LuaState* state, lua_TObject* value);

	LuaObject GetByName(const char* name);
	LuaObject GetByIndex(int index);
	LuaObject GetByObject(const LuaStackObject& obj);
	LuaObject GetByObject(const LuaObject& obj);
	LuaObject operator[](const char* name);
	LuaObject operator[](int index);
	LuaObject operator[](const LuaStackObject& obj);
	LuaObject operator[](const LuaObject& obj);

	LuaObject Lookup(const char* key);

	void Register(const char* funcName, lua_CFunction func, int nupvalues = 0);

	void Register(const char* funcName, int (*func)(LuaState*), int nupvalues = 0);

	template <class Callee>
	void Register(const char* funcName, const Callee& callee, int (Callee::*func)(LuaState*), int nupvalues = 0)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(Callee) + sizeof(func));
		memcpy(buffer, &callee, sizeof(Callee));
		memcpy(buffer + sizeof(Callee), &func, sizeof(func));
		Register(funcName, LPCD::LuaStateMemberDispatcher<Callee>, nupvalues + 1);
	}
	
	template <class Callee>
	void RegisterObjectFunctor(const char* funcName, int (Callee::*func)(LuaState*), int nupvalues = 0)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(func));
		memcpy(buffer, &func, sizeof(func));
		Register(funcName, LPCD::Object_MemberDispatcher_to_LuaState<Callee>, nupvalues + 1);
	}

	template <typename Func>
	inline void RegisterDirect(const char* funcName, Func func, unsigned int nupvalues = 0)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(func));
		memcpy(buffer, &func, sizeof(func));
		Register(funcName, LPCD::DirectCallFunctionDispatchHelper<Func>::DirectCallFunctionDispatcher, nupvalues + 1);
	}

	template <typename Callee, typename Func>
	inline void RegisterDirect(const char* funcName, Callee& callee, Func func, unsigned int nupvalues = 0)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(Callee) + sizeof(func));
		memcpy(buffer, &callee, sizeof(Callee));
		memcpy(buffer + sizeof(Callee), &func, sizeof(func));
		Register(funcName, LPCD::DirectCallMemberDispatcherHelper<Callee, Func>::DirectCallMemberDispatcher, nupvalues + 1);
	}


	template <typename Callee, typename Func>
	inline void RegisterObjectDirect(const char* funcName, Callee* callee, Func func, unsigned int nupvalues = 0)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(func));
		memcpy(buffer, &func, sizeof(func));
		Register(funcName, LPCD::DirectCallObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues + 1);
	}


	void Unregister(const char* funcName);

	// Private functions and data
	void AddToUsedList(LuaState* state);
	void AddToUsedList(LuaState* state, const lua_TObject& obj);
	void RemoveFromUsedList();

	LuaObject* m_next;		   // only valid when in free list
	LuaObject* m_prev;		   // only valid when in used list
	LuaState* m_state;
	enum { TOBJECT_SIZE = 12 };
#ifdef BUILDING_LUAPLUS
	lua_TObject m_object;
#else BUILDING_LUAPLUS
	unsigned char m_object[TOBJECT_SIZE];
#endif BUILDING_LUAPLUS

protected:
	LuaObject& SetTableHelper(const char* key, lua_TObject& valueObj);
	LuaObject& SetTableHelper(int key, lua_TObject& valueObj);
	LuaObject& SetTableHelper(LuaObject& key, lua_TObject& valueObj);
};


class LuaCall
{
public:
	LuaCall(LuaObject& functionObj);

	LuaStackObject operator<<(const LuaRun& /*run*/);
	LuaCall& operator=(const LuaCall& src);

	LuaState* m_state;
	LuaObject m_functionObj;
	int m_numArgs;
	int m_startResults;
};


LuaCall& operator<<(LuaCall& call, const LuaArgNil& value);
LuaCall& operator<<(LuaCall& call, float value);
LuaCall& operator<<(LuaCall& call, double value);
LuaCall& operator<<(const LuaCall& call, int value);
LuaCall& operator<<(LuaCall& call, unsigned int value);
LuaCall& operator<<(LuaCall& call, const char* value);
LuaCall& operator<<(LuaCall& call, const lua_WChar* value);
LuaCall& operator<<(LuaCall& call, lua_CFunction value);
LuaCall& operator<<(LuaCall& call, int (*value)(LuaState*));
LuaCall& operator<<(LuaCall& call, bool value);
LuaCall& operator<<(LuaCall& call, void* value);
LuaCall& operator<<(LuaCall& call, LuaStackObject& value);
LuaCall& operator<<(LuaCall& call, LuaObject& value);

} // namespace LuaPlus

#include "LuaState.h"

#endif LUAOBJECT_H
