#include "StdAfx.h"
#include "GCPrivateInfo.h"


#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "object/Logic/character/obj_playermyself.h"

uint GCPrivateInfoHandler::Execute( GCPrivateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//�����ݴ����ͻ���

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if(pPacket->GetSelf())  //�Լ���
		{
			CDataPool::GetMe()->PI_SetMyAge((INT)pPacket->GetAge());
			CDataPool::GetMe()->PI_SetMyBloodType((INT)pPacket->GetBloodType());
			CDataPool::GetMe()->PI_SetMyCharGUID(pPacket->GetCharGUID());
			CDataPool::GetMe()->PI_SetMyConsella(pPacket->GetConsella());
			CDataPool::GetMe()->PI_SetMyInfoType(pPacket->GetInfoType());
			CDataPool::GetMe()->PI_SetMySex(pPacket->GetSex());
			CDataPool::GetMe()->PI_SetMyYearAnimal(pPacket->GetYearAnimal());
			CDataPool::GetMe()->PI_SetMyProvince(pPacket->GetProvince());

			CDataPool::GetMe()->PI_SetMyCity(pPacket->GetCity());
			CDataPool::GetMe()->PI_SetMyEmailInfo(pPacket->GetEmailInfo());
			CDataPool::GetMe()->PI_SetMyJobInfo(pPacket->GetJobInfo());
			CDataPool::GetMe()->PI_SetMyLuckWord(pPacket->GetLuckWord());
			CDataPool::GetMe()->PI_SetMySchoolInfo(pPacket->GetSchoolInfo());

			CDataPool::GetMe()->PI_SetObjId(CObjectManager::GetMe()->GetMySelf()->GetID());
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PRIVATE_INFO,"self");
		}
		else //���˵�
		{
			CDataPool::GetMe()->PI_SetOtAge((INT)pPacket->GetAge());
			CDataPool::GetMe()->PI_SetOtBloodType((INT)pPacket->GetBloodType());
			CDataPool::GetMe()->PI_SetOtCharGUID(pPacket->GetCharGUID());
			CDataPool::GetMe()->PI_SetOtConsella(pPacket->GetConsella());
			CDataPool::GetMe()->PI_SetOtInfoType(pPacket->GetInfoType());
			CDataPool::GetMe()->PI_SetOtSex(pPacket->GetSex());
			CDataPool::GetMe()->PI_SetOtYearAnimal(pPacket->GetYearAnimal());
			CDataPool::GetMe()->PI_SetOtProvince(pPacket->GetProvince());

			CDataPool::GetMe()->PI_SetOtCity(pPacket->GetCity());
			CDataPool::GetMe()->PI_SetOtEmailInfo(pPacket->GetEmailInfo());
			CDataPool::GetMe()->PI_SetOtJobInfo(pPacket->GetJobInfo());
			CDataPool::GetMe()->PI_SetOtLuckWord(pPacket->GetLuckWord());
			CDataPool::GetMe()->PI_SetOtSchoolInfo(pPacket->GetSchoolInfo());

			//���յ����˵����ݲ������̴򿪽��棬�ȴ��������ȥ��
		}
	}
	

	
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
