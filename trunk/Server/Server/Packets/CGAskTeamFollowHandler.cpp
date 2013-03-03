/********************************************************************************
 *	�ļ�����	CGAskTeamFollowHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\CGAskTeamFollowHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 15 ��	18:07
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "CGAskTeamFollow.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ScanOperator_ActiveTeammates.h"
#include "GWEnterTeamFollow.h"
#include "GCAskTeamFollow.h"
#include "ServerManager.h"
#include "GCReturnTeamFollow.h"

uint CGAskTeamFollowHandler::Execute( CGAskTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	
	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	if ( pHuman->__GetTeamMemberCount() < 2 )
	{ // ����̫��
		return PACKET_EXE_CONTINUE;
	}

	if ( pHuman->__IsTeamLeader() == FALSE )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamFollowHandler: GUID=%X send a fake order.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	if ( pHuman->__GetTeamFollowFlag()==FALSE )
	{
		_FOLLOWEDMEMBER FollowedMember;

		FollowedMember.m_GUID = pHuman->GetGUID();
		FollowedMember.m_pHuman = pHuman;

		pHuman->GetHumanAI()->PushCommand_Idle(); // ��ֹ��ǰ�߼�
		pHuman->__SetTeamFollowFlag( TRUE );
		pHuman->__AddFollowedMember( FollowedMember );

		GCReturnTeamFollow Msg; // ֪ͨ�ͻ��˽������״̬
		Msg.SetGUID( pHuman->GetGUID() );
		Msg.SetReturn( TF_RESULT_FOLLOW_FLAG );
		pGamePlayer->SendPacket( &Msg );

		// ������Ҫ֪ͨ world GWEnterTeamFollow
		GWEnterTeamFollow* pMsg = (GWEnterTeamFollow*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_ENTER_TEAMFOLLOW);

		pMsg->SetGUID( pHuman->GetGUID() );

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}

	ScanOperator_ActiveTeammates ScanOp;
	SCANOPERATOR_ACTIVETEAMMATES_INIT init;

	init.m_bScanHuman = TRUE;
	init.m_fRadius = (FLOAT)g_Config.m_ConfigInfo.m_nAvailableFollowDist;
	init.m_MemberGUID = pHuman->GetGUID();
	init.m_nZoneRadius = (INT)init.m_fRadius/g_Config.m_ConfigInfo.m_ZoneSize + 1;
	init.m_Position = *(pHuman->__GetWorldPos());
	init.m_pScene = pScene;
	init.m_TeamID = pHuman->__GetTeamID();
	init.m_ZoneID = pHuman->getZoneID();

	ScanOp.Init( &init );

	if ( pScene->Scan( &ScanOp ) )
	{
		if ( ScanOp.m_nActiveTeammateCount>1 )
		{ // ���� 1 ��ʱ���Ƕӳ��Լ�
			for( INT i=0; i<ScanOp.m_nActiveTeammateCount; ++i )
			{
				if ( ScanOp.m_aActiveTeammate[i]==NULL )
				{
					Assert(FALSE);
					continue;
				}

				if ( !ScanOp.m_aActiveTeammate[i]->IsAlive() )
				{ // ����
					continue;
				}

				if ( ScanOp.m_aActiveTeammate[i]->__GetTeamFollowFlag() )
				{ // �Ѿ��������ģʽ
					continue;
				}

				// �����Ա���ڰ�̯״̬��������
				if( ScanOp.m_aActiveTeammate[i]->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
				{
					continue;
				}

				// ���ͻ��˷������� GCAskTeamFollow
				GCAskTeamFollow Msg;
				ScanOp.m_aActiveTeammate[i]->GetPlayer()->SendPacket( &Msg );
			}
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamFollowHandler: GUID=%X", 
		pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
