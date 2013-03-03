
#include "stdafx.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "Log.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "WGRelation.h"
#include "GCRelation.h"

uint WGRelationHandler::Execute( WGRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetRelation()->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL ) return PACKET_EXE_CONTINUE;

	WG_RELATION* pRelationPacket = pPacket->GetRelation();

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: ServerPlayer (Type=%d) ",
			pRelationPacket->m_Type );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCRelation Msg;
		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = pRelationPacket->m_Type;
		UCHAR uErr = 0;
		HumanRelation* pMyRelation = pHuman->GetHumanRelation();

		switch( pRelationPacket->m_Type )
		{
		case RET_RELATIONINFO:
			{ // ���� Server �Ļ��沢�����ͻ���
				_RELATION* pRelationData = pRelationPacket->m_RelationInfo.GetRelationData();
				const _OWN_RELATION* pRelation = pMyRelation->UpdateRelationInfo( pRelationData );
				RETURN_RELATION_INFO& RelationInfo = Msg.GetRelation()->m_RelationInfo;

				RelationInfo.CleanUp();

				RelationInfo.SetRelationData( pRelationData );

				// ������ ID ת���ɳ�����Դ ID
				ID_t SceneID = RelationInfo.m_Relation.GetSceneID();
				const _SCENE_DATA* pSceneData = g_pSceneManager->GetSceneInfo( SceneID );
				if ( pSceneData != NULL && pSceneData->m_Type == SCENE_TYPE_GAMELOGIC )
				{ // ֻ�д�����Ϸ�߼������Ÿ�������������ʾ
					SceneID = g_pSceneManager->GetSceneResID( SceneID );
				}
				else
				{
					SceneID = INVALID_ID;
				}

				RelationInfo.m_Relation.SetSceneID( SceneID );

				if( pRelation != NULL )
				{
					RelationInfo.SetRelationType( pRelation->m_Type );
					RelationInfo.SetGroup( pRelation->m_Group );
					RelationInfo.SetFriendPoint( pRelation->m_FriendPoint );
				}

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X ask info of GUID=%X.",
					pHuman->GetGUID(), pRelationData->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_VIEWPLAYER:
			{
				RETURN_VIEW_PLAYER& Send = Msg.GetRelation()->m_ViewPlayer;
				Send.CleanUp();

				Send.FillData( &(pRelationPacket->m_ViewPlayer) );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X View Player: %s.",
					pHuman->GetGUID(), Send.GetName() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_TARGETNOTONLINE:
			{ // �����ߣ���� Server �������������һ�εĹ�ϵ����Ϣ
				Msg.GetRelation()->m_Type = RET_RELATIONINFO;

				if( pRelationPacket->m_RelationGUID.GetTargetGUID() == INVALID_ID )
				{
					uErr = RET_ERR_TARGETNOTEXIST;
					break;
				}

				const _OWN_RELATION* pRelation = pMyRelation->GetRelationInfo( pRelationPacket->m_RelationGUID.GetTargetGUID() );
				if( pRelation == NULL )
				{
					uErr = RET_ERR_TARGETNOTEXIST;
					break;
				}

				RETURN_RELATION_INFO& RelationInfo = Msg.GetRelation()->m_RelationInfo;
				RelationInfo.CleanUp();
				_RELATION* pRelationData = RelationInfo.GetRelationData();

				const RELATION_MEMBER& Member = pRelation->m_Member;

				RelationInfo.SetRelationType( pRelation->m_Type );
				RelationInfo.SetGroup( pRelation->m_Group );
				RelationInfo.SetFriendPoint( pRelation->m_FriendPoint );

				pRelationData->SetGUID( Member.m_MemberGUID );
				pRelationData->SetName( Member.m_szMemberName );
				pRelationData->SetLevel( Member.m_nLevel );
				pRelationData->SetMenPai( Member.m_nMenPai );
				pRelationData->SetPortrait( Member.m_nPortrait );
				pRelationData->SetGuildID( Member.m_GuildID );

				if ( Member.m_GuildID != INVALID_ID )
				{ // ������Ҫ�ҵ��������
					//pRelationData->SetGuildName( pGuild->GetGuildName() );
				}

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X ask info of GUID=%X.",
					pHuman->GetGUID(), pRelationData->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ADDFRIEND:
			{
				RETURN_ADD_RELATION& AddRelation = pRelationPacket->m_AddRelation;

				// �����Ƿ��п�λ
				if( pMyRelation->IsGroupFull( (RELATION_GROUP)AddRelation.GetGroup() ) )
				{ 
					uErr = RET_ERR_GROUPISFULL;
					break;
				}

				_RELATION* pRelationData = AddRelation.GetRelationData();

				// ����Ŀǰ��ϵ�����Ѿ��и���ң����������ѻ��Ǻ�������
				RELATION_TYPE RelationType = pMyRelation->GetRelationType( pRelationData->GetGUID() );

				switch( RelationType )
				{
				case RELATION_TYPE_FRIEND:
				case RELATION_TYPE_BROTHER:
				case RELATION_TYPE_MARRY:
				case RELATION_TYPE_MASTER:
				case RELATION_TYPE_PRENTICE:
					uErr = RET_ERR_ISFRIEND;
					break;
				case RELATION_TYPE_BLACKNAME:
					{
						// �Ӻ�����������Ӧ�����ƶ���ȥ����Ӧ������������ӣ��������ﲻִ���κβ���
						return PACKET_EXE_CONTINUE;
						//uErr = RET_ERR_ISBLACKNAME;
					}
					break;

					// ����ǳ��ˣ����Ƚ�����Ϊ���ѵ��˴ӳ����б���ɾ��
				case RELATION_TYPE_ENEMY:
					{
						pMyRelation->DelRelation( RELATION_TYPE_ENEMY, pRelationData->GetGUID() );
					}
				case RELATION_TYPE_STRANGER:
					{
						// ��ȷ��ϵ
						// ���յ������ݲ��ֻ��浽 Server
						if( pMyRelation->AddRelation(RELATION_TYPE_FRIEND, pRelationData) != TRUE )
						{
							uErr = RET_ERR_RELATIONUNKNOWN;
						}
						else
						{
							if( pRelationData->GetPoint() != 0 )
							{
								pMyRelation->IncFriendPoint( pRelationData->GetGUID(), pRelationData->GetPoint() );
							}
							
							pMyRelation->SetFriendGroup( pRelationData->GetGUID(), (RELATION_GROUP)AddRelation.GetGroup() );

							// ����Ҫ��Ϣת�浽����Ϣ
							Msg.GetRelation()->m_NewRelation.CleanUp();
							Msg.GetRelation()->m_NewRelation.SetRelationType(AddRelation.GetRelationType());
							Msg.GetRelation()->m_NewRelation.SetGroup(AddRelation.GetGroup());
							Msg.GetRelation()->m_NewRelation.SetRelationData(pRelationData);

							pGamePlayer->SendPacket( &Msg );
							g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X add GUID=%X as a new friend.",
								pHuman->GetGUID(), pRelationData->GetGUID());
							return PACKET_EXE_CONTINUE;
						}
					}
					break;
				default:
					uErr = RET_ERR_RELATIONUNKNOWN;
					Assert(FALSE);
					break;
				}
			}
			break;
		//case RET_ADDTOBLACKLIST:
		//case RET_TEMPFRIEND_ADDTO_BLACKLIST:
			//{
				//RETURN_ADD_RELATION& AddRelation = pRelationPacket->m_AddRelation;
				//_RELATION* pRelationData = AddRelation.GetRelationData();

				//// �����Ƿ��п�λ
				//if( pMyRelation->IsGroupFull( RELATION_GROUP_BLACK ) )
				//{ // û�п�λ��
				//	uErr = RET_ERR_GROUPISFULL;
				//	break;
				//}

				//// ����Ŀǰ��ϵ�����Ѿ��и���ң����������ѻ��Ǻ�������
				//RELATION_TYPE RelationType = pMyRelation->GetRelationType( pRelationData->GetGUID() );

				//switch( RelationType )
				//{
				//case RELATION_TYPE_BLACKNAME:
				//	uErr = RET_ERR_ISBLACKNAME;
				//	break;
				//case RELATION_TYPE_FRIEND:
				//	{ // ��ת��
				//		if( pMyRelation->RelationTransition( RELATION_TYPE_BLACKNAME, pRelationData->GetGUID() ) != TRUE )
				//		{
				//			uErr = RET_ERR_RELATIONUNKNOWN;
				//		}
				//		else
				//		{
				//			Msg.GetRelation()->m_Type = RET_TRANSITION;

				//			RELATION_GUID_UCHAR_UCHAR& RelationInfo = Msg.GetRelation()->m_RelationGUIDUCHARUCHAR;
				//			RelationInfo.CleanUp();
				//			RelationInfo.SetTargetGUID( pRelationData->GetGUID() );
				//			RelationInfo.SetRelationType( RELATION_TYPE_BLACKNAME );
				//			RelationInfo.SetGroup( RELATION_GROUP_BLACK );

				//			pGamePlayer->SendPacket( &Msg );
				//			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X add GUID=%X to black list.",
				//				pHuman->GetGUID(), pRelationData->GetGUID());
				//			return PACKET_EXE_CONTINUE;
				//		}
				//	}
				//	break;
				//case RELATION_TYPE_STRANGER:
				//	{ // ��ȷ��ϵ
				//		if( pMyRelation->AddRelation(RELATION_TYPE_BLACKNAME, pRelationData) != TRUE )
				//		{ // ���յ������ݲ��ֻ��浽 Server
				//			uErr = RET_ERR_RELATIONUNKNOWN;
				//		}
				//		else
				//		{
				//			// ����Ҫ��Ϣת�浽����Ϣ
				//			Msg.GetRelation()->m_NewRelation.CleanUp();
				//			Msg.GetRelation()->m_NewRelation.SetRelationType(AddRelation.GetRelationType());
				//			Msg.GetRelation()->m_NewRelation.SetRelationData(pRelationData);

				//			pGamePlayer->SendPacket( &Msg );
				//			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X add GUID=%X to black list.",
				//				pHuman->GetGUID(), pRelationData->GetGUID() );
				//			return PACKET_EXE_CONTINUE;
				//		}
				//	}
				//	break;
				//case RELATION_TYPE_BROTHER:
				//case RELATION_TYPE_MARRY:
				//case RELATION_TYPE_MASTER:
				//case RELATION_TYPE_PRENTICE:
				//default:
				//	uErr = RET_ERR_RELATIONUNKNOWN;
				//	Assert(FALSE);
				//	break;
				//}
		//	}
		//	break;
		case RET_DELFRIENDFORADDENEMY:
			{
				RELATION_TYPE RelationType = pMyRelation->GetRelationType( pRelationPacket->m_RelationGUID.GetTargetGUID() );
				
				// ����Ǻ��ѻ����Ǻ�������ϵ������ɾ����������ǣ���������
				if ( RelationType == RELATION_TYPE_FRIEND )
				{
					pMyRelation->DelRelation( RELATION_TYPE_FRIEND, pRelationPacket->m_RelationGUID.GetTargetGUID() );
				}
				else if ( RelationType == RELATION_TYPE_BLACKNAME )
				{
					pMyRelation->DelRelation( RELATION_TYPE_BLACKNAME, pRelationPacket->m_RelationGUID.GetTargetGUID() );
				}

				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( pRelationPacket->m_RelationGUID.GetTargetGUID() );

				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X add GUID=%X delfriend as addenemy.",
					pHuman->GetGUID(), pRelationPacket->m_RelationGUID.GetTargetGUID());

				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ADDENEMY:
			{
				RETURN_ADD_RELATION& AddRelation = pRelationPacket->m_AddRelation;
				_RELATION* pRelationData = AddRelation.GetRelationData();

				// ����Ŀǰ��ϵ�����Ѿ��и���ң����������ѻ��Ǻ�������
				RELATION_TYPE RelationType = pMyRelation->GetRelationType( pRelationData->GetGUID() );

				switch( RelationType )
				{
				//����Ѿ��ڳ����б��ʲô��������
				case RELATION_TYPE_ENEMY:
					break ;
				case RELATION_TYPE_FRIEND:
				case RELATION_TYPE_BLACKNAME:
					break;
				case RELATION_TYPE_BROTHER:
				case RELATION_TYPE_MARRY:
				case RELATION_TYPE_MASTER:
				case RELATION_TYPE_PRENTICE:
				
					
				case RELATION_TYPE_STRANGER:
					{ 
						// ��ȷ��ϵ
						if( pMyRelation->AddRelation(RELATION_TYPE_ENEMY, pRelationData) != TRUE )
						{ 
							// ���յ������ݲ��ֻ��浽 Server
							uErr = RET_ERR_RELATIONUNKNOWN;
						}
						else
						{
							// ����Ҫ��Ϣת�浽����Ϣ
							Msg.GetRelation()->m_NewRelation.CleanUp();
							Msg.GetRelation()->m_NewRelation.SetRelationType(AddRelation.GetRelationType());
							Msg.GetRelation()->m_NewRelation.SetRelationData(pRelationData);

							pGamePlayer->SendPacket( &Msg );
							g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X add GUID=%X to enemy list.",
								pHuman->GetGUID(), pRelationData->GetGUID() );
							return PACKET_EXE_CONTINUE;
						}
					}
					break;
				
				default:
					uErr = RET_ERR_RELATIONUNKNOWN;
					Assert(FALSE);
					break;
				}
			}
			break;
		case RET_ADDFRIENDNOTIFY:
			{
				RETURN_NOTIFY_FRIEND* pRecv = &(pRelationPacket->m_NotifyFriend);
				RETURN_NOTIFY_FRIEND* pSend = &(Msg.GetRelation()->m_NotifyFriend);

				pSend->CleanUp();
				pSend->SetGUID( pRecv->GetGUID() );
				pSend->SetName( pRecv->GetName() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X was added friend by GUID=%X.",
					pHuman->GetGUID(), pRecv->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ASKADDFRIEND:
			{
				RETURN_NOTIFY_FRIEND* pRecv = &(pRelationPacket->m_NotifyFriend);
				RETURN_NOTIFY_FRIEND* pSend = &(Msg.GetRelation()->m_NotifyFriend);

				pSend->CleanUp();
				pSend->SetGUID( pRecv->GetGUID() );
				pSend->SetName( pRecv->GetName() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X was ask add friend by GUID=%X.",
					pHuman->GetGUID(), pRecv->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ASKADDTOBLACKLIST:
		case RET_ASKTHROWTOFRIENDLIST:
			{
				RETURN_NOTIFY_FRIEND* pRecv = &(pRelationPacket->m_NotifyFriend);
				RETURN_NOTIFY_FRIEND* pSend = &(Msg.GetRelation()->m_NotifyFriend);

				pSend->CleanUp();
				pSend->SetGUID( pRecv->GetGUID() );
				pSend->SetName( pRecv->GetName() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X was ask move to other list GUID=%X.",
					pHuman->GetGUID(), pRecv->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_DELFRIENDBYMAIL:
			{
				GUID_t guid = pRelationPacket->m_RelationGUID.GetTargetGUID();

				//RELATION_TYPE RelationType = pMyRelation->GetRelationType( guid );
				//switch( RelationType )
				//{
				//case RELATION_TYPE_FRIEND:// �������
				//	break;
				//default:
				//	uErr = RET_ERR_ISNOTFRIEND;
				//}

				//if ( uErr )
				//{
				//	break;
				//}

				RELATION_TYPE deltype = pMyRelation->GetRelationType( guid );
				if ( deltype == RELATION_TYPE_FRIEND )
				{
					pMyRelation->DelRelation( RELATION_TYPE_FRIEND, guid );
				}
				else
				{
					pMyRelation->DelRelation( RELATION_TYPE_BLACKNAME, guid );
				}

				Msg.GetRelation()->m_Type = RET_DELFRIEND;
				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );
				
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X delfriend by mail GUID=%X.",
					pHuman->GetGUID(), pRelationPacket->m_RelationGUID.GetTargetGUID() );

				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ONLINELIST:
			{
				RETURN_ONLINE_LIST* pRecv = &(pRelationPacket->m_RelationOnline);
				RETURN_ONLINE_LIST* pSend = &(Msg.GetRelation()->m_RelationOnline);

				pSend->CleanUp();

				for( INT i=0; i<pRecv->GetOnlineCount(); ++i )
				{
					pSend->AddOnlineRelation( pRecv->GetOnlineRelation(i) );
				}

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: %s gets online friend list.",
					pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_RELATIONONLINE:
			{
				RETURN_NOTIFY_ONLINE* pRecv = &(pRelationPacket->m_NotifyOnline);
				RETURN_NOTIFY_ONLINE* pSend = &(Msg.GetRelation()->m_NotifyOnline);

				pSend->CleanUp();
				pSend->SetTargetName( pRecv->GetTargetName() );
				pSend->SetMood( pRecv->GetMood() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: %s's friend, %s is online.",
					pHuman->GetName(), pRecv->GetTargetName());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_RELATIONOFFLINE:
			{
				RELATION_GUID* pRecv = &(pRelationPacket->m_RelationGUID);
				RELATION_GUID* pSend = &(Msg.GetRelation()->m_RelationGUID);

				pSend->CleanUp();
				pSend->SetTargetGUID( pRecv->GetTargetGUID() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X's friend, GUID=%X is offline.",
					pHuman->GetGUID(), pRecv->GetTargetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_NOTIFY_ADDTEMPFRIEND:
			{
				REQUEST_VIEW_PLAYER* pRecv = &(pRelationPacket->m_PlayerName);
				REQUEST_VIEW_PLAYER* pSend = &(Msg.GetRelation()->m_PlayerName);

				pSend->CleanUp();
				pSend->SetTargetName( pRecv->GetTargetName() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: Name=%s add Name=%s to temp friend.",
					pHuman->GetName(), pRecv->GetTargetName());
				return PACKET_EXE_CONTINUE;
			}
			break;
		//case RET_NEW_GROUP:
		//	{
		//		REQUEST_ADD_NEW_GROUP& NewGroup = pRelationPacket->m_AddNewGroup;
		//		Msg.GetRelation()->m_AddNewGroup.m_uGroupID = NewGroup.m_uGroupID;
		//		Msg.GetRelation()->m_AddNewGroup.SetGroupName(NewGroup.GetGroupName());

		//		pGamePlayer->SendPacket( &Msg );
		//		g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: Name=%s add New Group=%s .",
		//			pHuman->GetName(),NewGroup.GetGroupName());
		//		//pRelationPacket;
		//		return PACKET_EXE_CONTINUE;
		//	}
		//	break;
		case RET_REJECTADDFRIEND:
			{
				RETURN_NOTIFY_FRIEND* pRecv = &(pRelationPacket->m_NotifyFriend);
				RETURN_NOTIFY_FRIEND* pSend = &(Msg.GetRelation()->m_NotifyFriend);

				pSend->CleanUp();
				pSend->SetGUID( pRecv->GetGUID() );
				pSend->SetName( pRecv->GetName() );

				pGamePlayer->SendPacket( &Msg );
				g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GUID=%X was reject add friend by GUID=%X.",
					pHuman->GetGUID(), pRecv->GetGUID());
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ERR_TARGETNOTEXIST:
		case RET_ERR_GROUPISFULL:
		case RET_ERR_ISFRIEND:
		case RET_ERR_ISBLACKNAME:
		case RET_ERR_SPOUSETOBLACKLIST:
		case RET_ERR_MASTERTOBLACKLIST:
		case RET_ERR_PRENTICETOBLACKLIST:
		case RET_ERR_BROTHERTOBLACKLIST:
		case RET_ERR_CANNOT_ADDFRIEND:
		case RET_ERR_RELATIONUNKNOWN:
		case RET_ERR_CANNOTRECEIVEMAIL:
		case RET_ERR_NOTRECVSTRANGEMAIL:
		case RET_ERR_FRIENDGROUPISFULL:
		case RET_ERR_BLACKGROUPISFULL:
			uErr = pRelationPacket->m_Type;
			break;

		default:
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		Msg.GetRelation()->m_Type = uErr;
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: GamePlayer (Type=%d) ",
			pRelationPacket->m_Type );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
