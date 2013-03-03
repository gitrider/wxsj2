//* 			������Ӱ���ϵͳ			*

#pragma once
#include "GIHelperSystem.h"
#include "..\Interface\GMinterface_Lua.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CHelperSystem : public tHelperSystem, public LuaExport< CHelperSystem >
{
	//-----------------------------------------------------------
	//����ӿ�
	//-----------------------------------------------------------
public:
	// ��ת��ָ����������
	void	GotoAnchorHelp(const STRING& strAnchor);

	//���ش˷��������а�ר�� tongxi
	void	GotoAnchorHelp();

	//-----------------------------------------------------------
	// ������Lua�ĺ���
	//-----------------------------------------------------------
public:
	INT Lua_GotoHelper(LuaPlus::LuaState* state);


	INT Lua_GotoPaihang(LuaPlus::LuaState* state);
protected:
	//������������
	void	CreateHelperProcess(const STRING& strAnchor);

	//-----------------------------------------------------------
	//���ļ̳�
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

protected:
	//��������������ܴ�����
	STRING	m_strCmdWnd;
	//��������
	HANDLE	m_hHelperProcess;

public:
	CHelperSystem();
	virtual ~CHelperSystem();

protected:
	KL_DECLARE_DYNAMIC(CHelperSystem);
};
