#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\event\GMEventSystem.h"

#include "GCTeamLeaderAskInvite.h"

uint GCTeamLeaderAskInviteHandler::Execute( GCTeamLeaderAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	

/*
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{ // ����ط�������ֱ���׳���Ϣ��û�еط��洢��Ϣ
		char szTemp[MAX_PATH] = {0};
		_snprintf(szTemp, MAX_PATH, "%s ϣ������ %s �������", pPacket->GetSourName(), pPacket->GetDestName());
		CGTeamLeaderRetInvite msg;
		msg.SetSourGUID( pPacket->GetSourGUID() );
		msg.SetDestGUID( pPacket->GetDestGUID() );

		if(IDYES == MessageBox(NULL, szTemp, "�������!!!", MB_YESNO|MB_ICONQUESTION))
		{
			msg.SetReturn(1);
		}
		else
		{
			msg.SetReturn(0);
		}

		CNetManager::GetMe()->SendPacket(&msg);
	}
*/
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//CUIDataPool::GetMe()->SetSourceGUID( pPacket->GetSourGUID() );
		//CUIDataPool::GetMe()->SetDestinationGUID( pPacket->GetDestGUID() );//

		INT iPos = CUIDataPool::GetMe()->SetMemberInviteGUID(pPacket->GetSourGUID(), pPacket->GetDestGUID());
		CEventSystem::GetMe()->PushEvent(GE_TEAM_MEMBER_INVITE, pPacket->GetSourName(), pPacket->GetDestName(), iPos); 

		return PACKET_EXE_CONTINUE;//
	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
