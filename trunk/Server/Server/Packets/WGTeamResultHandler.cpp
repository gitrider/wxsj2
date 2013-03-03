

#include "stdafx.h"
#include "WGTeamResult.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCTeamResult.h"
#include "GCNotifyTeamInfo.h"
#include "GCTeamMemberInfo.h"


uint WGTeamResultHandler::Execute( WGTeamResult* pPacket, Player* pPlayer )
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

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamResultHandler: ServerPlayer (Return=%d, GUID=%X, TeamID=%d) ",
			pPacket->GetReturn(), pPacket->GetGUID(), pPacket->GetTeamID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
		GCTeamResult tMsg;
		BOOL bNotifyTeamInfoFlag = FALSE;
		tMsg.SetReturn( pPacket->GetReturn() );
		tMsg.SetGUID( pPacket->GetGUID() );
		tMsg.SetTeamID( pPacket->GetTeamID() );
		tMsg.SetGUIDEx( pPacket->GetGUIDEx() );
		tMsg.SetSceneID( pPacket->GetSceneID() );
		tMsg.SetSceneResID( g_pSceneManager->GetSceneResID(pPacket->GetSceneID()) );		
		tMsg.SetIcon( pPacket->GetIcon() );
		tMsg.SetName( pPacket->GetName() );
		tMsg.SetDataID( pPacket->GetDataID() );
		tMsg.SetFamily( pPacket->GetFamily() );
		tMsg.SetLevel( pPacket->GetLevel() );

		switch( pPacket->GetReturn() )
		{
		case TEAM_RESULT_TEAMREFRESH:
		case TEAM_RESULT_MEMBERENTERTEAM:
			{
				if( !pTeamInfo->HasTeam() )
				{
					pTeamInfo->SetTeamID( pPacket->GetTeamID() );
					pHuman->GetDB()->SetDBTeamID( pPacket->GetTeamID() ) ;
					pTeamInfo->SetMySceneID( pScene->SceneID() );
					bNotifyTeamInfoFlag = TRUE;
				}

				TEAMMEMBER Member;
				Member.m_GUID = pPacket->GetGUID();
				Member.m_SceneID = pPacket->GetSceneID();

				if ( Member.m_GUID == pGamePlayer->m_HumanGUID )
				{
					Member.m_ObjID = pHuman->GetID();
				}
				else if ( Member.m_SceneID == pScene->SceneID() )
				{
					GamePlayer* ptempGamePlayer = g_pPlayerPool->GetPlayer(pPacket->GetGUIDEx());
					Obj_Human* ptempHuman;

					if ( ptempGamePlayer == NULL )
					{
						Assert(FALSE && "����ʧȥ�����ӡ�");
					}
					else if ( (ptempHuman = ptempGamePlayer->GetHuman()) == NULL )
					{
						Assert(FALSE && "�����Ҳ����ˡ���");
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

				tMsg.SetGUIDEx( Member.m_ObjID ); // �����ѵ� ObjID ����ȥ
				pTeamInfo->AddMember( &Member );

				if ( pTeamInfo->IsFull() )
				{
					bNotifyTeamInfoFlag = TRUE;
				}
			}
			break;
		case TEAM_RESULT_MEMBERLEAVETEAM:
		case TEAM_RESULT_TEAMKICK:
		case TEAM_RESULT_LEADERLEAVETEAM:
			{
				if ( pTeamInfo->IsFull() )
				{ // ���鲻����
					bNotifyTeamInfoFlag = TRUE;
				}

				if ( pGamePlayer->m_HumanGUID == pPacket->GetGUID() )
				{ // �Լ��������
					if ( pHuman->__GetTeamFollowFlag() )
					{
						pHuman->__StopTeamFollow(FALSE);
					}

					pTeamInfo->DisMiss();
					pHuman->GetDB()->SetDBTeamID( INVALID_ID ) ;

					bNotifyTeamInfoFlag = TRUE;
				}
				else
				{
					pTeamInfo->DelMember( pPacket->GetGUID() );

					if ( pPacket->GetReturn() == TEAM_RESULT_LEADERLEAVETEAM
					  && pTeamInfo->IsLeader() // �ӳ��˶Ӻ��Լ�����¶ӳ�
					  )
					{
						bNotifyTeamInfoFlag = TRUE;
					}
				}
			}
			break;
		case TEAM_RESULT_TEAMDISMISS:
			{
				if ( pTeamInfo->IsLeader() )
				{ // �Լ��Ƕӳ�
					if ( pHuman->__GetTeamFollowFlag() )
					{
						pHuman->__StopTeamFollow(FALSE);
					}
				}

				pTeamInfo->DisMiss();
				pHuman->GetDB()->SetDBTeamID( INVALID_ID ) ;
				bNotifyTeamInfoFlag = TRUE;
			}
			break;
		case TEAM_RESULT_TEAMAPPOINT:
			{
				if ( pTeamInfo->IsLeader() )
				{ // �Լ��Ƕӳ�
					if ( pHuman->__GetTeamFollowFlag() )
					{
						pHuman->__StopTeamFollow();
					}

					bNotifyTeamInfoFlag = TRUE;
				}

				pTeamInfo->Appoint( pPacket->GetGUIDEx() );

				if ( pTeamInfo->IsLeader() )
				{ // �Լ��Ƕӳ���
					bNotifyTeamInfoFlag = TRUE;
				}
			}
			break;
		case TEAM_RESULT_STARTCHANGESCENE:
			{
				pTeamInfo->StartChangeScene( pPacket->GetGUID() );
			}
			break;
		case TEAM_RESULT_ENTERSCENE:
			{
				if ( pPacket->GetGUID() == pGamePlayer->m_HumanGUID )
				{ // �Լ�����
					pTeamInfo->EnterScene( pGamePlayer->m_HumanGUID, pScene->SceneID(),
											pHuman->GetID() );

					if ( pHuman->__GetTeamFollowFlag() )
					{
						_FOLLOWEDMEMBER FollowedMember;

						FollowedMember.m_GUID = pHuman->GetGUID();
						FollowedMember.m_pHuman = pHuman;
						pHuman->__AddFollowedMember( FollowedMember );
					}
				}
				else
				{
					ObjID_t oid;

					if ( pPacket->GetSceneID() == pScene->SceneID() )
					{
						GamePlayer* ptempGamePlayer = g_pPlayerPool->GetPlayer(pPacket->GetGUIDEx());
						Obj_Human* ptempHuman;

						if ( ptempGamePlayer == NULL )
						{
							Assert(FALSE && "����ʧȥ�����ӡ�");
						}
						else if ( (ptempHuman = ptempGamePlayer->GetHuman()) == NULL )
						{
							Assert(FALSE && "�����Ҳ����ˡ���");
						}
						else
						{
							oid = ptempHuman->GetID();

							if ( ptempHuman->__GetTeamFollowFlag() )
							{
								_FOLLOWEDMEMBER FollowedMember;

								FollowedMember.m_GUID = ptempHuman->GetGUID();
								FollowedMember.m_pHuman = ptempHuman;
								pHuman->__AddFollowedMember( FollowedMember );
							}
						}
					}
					else
					{
						oid = INVALID_ID;
					}

					pTeamInfo->EnterScene( pPacket->GetGUID(), pPacket->GetSceneID(), oid );
					tMsg.SetGUIDEx( oid );
				}
			}
			break;
		case TEAM_RESULT_MEMBEROFFLINE:
			{
				pTeamInfo->MemberOffLine( pPacket->GetGUID() );

				GCTeamMemberInfo tmInfoMsg;
				tmInfoMsg.setGUID( pPacket->GetGUID() );
				tmInfoMsg.SetDeadLink( TRUE );

				pGamePlayer->SendPacket( &tmInfoMsg );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamResultHandler: GUID=%X has lost connection.",
					pPacket->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		default :
			{
				Assert(FALSE);
			}
			break;
		};

		pGamePlayer->SendPacket( &tMsg );

		if ( bNotifyTeamInfoFlag == TRUE )
		{
			GCNotifyTeamInfo notifyMsg;
			TeamInfo* pTeamInfo = pHuman->GetTeamInfo();

			notifyMsg.SetObjID( pHuman->GetID() );
			notifyMsg.SetHaveTeamFlag( pTeamInfo->HasTeam() );

			if ( pTeamInfo->HasTeam() )
			{
				notifyMsg.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
				notifyMsg.SetTeamFullFlag( pTeamInfo->IsFull() );
			//	notifyMsg.SetTeamFollowFlag( pHuman->__GetTeamFollowFlag() );
			}
			else
			{
				notifyMsg.SetTeamLeaderFlag( FALSE );
				notifyMsg.SetTeamFullFlag( FALSE );
			//	notifyMsg.SetTeamFollowFlag( FALSE );
			}

			if ( pHuman->getZoneID() != INVALID_ID )
			{
				pScene->BroadCast( &notifyMsg, pHuman );
			}

			g_pLog->FastSaveLog( LOG_FILE_1, "BroadCast GCNotifyTeamInfo..............." );
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamResultHandler: GamePlayer (Return=%d, to:%d, GUID=%u, TeamID=%d, GUIDEx=%u, SceneID=%d) ",
			pPacket->GetReturn(),pPacket->GetReturn(),pPacket->GetGUID(),pPacket->GetTeamID(), pPacket->GetGUIDEx(), pPacket->GetSceneID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
