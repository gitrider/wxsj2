
/** GIScriptSystem.h
 */

#pragma once


#include "GINode.h"



namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class tScriptEnvironment
{
public:

	//-----------------------------------------------------------------
	//���ⲿ����
	
	//�õ�����
	virtual LPCTSTR GetName(VOID) const = 0;
	//ִ���ļ�
	virtual	INT	DoFile(LPCTSTR szScriptFile) = 0;
	//ִ�к�������
	virtual VOID DoFunction(LPCTSTR szFunctionName, LPCTSTR szParam=NULL) = 0;
	//ִ���ַ���
	virtual VOID DoString(LPCTSTR szString)	= 0;
	//�õ���������
	virtual LuaPlus::LuaObject*	GetLuaObject(VOID) = 0;

};


class KYLIN_API tScriptSystem : public tNode
{
public:

	//ȡ��Lua״̬
	virtual LuaPlus::LuaState* GetLuaState(VOID) = 0;
	//�����µĽű�����
	virtual	tScriptEnvironment* NewEnvironment(LPCTSTR szName, LPCTSTR szFileName) = 0;
	//���ٻ���
	virtual VOID DestroyEnvironment(tScriptEnvironment* pEnv) = 0;
	virtual tScriptEnvironment* GetEnvironmentByName(const LPCTSTR szName ) =0;

protected:

	KL_DECLARE_DYNAMIC( tScriptSystem );

};

