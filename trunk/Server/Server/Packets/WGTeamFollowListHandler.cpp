/********************************************************************************
 *	�ļ�����	WGTeamFollowListHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGTeamFollowListHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	23:46
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGTeamFollowList.h"
#include "GCTeamFollowList.h"
#include "GCReturnTeamFollow.h"

#include "Log.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

uint WGTeamFollowListHandler::Execute( WGTeamFollowList* pPacket, Player* pPlayer )
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
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamFollowListHandler: ServerPlayer (GUID=%X) ",
			pHuman->GetGUID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
		if( pTeamInfo->HasTeam() == FALSE )
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		pHuman->__ClearFollowedMembers(); // ��վ���Ϣ

		_FOLLOWEDMEMBER myInfo;
		myInfo.m_GUID = pHuman->GetGUID();
		myInfo.m_pHuman = pHuman;

		for( INT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			_FOLLOWEDMEMBER info;
			const TEAMMEMBER* pMember;

			info.m_GUID = pPacket->GetFollowMember(i);
			info.m_pHuman = NULL;
			pMember = pTeamInfo->GetTeamMemberByGUID( info.m_GUID );

			if ( pMember->m_SceneID == pScene->SceneID() )
			{
				info.m_pHuman = pScene->GetHumanManager()->GetHuman( pMember->m_ObjID );

				if ( info.m_pHuman!=NULL )
				{
					if ( info.m_pHuman->GetGUID()!=pMember->m_GUID )
					{ // ��ȱ����
						info.m_pHuman = NULL;
					}
					else
					{ // �ø�����Ѱ��Լ��ӽ�ȥ���Ը��� pHuman ָ��
						info.m_pHuman->__AddFollowedMember( myInfo );
					}
				}
			}

			pHuman->__AddFollowedMember( info );
		}

		if ( pPacket->GetMemberCount()>0 )
		{
			pHuman->__SetTeamFollowFlag( TRUE );

			GCReturnTeamFollow Msg; // ֪ͨ�ͻ��˽������״̬
			Msg.SetGUID( pHuman->GetGUID() );
			Msg.SetReturn( TF_RESULT_FOLLOW_FLAG );
			pGamePlayer->SendPacket( &Msg );
		}
		else
		{ // �ӳ������˳���World ���ܷ���һ���յ��б����
			pHuman->__SetTeamFollowFlag( FALSE );
			pHuman->GetHumanAI()->PushCommand_StopTeamFollow();

			GCReturnTeamFollow Msg; // ֪ͨ�ͻ��˽������״̬
			Msg.SetGUID( pHuman->GetGUID() );
			Msg.SetReturn( TF_RESULT_STOP_FOLLOW );
			pGamePlayer->SendPacket( &Msg );
		}

		if ( pTeamInfo->IsLeader() )
		{ // �ӳ�
			GCTeamFollowList Msg;
			Msg.SetObjID( pHuman->GetID() );

			for( INT i=1; i<pHuman->__GetFollowedMembersCount(); ++i )
			{
				Obj_Human* pMember;

				pMember = pHuman->__GetFollowedMember(i)->m_pHuman;
				Msg.AddFollowMember( pHuman->__GetFollowedMember(i)->m_GUID );

				if ( pMember!=NULL && pMember->getZoneID()!=INVALID_ID )
				{ // ��������˳����Ÿ���
					pMember->GetHumanAI()->PushCommand_TeamFollow();
				}
			}

			if ( pHuman->getZoneID() != INVALID_ID )
			{
				pScene->BroadCast( &Msg, pHuman, TRUE );
			}
		}
		else if ( pPacket->GetMemberCount()>0 )
		{ // �Ǹ�����
			Obj_Human* pLeader = pHuman->__GetFollowedMember(0)->m_pHuman;

			if ( pLeader!=NULL && pLeader->getZoneID()!=INVALID_ID )
			{ // ��������˳����Ÿ���
				pHuman->GetHumanAI()->PushCommand_TeamFollow();
			}
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamFollowListHandler: GamePlayer (GUID=%X) ",
			pHuman->GetGUID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
