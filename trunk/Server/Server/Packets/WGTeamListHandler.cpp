/********************************************************************************
 *	�ļ�����	WGTeamListHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGTeamListHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 1 �� 16 ��	10:10
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGTeamList.h"
#include "GCTeamList.h"
#include "GCNotifyTeamInfo.h"

#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

uint WGTeamListHandler::Execute( WGTeamList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if ( pScene==NULL )
	{ // ��Ҫ����Ҷ���
		// Assert( pScene );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamListHandler: ServerPlayer (GUID=%X, TeamID=%d) ",
			pHuman->GetGUID(), pPacket->GetTeamID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
		if ( pTeamInfo->HasTeam() )
		{ // ֻҪ�ͻ���������ɵĶ�����Ϣ������շ������˵Ķ����б�
			pTeamInfo->DisMiss();
		}

		pTeamInfo->SetTeamID( pPacket->GetTeamID() );
		pHuman->GetDB()->SetDBTeamID( pPacket->GetTeamID() ) ;
		pTeamInfo->SetMySceneID( pScene->SceneID() );

		GCTeamList Msg;
		Msg.SetTeamID( pPacket->GetTeamID() );

		for( UINT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			const TEAM_LIST_ENTRY& entry = pPacket->GetTeamMember( i );
			TEAM_LIST_ENTRY newEntry;

			TEAMMEMBER Member;
			Member.m_GUID = entry.GetGUID();
			Member.m_SceneID = entry.GetSceneID();
			if ( Member.m_GUID == pGamePlayer->m_HumanGUID )
			{
				Member.m_ObjID = pHuman->GetID();
			}
			else if ( Member.m_SceneID == pScene->SceneID() )
			{
				GamePlayer* ptempGamePlayer = g_pPlayerPool->GetPlayer( entry.GetExtraID() );
				Obj_Human* ptempHuman;

				if ( ptempGamePlayer == NULL )
				{
					Assert(FALSE && "����ʧȥ�����ӡ�");
					Member.m_ObjID = INVALID_ID;
				}
				else if ( (ptempHuman = ptempGamePlayer->GetHuman()) == NULL )
				{
					Assert(FALSE && "�����Ҳ����ˡ���");
					Member.m_ObjID = INVALID_ID;
				}
				else
				{
					Member.m_ObjID = ptempHuman->GetID();
				}
			}
			else
			{
				Member.m_ObjID = INVALID_ID;
			}

			newEntry = entry;
			newEntry.SetSceneResID( g_pSceneManager->GetSceneResID(newEntry.GetSceneID()) );
			newEntry.SetExtraID( Member.m_ObjID );

			pTeamInfo->AddMember( &Member );
			Msg.AddTeamMember( newEntry );
		}

		pGamePlayer->SendPacket( &Msg );

		GCNotifyTeamInfo notifyMsg;

		notifyMsg.SetObjID( pHuman->GetID() );
		notifyMsg.SetHaveTeamFlag( pTeamInfo->HasTeam() );

		if ( pTeamInfo->HasTeam() )
		{
			notifyMsg.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
			notifyMsg.SetTeamFullFlag( pTeamInfo->IsFull() );
		}
		else
		{
			notifyMsg.SetTeamLeaderFlag( FALSE );
			notifyMsg.SetTeamFullFlag( FALSE );
		}

		if ( pHuman->getZoneID() != INVALID_ID )
		{
			pScene->BroadCast( &notifyMsg, pHuman );
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamListHandler: GamePlayer (to:%d, GUID=%ld, TeamID=%d) ",
			pPacket->GetPlayerID(), pHuman->GetGUID(), pPacket->GetTeamID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
