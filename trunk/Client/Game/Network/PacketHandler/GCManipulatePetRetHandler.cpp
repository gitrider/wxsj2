
#include "stdafx.h"
#include "GCManipulatePetRet.h"
#include "..\..\DataPool\GMDP_Struct_Pet.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Global.h"
#include "../../Event/GMEventSystem.h"
#include "../../DataPool/GMUIDataPool.h"


UINT GCManipulatePetRetHandler::Execute( GCManipulatePetRet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{	
		switch (pPacket->GetManipulateRet())
		{
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTUREFALID:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Arrest_Fail");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��׽ʧ��");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTURESUCC:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Arrest__Ok");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��׽�ɹ�");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Call_Fail");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ٻ�ʧ��");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPSUCC:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Call_Ok");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ٻ��ɹ�");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLFALID:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_CallUp_Failed");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLSUCC:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_CallUp_Success");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREEFALID:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Free_Failed");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREESUCC:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Free_Success");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
			// �򿪽������ɹ�
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_OK:
			{
				INT nNPCSeverID = pPacket->GetObjID();
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_MOUNT_MATING_PAGE, nNPCSeverID);
			}
			break;
			// �򿪽������ʧ��
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL:		
			{				
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���ﷱֳ���濪��ʧ��");
			}
			break;
			// �������������ɹ�
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_OK:
			{
				// �Լ�����
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 1);
			}
			break;
			// ������������ʧ��
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_FAIL:
			{
				// �Լ�����
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// �����������
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_UNLOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// �Է�������������
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_LOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_OTHER_MOUNT_MATING_LOCKED, 1);
			}
			break;			
			// �Է������������
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_UNLOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_OTHER_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// ���ｻ��ȷ�ϳɹ�
		case GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS:
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_MOUNT_MATING_PAGE);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���ｻ��ɹ�");
			}
			break;
			// ���ｻ��ȷ��ʧ��
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CONFIRM_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���ｻ��ȷ��ʧ��");
			}
			break;
			// ���ｻ��ȡ��
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CANCEL:
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_MOUNT_MATING_PAGE);
			}
			break;
			// ������ȡ�ɹ�
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_RETRIEVE:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����Ѿ���ȡ");
			}
			break;

		
		case GCManipulatePetRet::MANIPULATEPET_RET_NO_TEAM:					// û�����
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "û�����");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_TEAM_MEM_COUNT:			// ������������2��, �����˲���ͬһ������
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������������2��, �����˲���ͬһ������");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_TEAM_LEADER:				// ���Ƕӳ�
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�����Ѿ���ȡ");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_AREA:						// ��NPC���벻��
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��NPC���벻��");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LOCK:				// ���뽻�����������״̬,���ܽ���
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�����������״̬,���ܽ���");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SEX_SAME:			// ���뽻������Ա���ͬ
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻������Ա���ͬ");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_HAPPINESS:			// ���뽻�������ֶȲ���100
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�������ֶȲ���100");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_GENERATION: 		// ���뽻�������һ����
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�������һ����");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL:			// ���뽻�����ȼ����5
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�����ȼ����5");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSED:			// ���뽻���������ż�Ҳ��ǶԷ�
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻���������ż�Ҳ��ǶԷ�");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SCENE:				// ���뽻��������˲���ͬһ������
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻��������˲���ͬһ������");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL15:			// ���뽻�����ȼ��������15��
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�����ȼ��������15��");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_CALLUP:				// ���뽻��������ǳ�ս״̬
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻��������ǳ�ս״̬");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_VARIATION:			// ���뽻��������Ǳ���
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻��������Ǳ���");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATINGLEVEL:		// ���뽻�����ȼ����ϴν�������20��
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�����ȼ����ϴν�������20��");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING:				// ���뽻�������������������ڽ���
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���뽻�������������������ڽ���");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSE:				// ��û����������ﷱֳ
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��û����������ﷱֳ");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_FINISH:		//��˻�δ��ֳ���
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��˻�δ��ֳ���");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_NO_SOLT:			// û�пո�����
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "û�пո�����");
			break;
		default :
			break;
		}
		//CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
