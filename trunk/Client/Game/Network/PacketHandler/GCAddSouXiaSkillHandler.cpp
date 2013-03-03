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
		case SOUXIA_SKILL_UPGRADE_SUCCESS:	//ѧϰ�ɹ�
			{
				SHORT index = pPacket->GetCurSouXiaPos();		//��ø�����װ���� ����¼��λ��
				BYTE type = pPacket->GetSkillType();			//��ǰ������¼��ҳ����

				SOUXIA_DATA* data = CDataPool::GetMe()->UserSouXia_GetInfo( index );
				int num = 0;//��ǰ�����͵ļ����Ѿ�ѧ���ж��ٸ�
				if( !data )
				{
					data = new SOUXIA_DATA;
					CDataPool::GetMe()->UserSouXia_SetInfo( index, data );
				}

				switch( type )
				{
				case 1:			//����
					{
						num = data->GetCurSkillCount();
						data->m_Skill.StudySkillId[num] = pPacket->GetSkillID();
						data->m_Skill.StudyCount += 1;
					}
					break;

				case 2:			//�����䷽
					{
						num = data->GetCurProductCount();
						data->m_Product.StudyProductId[num] = pPacket->GetSkillID();
						data->m_Product.StudyCount += 1;
					}
					break;

				case 3:			//�����ٻ�
					{
						num = data->GetCurPetCount();
						data->m_Pet.StudyPet[num].StudyZhaoHuan = pPacket->GetSkillID();
						data->m_Pet.StudyPet[num].LeftUseTime = pPacket->GetCurUseTime();
						data->m_Pet.StudyCount += 1;
					}
					break;

				case 4:			//�����ٻ�
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

		case SOUXIA_SKILL_UPGRADE_FAIL:	//ѧϰʧ��
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_UPGRADE_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_SKILL_STUDY_TYPE_ERROR:	//ѧϰ���ʹ���
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_STUDY_TYPE_ERROR");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_SKILL_FULL:	//��������
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PRODUCT_FULL:		//�䷽����
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PRODUCT_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PET_ZHAOHUAN_FULL:		//�����ٻ���������
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PET_ZHAOHUAN_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_ZUOJI_ZHAOHUAN_FULL:		//�����ٻ���������
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