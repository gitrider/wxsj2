
#include "stdafx.h"
#include "GCXinfaStudyInfo.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"

uint GCXinfaStudyInfoHandler::Execute( GCXinfaStudyInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//INT idMenpai = (INT)pPacket->GetMenpaiInfo_ID();

		////���ж������ʦ�ǲ����Լ������ɽ�ʦ
		//if(idMenpai != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MenPai())
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GCXinfaStudyInfohandler_Info_Not_Join_Menpai");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��û�м����ҵ����ɣ�");
		//	return PACKET_EXE_CONTINUE ;
		//}
		//INT nObjId = (INT)pPacket->GetTeacherId();
		//CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nObjId);

		//if(pObj)
		//{
		//	CDataPool::GetMe()->StudyXinfa_SetXinfaTeacherId(pObj->GetServerID());
		//}
		//
		//CDataPool::GetMe()->StudyXinfa_SetMenpaiInfo( idMenpai );
		//CEventSystem::GetMe()->PushEvent(GE_TOGLE_SKILLSTUDY,pObj?pObj->GetID():-1);
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
