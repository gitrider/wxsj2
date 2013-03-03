
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "DataPool/GMDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "CGAskLeanAbility.h"



namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	AbilityTeacher AbilityTeacher::s_AbilityTeacher;
	LuaPlus::LuaObject* AbilityTeacher::s_pMetaTable = NULL;

	// �õ���ǰ��ʦ�Ƿ����
	INT AbilityTeacher::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_AbilityTeacher)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	// �õ���ǰ��ʦ���̵ܽ������ID
	INT AbilityTeacher::GetAbilityID(LuaPlus::LuaState* state)
	{
		INT nAbilityID = CDataPool::GetMe()->StudyAbility_GetID();

		state->PushInteger( nAbilityID );
		return 1;
	}

	// �õ���������һ������Ҫ�ľ���ֵ
	INT AbilityTeacher::GetNeedExp(LuaPlus::LuaState* state)
	{
		INT nExp = CDataPool::GetMe()->StudyAbility_GetNeedExp();

		state->PushInteger( nExp );
		return 1;
	}

	// �õ���������һ������Ҫ�Ľ�Ǯ
	INT AbilityTeacher::GetNeedMoney(LuaPlus::LuaState* state)
	{
		INT nMoney = CDataPool::GetMe()->StudyAbility_GetNeedMoney();
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}

	// �õ���������һ������Ҫ��������
	INT AbilityTeacher::GetNeedSkillExp(LuaPlus::LuaState* state)
	{
		INT nSkillExp = CDataPool::GetMe()->StudyAbility_GetSkillExp();

		state->PushInteger( nSkillExp );
		return 1;
	}

	// ��÷�����������һЩ����
	INT AbilityTeacher::GetServerData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsString()))
		{
			KLThrow("AbilityTeacher::GetServerData[2] param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str == "npcid")
		{
			state->PushInteger(CDataPool::GetMe()->StudyAbility_GetNpcId());
		}
		else if(str == "scriptid")
		{
			state->PushInteger(CDataPool::GetMe()->StudyAbility_GetScriptId());
		}

		return 1;
	}
};