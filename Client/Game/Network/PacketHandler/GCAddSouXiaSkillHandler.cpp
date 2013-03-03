#include "StdAfx.h"
#include "GCSkillUpgradeBySouXia.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMGameInterface_Script_SouXia.h"

using namespace Packets;

UINT GCAddSouXiaSkillHandler::Execute(GCAddSouXiaSkill* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		switch( pPacket->GetResult() )
		{
		case SOUXIA_SKILL_UPGRADE_SUCCESS:	//学习成功
			{
				SHORT index = pPacket->GetCurSouXiaPos();		//获得该搜侠装备的 捜侠录的位置
				BYTE type = pPacket->GetSkillType();			//当前打开搜侠录的页类型

				SOUXIA_DATA* data = CDataPool::GetMe()->UserSouXia_GetInfo( index );
				int num = 0;//当前该类型的技能已经学了有多少个
				if( !data )
				{
					data = new SOUXIA_DATA;
					CDataPool::GetMe()->UserSouXia_SetInfo( index, data );
				}

				switch( type )
				{
				case 1:			//技能
					{
						num = data->GetCurSkillCount();
						data->m_Skill.StudySkillId[num] = pPacket->GetSkillID();
						data->m_Skill.StudyCount += 1;
					}
					break;

				case 2:			//武器配方
					{
						num = data->GetCurProductCount();
						data->m_Product.StudyProductId[num] = pPacket->GetSkillID();
						data->m_Product.StudyCount += 1;
					}
					break;

				case 3:			//神兽召唤
					{
						num = data->GetCurPetCount();
						data->m_Pet.StudyPet[num].StudyZhaoHuan = pPacket->GetSkillID();
						data->m_Pet.StudyPet[num].LeftUseTime = pPacket->GetCurUseTime();
						data->m_Pet.StudyCount += 1;
					}
					break;

				case 4:			//坐骑召唤
					{
						num = data->GetCurZuoJiCount();
						data->m_ZuoJi.StudyZuoji[num].StudyZhaoHuan = pPacket->GetSkillID();
						data->m_ZuoJi.StudyZuoji[num].LeftUseTime = pPacket->GetCurUseTime();
						data->m_ZuoJi.StudyCount += 1;
					}
					break;
				}
				
				SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.UpdateMainMenuBarSouxiaSkillIDNum( pPacket->GetSkillID(), 
					SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiaSkillNum(pPacket->GetSkillID()) );

				CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_STUDY, type );
			}
			break;

		case SOUXIA_SKILL_UPGRADE_FAIL:	//学习失败
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_UPGRADE_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_SKILL_STUDY_TYPE_ERROR:	//学习类型错误
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_STUDY_TYPE_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_SKILL_FULL:	//技能满了
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PRODUCT_FULL:		//配方满了
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PRODUCT_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PET_ZHAOHUAN_FULL:		//神兽召唤技能满了
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PET_ZHAOHUAN_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_ZUOJI_ZHAOHUAN_FULL:		//坐骑召唤技能满了
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_ZUOJI_ZHAOHUAN_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		}
		
		
	}

	return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}