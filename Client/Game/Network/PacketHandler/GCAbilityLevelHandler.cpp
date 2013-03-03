#include "StdAfx.h"
#include "GCAbilityLevel.h"
#include "GIUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"

uint GCAbilityLevelHandler::Execute( GCAbilityLevel* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);

		//--------------------------------------------------------------
		//生活技能刷新
/*
		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_ABILITY;
		cmdTemp.m_adwParam[0]	= 2;
		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->getAbilityId());
		cmdTemp.m_adwParam[2]	= (UINT)(pPacket->getLevel());
		pDataPool->OnCommand_( &cmdTemp );
*/

		switch ( pPacket->GetResult() )
		{
		case ABILITY_UPGRADE_SUCCESS:	//成功
			{
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_LifeAbility(
					pPacket->GetAbilityId(), pPacket->GetLevel(), -1);

				//同时修改数据池中的数据，为再次升级准备

				//策划说，只有升级没降级，所以只要收到就播放。
				CSoundSystemFMod::_PlayUISoundFunc(66);

				CHAR szText[128];
				sprintf(szText, "%d;0", (INT)(BASE_ACTION_ABILITY_LEVEL_UP));
				CObjectManager::GetMe()->GetMySelf()->SetChatMoodAction(STRING(szText));

				//填加提示信息
				BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
				if(bLog && CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pPacket->GetAbilityId()))
				{
					LPCTSTR pAbilityName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(
						pPacket->GetAbilityId())->m_pDefine->szName;
					UINT level = pPacket->GetLevel();
					STRING strTemp = COLORMSGFUNC("skill_levle_up", pAbilityName, level);
					ADDTALKMSG(strTemp);
				}

				//通知相关界面(通知审计界面和生活技能机面)
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_LIFESKILL_PAGE);
			}
			break;

		case ABILITY_UPGRADE_NPC_ERROR:		//NPC错误
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_STUDY_TYPE_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case ABILITY_UPGRADE_NPC_DISTANCE_FAR:		//距离NPC太远
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ABILITY_UPGRADE_NPC_DISTANCE_FAR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case ABILITY_UPGRADE_MONEY_ERROR:			//金钱不足失败
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ABILITY_UPGRADE_MONEY_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case ABILITY_UPGRADE_EXP_ERROR:		//玩家经验不足失败
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ABILITY_UPGRADE_EXP_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case ABILITY_UPGRADE_SKILL_EXP_ERROR:		//技能经验失败
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ABILITY_UPGRADE_SKILL_EXP_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case ABILITY_UPGRADE_HUMAN_LEVEL_ERROR:		//等级不足
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ABILITY_UPGRADE_HUMAN_LEVEL_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
