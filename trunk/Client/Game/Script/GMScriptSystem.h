
/** �ű�ϵͳ������(LUA PLUS)
 */

#pragma once


#include "GIScriptSystem.h"
#include "LuaPlus.h"


//-----------------------------------------------------------------

class CScriptEnvironment : public tScriptEnvironment
{
public:

	// �õ�����
	virtual LPCTSTR GetName(VOID) const				{ return m_strName.c_str(); }
	// ִ���ļ�
	virtual	INT DoFile(LPCTSTR szScriptFile);
	// ִ������
	virtual VOID DoFunction(LPCTSTR szFunctionName, LPCTSTR szParam=NULL);
	// ִ���ַ���
	virtual VOID DoString(LPCTSTR szString);
	// �õ���������
	virtual LuaPlus::LuaObject*	GetLuaObject(VOID)	{ return &m_theEnvObj; }
protected:

	// �����쳣�Ľű�����
	void DoString_WithCPPException(LPCTSTR szString);
	void DoString_WithGenException(LPCTSTR szString);

protected:

	/// ������
	STRING				m_strName;
	/// ��lua�л���obj������
	STRING				m_strNameInLua;
	/// �ļ���
	STRING				m_strFileName;
	/// lua����
	LuaPlus::LuaObject	m_theEnvObj;

protected:

	CScriptEnvironment(LPCTSTR szName, LPCTSTR szFileName);
	virtual ~CScriptEnvironment();

	friend class CScriptSystem;

};


//-----------------------------------------------------------------

class CScriptSystem : public tScriptSystem
{
public:

	// ȡ��Lua״̬
	virtual LuaPlus::LuaState* GetLuaState(VOID)		{ return GetMe()->_GetLuaState(); }
	// �����µĽű�����
	virtual	tScriptEnvironment* NewEnvironment(LPCTSTR szName, LPCTSTR szFileName);
	// ���ٻ���
	virtual VOID DestroyEnvironment(tScriptEnvironment* pEnv);
	// ��Lua�쳣ʱ�Ƿ��׳�
	BOOL IsEnableLuaCrash(void) const					{ return m_bEnableLuaCrash; }
	// ���õ�ǰEnv
	void SetActiveEnvironment(LPCTSTR szEnvName);
	// ȡ�õ�ǰEnv
	STRING GetActiveEnvironment(void) const				{ return m_strEnvName; }

	virtual tScriptEnvironment* GetEnvironmentByName(const LPCTSTR szName );


	//================ �����б�

	// �õ�����ű��ļ���
	const STRING& GetQuestFile(int nID);

	// ȡ��Lua״̬(��̬)
	static LuaPlus::LuaState* _GetLuaState(void);

public:

	virtual VOID Initial(VOID*);
	virtual VOID Release(VOID);
	virtual VOID Tick(VOID);

protected:

	// ������������
	void _loadQuestFile(void);

protected:

	/// �ű���������
	std::list<CScriptEnvironment*>	m_listScriptEnv;
	/// ��Lua�쳣ʱ�Ƿ��׳�
	BOOL							m_bEnableLuaCrash;
	/// ��������ű��б�
	std::map<int, STRING>			m_mapQuestFile;
	/// ��ǰ�����Env����
	STRING							m_strEnvName;

public:

	CScriptSystem();
	virtual ~CScriptSystem();
	static CScriptSystem* GetMe(VOID)	{ return s_pMe; }

protected:

	static CScriptSystem* s_pMe;

	KL_DECLARE_DYNAMIC(CScriptSystem);

};