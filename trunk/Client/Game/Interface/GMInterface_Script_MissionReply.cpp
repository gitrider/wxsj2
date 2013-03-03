
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Global.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Event/GMEventSystem.h"
#include "Network/NetManager.h"
#include "DataPool/GMUIDataPool.h"
#include "object/Logic/character/obj_playermyself.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Event/GMEventSystem.h"

#include "Action\GMActionSystem.h"

#include "CGMissionCheck.h"



namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	MissionReply MissionReply::s_MissionReply;
	LuaPlus::LuaObject* MissionReply::s_pMetaTable = NULL;

	// �õ�����Ƿ����
	INT MissionReply::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_MissionReply)
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

	// �򿪳����б�
	INT MissionReply::OpenPetFrame(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//���"Continue�Ĳ���"
	INT MissionReply::OnContinue(LuaPlus::LuaState* state)
	{
		// �����ݳ��л�ȡ���ݣ�����������Լ��϶�������ͷ����Ʒ�ڱ����е�������,
		// ���������������ȷ��,
		LuaStack args(state);
		INT nPetIndex;
		if (!(args[2].IsInteger()) ) 
		{
			nPetIndex = -1;
		}
		else
		{
			nPetIndex = args[2].GetInteger();	

			/*SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetIndex);
			if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
			{
				nPetIndex = -1;
			}*/
		}

		CEventSystem::GetMe()->PushEvent( GE_CLOSE_MISSION_REPLY );
		CUIDataPool::GetMe()->SendMissionItemInfo(nPetIndex);

		return 0;
	}
	//
	INT MissionReply::EnumItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		//������
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: MissionReply::EnumItem Wrong Param2");
		}
		INT nIndex = args[2].GetInteger();
		if(nIndex>=0 && nIndex<=3)
		{
			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_MISSIONREFER);
			if(pActionItem)
			{
				LuaObject objReturn = state->BoxPointer(pActionItem);
				objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
				objReturn.PushStack();

				return 1;
			}
		}
		//�Ƿ�ActionItem
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		return 1;
	}

}