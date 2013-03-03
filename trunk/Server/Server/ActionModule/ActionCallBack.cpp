///////////////////////////////////////////////////////////////////////////////
// �ļ�����ActionCallBack.cpp
// ����Ա������
// ����˵�����ص�������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Type.h"
#include "ActionCallBack.h"
#include "ActionCallBackForScript.h"
#include "ActionCallBackForSkill.h"
namespace Action_Module
{
	//ȫ��Ψһ�Ķ����ص��߼�ʵ��
	ActionCallBackFunctorForSkill_T const g_ActionCallBackFunctorForSkill;
	ActionCallBackFunctorForScript_T const g_ActionCallBackFunctorForScript;
	
	ActionCallBackFunctor_T const& GetGlobalScriptCallBackFunctor(VOID)
	{
		return static_cast<ActionCallBackFunctor_T const&>(g_ActionCallBackFunctorForScript);
	}
	ActionCallBackFunctor_T const& GetGlobalSkillCallBackFunctor(VOID)
	{
		return static_cast<ActionCallBackFunctor_T const&>(g_ActionCallBackFunctorForSkill);
	}
	
}
