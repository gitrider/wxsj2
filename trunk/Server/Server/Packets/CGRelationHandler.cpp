

#include "stdafx.h"
#include "CGRelation.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GCRelation.h"
#include "GWRelation.h"
#include "ServerManager.h"

uint CGRelationHandler::Execute( CGRelation* pPacket, Player* pPlayer )
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

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	CG_RELATION* pRelationPacket = pPacket->GetRelation();
	GCRelation Msg;
	Msg.GetRelation()->CleanUp();
	INT nErr = 0;

	switch( pRelationPacket->m_Type )
	{
	case REQ_RELATIONLIST:
		{
			GC_RELATIONLIST& RelationList = Msg.GetRelation()->m_RelationList;

			RelationList.CleanUp();

			pHuman->GetHumanRelation()->FillRelationList(RelationList);
			Msg.GetRelation()->m_Type = RET_RELATIONLIST;

			RelationList.SetMood( pHuman->GetHumanRelation()->GetMood() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X get relation list.",
				pHuman->GetGUID() );

			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);

			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONONLINE;
			pRelation->SetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X inform online message.",
				pHuman->GetGUID() );
		}
		break;
	case REQ_RELATIONINFO:
		{ // 直接发给 World
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID(pRelationPacket->m_RequestInfo.GetTargetGUID());

			if( pRelationPacket->m_RequestInfo.m_uNameSize > 0 )
			{
				pRelation->m_RequestInfo.SetTargetName(pRelationPacket->m_RequestInfo.GetTargetName());
			}

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X get relation infomation of GUID=%X.",
				pHuman->GetGUID(), pRelationPacket->m_RequestInfo.GetTargetGUID() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_REJECTADDFRIEND:
		{ //拒绝好友邀请信息,直接传给world
			REQUEST_ADD_RELATION_WITH_GROUP* pMyFriend = &(pRelationPacket->m_AddRelationWithGroup);

			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_REJECTADDFRIEND;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_ADD_RELATION_WITH_GROUP& Relation = pRelation->m_AddRelationWithGroup;
			Relation.CleanUp();
			Relation.SetTargetGUID( pMyFriend->GetTargetGUID() );
			Relation.SetTargetName( pMyFriend->GetTargetName() );
			Relation.SetRelationType( pMyFriend->GetRelationType() );
			Relation.SetGroup( pMyFriend->GetGroup() );	
			
			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s reject add friend.",
				pHuman->GetName(), pMyFriend->GetTargetName() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_VIEWPLAYER:
		{
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSend = pMsg->GetRelation();
			pSend->CleanUp();
			pSend->m_Type = REQ_VIEWPLAYER;
			pSend->SetGUID( pHuman->GetGUID() );
			pSend->m_ViewPlayer.CleanUp();
			pSend->m_ViewPlayer.SetTargetName(pRelationPacket->m_ViewPlayer.GetTargetName());

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X View Player: %s.",
				pHuman->GetGUID(), pRelationPacket->m_ViewPlayer.GetTargetName() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_ADDFRIEND:
	//case REQ_TEMPFRIEND_TO_FRIEND:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			REQUEST_ADD_RELATION_WITH_GROUP* pMyFriend = &(pRelationPacket->m_AddRelationWithGroup);
	
			if( pMyFriend->GetTargetGUID() == INVALID_ID )
			{
				if( strlen(pMyFriend->GetTargetName()) < 1 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add a friend without GUID and name.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				else if( strcmp(pMyFriend->GetTargetName(), pHuman->GetName()) == 0 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self as a friend.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}

			}
			else if( pMyFriend->GetTargetGUID() == pHuman->GetGUID() )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self as a friend.",
					pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			RELATION_GROUP grp = (RELATION_GROUP)pMyFriend->GetGroup();
			switch( grp )
			{ // 只能是以下分组
			case RELATION_GROUP_FRIEND_ALL:
			case RELATION_GROUP_F1:
			case RELATION_GROUP_F2:
			case RELATION_GROUP_F3:
			case RELATION_GROUP_F4:
			case RELATION_GROUP_F5:
				break;
			default:
				AssertEx( FALSE, "Invalid Friend Group.");
				return PACKET_EXE_CONTINUE;
			}

			// 看看是否还有空位
			if( pMyRelation->IsGroupFull( grp ) )
			{ // 没有空位了
				nErr = RET_ERR_FRIENDGROUPISFULL;
				break;
			}

			// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			RELATION_TYPE RelationType;


			if ( pMyFriend->GetTargetGUID() == INVALID_ID )
			{
				const _OWN_RELATION* mRel = pMyRelation->GetRelationInfo( pMyFriend->GetTargetName() ); 
				
				//如果在关系列表中没有找到，那可能是第一次加为好友
				if ( NULL == mRel )
				{
					RelationType = RELATION_TYPE_STRANGER;
				}
				else
				{
					RelationType = pMyRelation->GetRelationType( mRel->m_Member.m_MemberGUID );
				}
			}
			else
			{
				RelationType = pMyRelation->GetRelationType( pMyFriend->GetTargetGUID() );
			}

			
			switch( RelationType )
			{
			case RELATION_TYPE_FRIEND:
				nErr = RET_ERR_ISFRIEND;
				break;
			case RELATION_TYPE_BLACKNAME:
				//nErr = RET_ERR_ISBLACKNAME;
				return PACKET_EXE_CONTINUE;
				//在这需要判断被邀请的玩家是否在自己的黑名单中，如果在自己的黑名单中，直接移动好友列表中，不需要对方做任何的验证
				//这是一个正确的移动组的过程，现在不需要调用，以后可能用的到
				/*{
					GUID_t TargetGUID;
					if ( pMyFriend->GetTargetGUID() == INVALID_ID )
					{
						const _OWN_RELATION* mRelation = pMyRelation->GetRelationInfo( pMyFriend->GetTargetName() ); 
						TargetGUID = mRelation->m_Member.m_MemberGUID;
					}
					else
					{
						TargetGUID = pMyFriend->GetTargetGUID();
					}
					
					// 用转换
					if( pMyRelation->RelationTransition( RELATION_TYPE_FRIEND, TargetGUID ) != TRUE )
					{
						nErr = RET_ERR_RELATIONUNKNOWN;
					}
					else
					{
						Msg.GetRelation()->m_Type = RET_TRANSITION;
						Msg.GetRelation()->m_RelationGUIDUCHARUCHAR.CleanUp();
						Msg.GetRelation()->m_RelationGUIDUCHARUCHAR.SetTargetGUID( TargetGUID );
						Msg.GetRelation()->m_RelationGUIDUCHARUCHAR.SetRelationType( RELATION_TYPE_FRIEND );
						Msg.GetRelation()->m_RelationGUIDUCHARUCHAR.SetGroup( RELATION_GROUP_FRIEND_ALL );

						GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
						GW_RELATION* pRelation = pMsg->GetRelation();
						pRelation->CleanUp();
						pRelation->m_Type = REQ_TRANSITION;
						pRelation->SetGUID( pHuman->GetGUID() );
						RELATION_GUID_UCHAR& Relation = pRelation->m_TransitionRelation;
						Relation.CleanUp();
						Relation.SetTargetGUID( TargetGUID );
						Relation.SetRelationType( RELATION_GROUP_BLACK );

						g_pServerManager->SendPacket( pMsg, INVALID_ID );
						
					}
				}*/
				break;

			case RELATION_TYPE_ENEMY:
			case RELATION_TYPE_STRANGER:
				{ // 正确关系
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);

					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_ADD_RELATION_WITH_GROUP& Relation = pRelation->m_AddRelationWithGroup;
					Relation.CleanUp();
					Relation.SetTargetGUID( pMyFriend->GetTargetGUID() );
					Relation.SetTargetName( pMyFriend->GetTargetName() );
					Relation.SetRelationType( pMyFriend->GetRelationType() );
					Relation.SetGroup( pMyFriend->GetGroup() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s as a new friend.",
						pHuman->GetName(), pMyFriend->GetTargetName() );

					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				nErr = RET_ERR_RELATIONUNKNOWN;
				Assert(FALSE);
				break;
			}
		}
		break;
	//case REQ_ADDTOBLACKLIST:
	//case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
	//	{
			//HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			//REQUEST_ADD_RELATION* pNewBlackName = &(pRelationPacket->m_AddRelation);

			//if( pNewBlackName->GetTargetGUID() == INVALID_ID )
			//{
			//	if( strlen(pNewBlackName->GetTargetName()) < 1 )
			//	{
			//		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add a blackname without GUID and name.",
			//			pHuman->GetName() );
			//		return PACKET_EXE_CONTINUE;
			//	}
			//	else if( strcmp(pNewBlackName->GetTargetName(), pHuman->GetName()) == 0 )
			//	{
			//		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into blacklist.",
			//			pHuman->GetName() );
			//		return PACKET_EXE_CONTINUE;
			//	}

			//}
			//else if( pNewBlackName->GetTargetGUID() == pHuman->GetGUID() )
			//{
			//	g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into blacklist.",
			//		pHuman->GetName() );
			//	return PACKET_EXE_CONTINUE;
			//}

			//// 看看黑名单内是否还有空位
			//if( pMyRelation->IsGroupFull( RELATION_GROUP_BLACK ) )
			//{ // 没有空位了
			//	nErr = RET_ERR_BLACKGROUPISFULL;
			//	break;
			//}


			//// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			//RELATION_TYPE RelationType;

			//if( pNewBlackName->GetTargetGUID() != INVALID_ID )
			//{
			//	RelationType = pMyRelation->GetRelationType( pNewBlackName->GetTargetGUID() );
			//}
			//else
			//{
			//	RelationType = RELATION_TYPE_STRANGER;
			//}

			//switch( RelationType )
			//{
			//case RELATION_TYPE_BLACKNAME:
			//	nErr = RET_ERR_ISBLACKNAME;
			//	break;
			//case RELATION_TYPE_MARRY:
			//	nErr = RET_ERR_SPOUSETOBLACKLIST;
			//	break;
			//case RELATION_TYPE_MASTER:
			//	nErr = RET_ERR_MASTERTOBLACKLIST;
			//	break;
			//case RELATION_TYPE_PRENTICE:
			//	nErr = RET_ERR_PRENTICETOBLACKLIST;
			//	break;
			//case RELATION_TYPE_BROTHER:
			//	nErr = RET_ERR_BROTHERTOBLACKLIST;
			//	break;
			//case RELATION_TYPE_FRIEND:
			//	{ // 用转换
			//		if( pMyRelation->RelationTransition( RELATION_TYPE_BLACKNAME, pNewBlackName->GetTargetGUID() ) != TRUE )
			//		{
			//			nErr = RET_ERR_RELATIONUNKNOWN;
			//		}
			//		else
			//		{
			//			Msg.GetRelation()->m_Type = RET_TRANSITION;

			//			RELATION_GUID_UCHAR_UCHAR& RelationInfo = Msg.GetRelation()->m_RelationGUIDUCHARUCHAR;
			//			RelationInfo.CleanUp();
			//			RelationInfo.SetTargetGUID( pNewBlackName->GetTargetGUID() );
			//			RelationInfo.SetRelationType( RELATION_TYPE_BLACKNAME );
			//			RelationInfo.SetGroup( RELATION_GROUP_BLACK );

			//			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add GUID=%X to black list.",
			//				pHuman->GetName(), pNewBlackName->GetTargetGUID() );
			//		}
			//	}
			//	break;
			//case RELATION_TYPE_STRANGER:
			//	{ // 正确关系
			//		GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			//		GW_RELATION* pRelation = pMsg->GetRelation();
			//		pRelation->CleanUp();
			//		pRelation->m_Type = pRelationPacket->m_Type;
			//		pRelation->SetGUID( pHuman->GetGUID() );
			//		REQUEST_ADD_RELATION& Relation = pRelation->m_AddRelation;
			//		Relation.CleanUp();
			//		Relation.SetTargetGUID( pNewBlackName->GetTargetGUID() );
			//		Relation.SetTargetName( pNewBlackName->GetTargetName() );
			//		Relation.SetRelationType( pNewBlackName->GetRelationType() );

			//		g_pServerManager->SendPacket( pMsg, INVALID_ID );
			//		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s to black list.",
			//			pHuman->GetName(), pNewBlackName->GetTargetName() );

			//		return PACKET_EXE_CONTINUE;
			//	}
			//	break;
			//default:
			//	nErr = RET_ERR_RELATIONUNKNOWN;
			//	Assert(FALSE);
			//	break;
			//}
		//}
		//break;
	case REQ_ADDTOENEMY:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			REQUEST_ADD_RELATION* pNewEnemyName = &(pRelationPacket->m_AddRelation);

			if( pNewEnemyName->GetTargetGUID() == INVALID_ID )
			{
				if( strlen(pNewEnemyName->GetTargetName()) < 1 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add a enemyname without GUID and name.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				else if( strcmp(pNewEnemyName->GetTargetName(), pHuman->GetName()) == 0 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into enemylist.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}

			}
			else if( pNewEnemyName->GetTargetGUID() == pHuman->GetGUID() )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into enemylist.",
					pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			RELATION_TYPE RelationType;

			if ( pNewEnemyName->GetTargetGUID() == INVALID_ID )
			{
				const _OWN_RELATION* mRel = pMyRelation->GetRelationInfo( pNewEnemyName->GetTargetName() ); 

				//如果在关系列表中没有找到，那可能是第一次加为好友
				if ( NULL == mRel )
				{
					RelationType = RELATION_TYPE_STRANGER;
				}
				else
				{
					RelationType = pMyRelation->GetRelationType( mRel->m_Member.m_MemberGUID );
				}
			}
			else
			{
				RelationType = pMyRelation->GetRelationType( pNewEnemyName->GetTargetGUID() );
			}


			switch( RelationType )
			{
			case RELATION_TYPE_ENEMY:// 如果已经是仇人，则不做任何操作
				{
					return PACKET_EXE_CONTINUE;
				}
				break;
			case RELATION_TYPE_BLACKNAME:
			case RELATION_TYPE_MARRY:
			case RELATION_TYPE_MASTER:
			case RELATION_TYPE_PRENTICE:
			case RELATION_TYPE_BROTHER:
			case RELATION_TYPE_FRIEND:
				//以现在的策划案，仇人组与所有组都不冲突，所以加入仇人的时候只是加入，而不是移动
				//{ // 用转换
				//	if( pMyRelation->RelationTransition( RELATION_TYPE_ENEMY, pNewEnemyName->GetTargetGUID() ) != TRUE )
				//	{
				//		nErr = RET_ERR_RELATIONUNKNOWN;
				//	}
				//	else
				//	{
				//		Msg.GetRelation()->m_Type = RET_TRANSITION;

				//		RELATION_GUID_UCHAR_UCHAR& RelationInfo = Msg.GetRelation()->m_RelationGUIDUCHARUCHAR;
				//		RelationInfo.CleanUp();
				//		RelationInfo.SetTargetGUID( pNewEnemyName->GetTargetGUID() );
				//		RelationInfo.SetRelationType( RELATION_TYPE_ENEMY );
				//		RelationInfo.SetGroup( RELATION_GROUP_BLACK );

				//		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add GUID=%X to black list.",
				//			pHuman->GetName(), pNewEnemyName->GetTargetGUID() );
				//	}
				//}
				//break;
			case RELATION_TYPE_STRANGER:
				{ // 正确关系
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_ADD_RELATION& Relation = pRelation->m_AddRelation;
					Relation.CleanUp();
					Relation.SetTargetGUID( pNewEnemyName->GetTargetGUID() );
					Relation.SetTargetName( pNewEnemyName->GetTargetName() );
					Relation.SetRelationType( pNewEnemyName->GetRelationType() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s to black list.",
						pHuman->GetName(), pNewEnemyName->GetTargetName() );

					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				nErr = RET_ERR_RELATIONUNKNOWN;
				Assert(FALSE);
				break;
			}
		}

		break;
	case REQ_TRANSITION:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			RELATION_GUID_UCHAR_UCHAR* pTrans = &(pRelationPacket->m_RelationGUIDUCHARUCHAR);
			GUID_t guid = pTrans->GetTargetGUID();

			// 先判断是否好友的各个组之间的转换
			RELATION_TYPE oldRelationType = pMyRelation->GetRelationType( guid );

			if( pMyRelation->IsFriend( guid )
			 && oldRelationType == (RELATION_TYPE)pTrans->GetRelationType()
			 ) // 好友换组，包括配偶，师徒，结拜兄弟等等
			{
				RELATION_GROUP grp = (RELATION_GROUP)pTrans->GetGroup();
				switch( grp )
				{ // 只能是以下分组
				case RELATION_GROUP_FRIEND_ALL:
				case RELATION_GROUP_F1:
				case RELATION_GROUP_F2:
				case RELATION_GROUP_F3:
				case RELATION_GROUP_F4:
				case RELATION_GROUP_F5:
					break;
				default:
					AssertEx( FALSE, "Invalid Friend Group.");
					return PACKET_EXE_CONTINUE;
				}

				// 看看是否还有空位
				if( pMyRelation->IsGroupFull( grp ) )
				{ // 没有空位了
					nErr = RET_ERR_FRIENDGROUPISFULL;
				}
				else
				{
					pMyRelation->SetFriendGroup( guid, grp );

					Msg.GetRelation()->m_Type = RET_TRANSITION;
					Msg.GetRelation()->m_RelationGUIDUCHARUCHAR = *pTrans;

					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X transition GUID=%X to friend group %d.",
						pHuman->GetGUID(), guid, grp );
				}
			}
			else
			{ // 其他的转换情况
				
				//如果是向黑名单组转换情况，则也需要对黑名单是否满员进行验证
				//如果向黑名单转换情况，还需要添加两个验证
				if( (RELATION_TYPE)pTrans->GetRelationType() == RELATION_TYPE_BLACKNAME )
				{
					// 看看是否还有空位
					if( pMyRelation->IsGroupFull( RELATION_GROUP_BLACK ) )
					{ // 没有空位了
						nErr = RET_ERR_BLACKGROUPISFULL;
						break;
					}

					// 看看对方是不是我的好友,如果不是，不能添加到黑名单中
					if ( !pMyRelation->IsFriend( guid ) )
					{
						nErr = RET_ERR_ISNOTFRIEND;
						break;
					}

					// 看看对方是否已经在黑名单里了
					if ( pMyRelation->IsBlackName( guid ) )
					{
						nErr = RET_ERR_ISBLACKNAME;
						break;
					}
				}
				//如果是从黑名单向好友转换情况
				else if ( (RELATION_TYPE)pTrans->GetRelationType() == RELATION_TYPE_FRIEND )
				{
					// 看看好友组是否还有空位
					if( pMyRelation->IsGroupFull( RELATION_GROUP_FRIEND_ALL ) )
					{ 
						nErr = RET_ERR_FRIENDGROUPISFULL;
						break;
					}

					// 看看对方是不是在我的黑名单中,如果不是，不能添加到好友列表中
					if ( !pMyRelation->IsBlackName( guid ) )
					{
						nErr = RET_ERR_ISNOTINBLACKLIST;
						break;
					}

					// 看看对方是否已经在黑名单里了
					if ( pMyRelation->IsFriend( guid ) )
					{
						nErr = RET_ERR_ISBLACKNAME;
						break;
					}
				}

				if( pMyRelation->CanTransition( oldRelationType, (RELATION_TYPE)pTrans->GetRelationType() ) != TRUE )
				{
					nErr = RET_ERR_CANNOTTRANSITION;
				}
				else if( pMyRelation->RelationTransition((RELATION_TYPE)pTrans->GetRelationType(), guid) != TRUE )	
				{
					nErr = RET_ERR_RELATIONUNKNOWN;
				}
				else
				{
					Msg.GetRelation()->m_Type = RET_TRANSITION;
					Msg.GetRelation()->m_RelationGUIDUCHARUCHAR = *pTrans;

					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					RELATION_GUID_UCHAR& Relation = pRelation->m_TransitionRelation;
					Relation.CleanUp();
					Relation.SetTargetGUID( guid );
					Relation.SetRelationType( pTrans->GetRelationType() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );

					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X transition GUID=%X from %d to %d.",
						pHuman->GetGUID(), guid, oldRelationType, pTrans->GetRelationType() );
				}
			}
		}
		break;
	case REQ_DELFRIEND:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			GUID_t guid = pRelationPacket->m_DelFriend.GetGUID();
			const CHAR* name = pRelationPacket->m_DelFriend.GetName();

			// 看是否有这么个朋友
			RELATION_TYPE RelationType = pMyRelation->GetRelationType( guid );

			switch( RelationType )
			{
				// 暂时还没有这些关系，暂时不用对这些关系进行验证,但是以后有这些关系之后，在这就有可能进行验证，代码还是有用的。
			//case RELATION_TYPE_MARRY:
			//	nErr = RET_ERR_DELSPOUSE;
			//	break;
			//case RELATION_TYPE_MASTER:
			//	nErr = RET_ERR_DELMASTER;
			//	break;
			//case RELATION_TYPE_PRENTICE:
			//	nErr = RET_ERR_DELPRENTICE;
			//	break;
			//case RELATION_TYPE_BROTHER:
			//	nErr = RET_ERR_DELBROTHER;
			//	break;
			case RELATION_TYPE_FRIEND: // 正常情况
				break;
			default:
				nErr = RET_ERR_ISNOTFRIEND;
				break;
			}

			if ( nErr )
			{ // 如果有问题，马上退出
				break;
			}

			// 玩家对被删除好友的好友度
			//INT nMyPoint = pMyRelation->GetFriendPoint( guid );

			// 如果被删除的人不在线在这就不好处理了
			// 不在线的话就无法找到Obj_Human
			/*Obj_Human* pTargetHuman = (Obj_Human*)(g_pGUIDManager->Get( guid ));
				Assert( pTargetHuman );

			HumanRelation* pTargetRelation = pTargetHuman->GetHumanRelation();
				Assert( pTargetRelation );*/
				
			// 被删除好友对玩家的好友度
			//INT nTargetPoint = pTargetRelation->GetFriendPoint( pHuman->GetGUID() );

			if( pMyRelation->GetRelationInfo( guid )->m_FriendPoint >= g_Config.m_ConfigInfo.m_nPasswdPoint
			 && (pHuman->__IsPasswordSetup() && pHuman->__IsPasswordUnlock() != TRUE)
			 )
			{ // 看是否需要对比二级密码
				nErr = RET_ERR_PASSWDMISMATCH;
			}
			else if( pMyRelation->DelRelation( RELATION_TYPE_FRIEND, guid ) != TRUE )
			{ // 删掉它
				nErr = RET_ERR_RELATIONUNKNOWN;
			}
			else
			{
				Msg.GetRelation()->m_Type = RET_DELFRIEND;
				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );

				

				// 在这里向word发送的是删除好友信息
				{
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_DEL_FRIEND& Relation = pRelation->m_DelFriend;
					Relation.CleanUp();
					Relation.SetGUID( guid );
					Relation.SetName( name );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );

					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X del GUID=%X from friend list.",
						pHuman->GetGUID(), guid );
				}
			}
		}
		break;
	//case REQ_DELFROMBLACKLIST:
	//	{
			//HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			//GUID_t guid = pRelationPacket->m_RelationGUID.GetTargetGUID();

			//// 看是否存在于黑名单
			//if( pMyRelation->HaveRelation( RELATION_TYPE_BLACKNAME, guid ) != TRUE )
			//{
			//	nErr = RET_ERR_ISNOTINBLACKLIST;
			//}
			//else if( pMyRelation->DelRelation( RELATION_TYPE_BLACKNAME, guid ) != TRUE )
			//{ // 删掉它
			//	nErr = RET_ERR_RELATIONUNKNOWN;
			//}
			//else
			//{
			//	Msg.GetRelation()->m_Type = RET_DELFROMBLACKLIST;
			//	Msg.GetRelation()->m_RelationGUID.CleanUp();
			//	Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );

			//	GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			//	GW_RELATION* pRelation = pMsg->GetRelation();
			//	pRelation->CleanUp();
			//	pRelation->m_Type = pRelationPacket->m_Type;
			//	pRelation->SetGUID( pHuman->GetGUID() );
			//	RELATION_GUID& Relation = pRelation->m_RelationGUID;
			//	Relation.CleanUp();
			//	Relation.SetTargetGUID( guid );

			//	g_pServerManager->SendPacket( pMsg, INVALID_ID );

			//	g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X del GUID=%X from black list.",
			//		pHuman->GetGUID(), guid );
			//}
	//	}
	//	break;
	case REQ_DELFROMENEMY:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			GUID_t guid = pRelationPacket->m_RelationGUID.GetTargetGUID();

			// 看看是否存在于仇人列表里
			if ( pMyRelation->HaveRelation( RELATION_TYPE_ENEMY, guid )!= TRUE )
			{
				// 如果没有在仇人列表中，直接返回，不做任何操作
				return PACKET_EXE_CONTINUE;
			}
			else if ( pMyRelation->DelRelation( RELATION_TYPE_ENEMY, guid )!=TRUE )
			{	
				// 把server端的玩家数据删除
				nErr = RET_ERR_RELATIONUNKNOWN;
			}
			else
			{
				Msg.GetRelation()->m_Type = RET_DELFROMENEMY;
				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );

				GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
				GW_RELATION* pRelation = pMsg->GetRelation();
				pRelation->CleanUp();
				pRelation->m_Type = pRelationPacket->m_Type;
				pRelation->SetGUID( pHuman->GetGUID() );
				RELATION_GUID& Relation = pRelation->m_RelationGUID;
				Relation.CleanUp();
				Relation.SetTargetGUID( guid );
				g_pServerManager->SendPacket( pMsg, INVALID_ID );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X del GUID=%X from enemy list.",
					pHuman->GetGUID(), guid );
			}
		}
		break;
	case REQ_MODIFYMOOD:
		{
			pHuman->GetHumanRelation()->SetMood( pRelationPacket->m_ModifyMood.GetMood() );
			Msg.GetRelation()->m_Type = RET_NEWMOOD;
			Msg.GetRelation()->m_NewMood.SetMood( pHuman->GetHumanRelation()->GetMood() );

			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = pRelationPacket->m_Type;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_MODIFY_MOOD& Relation = pRelation->m_ModifyMood;
			Relation.CleanUp();
			Relation.SetMood( pHuman->GetHumanRelation()->GetMood() );

            //ModifyBy Amos   称号系统已修改，这里的功能不明先注释了，有问题找我
			//如果当前显示的心情被置为称号，那么同事更新当前称号 Add by hongyu ,如果有不合适，请找我
			//if( pHuman->GetTitleType() == _TITLE::MOOD_TITLE )
			//{
			//	pHuman->SetTitleType(_TITLE::MOOD_TITLE);
			//	pHuman->SetTitle(pHuman->GetHumanRelation()->GetMood());
			//}

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: %s modify mood.", pHuman->GetName() );
		}
		break;
	case REQ_NOTIFY_ADDTEMPFRIEND:
		{
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = pRelationPacket->m_Type;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_VIEW_PLAYER& Relation = pRelation->m_ViewPlayer;
			Relation.CleanUp();
			Relation.SetTargetName( pRelationPacket->m_ViewPlayer.GetTargetName() );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X add Name=%s from black list.",
				pHuman->GetGUID(), Relation.GetTargetName() );
		}
		break;
	//case REQ_ADD_NEW_GROUP:
	//	{
			//REQUEST_ADD_NEW_GROUP& NewGroup = pRelationPacket->m_AddNewGroup;
			//int len = (int)strlen( NewGroup.GetGroupName() );
			//if ( len <= 0 )
			//{
			//	g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GroupName is NULL .",
			//		pRelationPacket->m_AddNewGroup.GetGroupName() );
			//	return PACKET_EXE_CONTINUE;			
			//}

			//const _RELATION_DB_LOAD     *pRelation = pHuman->GetDB()->GetRelationDB();

			////检查玩家自定义组情况
			//int i=0;
			//for( i=0;i<RELATION_GROUP_F5;i++)
			//{
			//	int len = (int)strlen(pRelation->m_szName[i] );
			//	if ( len > 0 )
			//	{
			//		//判断是否同名
			//		if ( strcmp(pRelation->m_szName[i],NewGroup.GetGroupName()) == 0 )
			//		{
			//			//Assert( FALSE && "组同名，不能新建" );
			//			return PACKET_EXE_CONTINUE;						
			//		}

			//		continue;
			//	}

			//	//更新玩家组名,组ID
			//	pHuman->GetDB()->SetName(i,NewGroup.GetGroupName());
			//	NewGroup.m_uGroupID = i + RELATION_GROUP_F1;
			//	g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Group=%s .",
			//		pHuman->GetName(),NewGroup.GetGroupName() );

			//	//返回玩家消息
			//	GCRelation Msg;
			//	Msg.GetRelation()->CleanUp();
			//	Msg.GetRelation()->m_Type = RET_NEW_GROUP;
			//	Msg.GetRelation()->m_AddNewGroup.m_uGroupID = NewGroup.m_uGroupID;
			//	Msg.GetRelation()->m_AddNewGroup.SetGroupName(NewGroup.GetGroupName());

			//	pGamePlayer->SendPacket( &Msg );

			//	g_pLog->FastSaveLog( LOG_FILE_1, "WGRelationHandler: Name=%s add New Group=%s .",
			//		pHuman->GetName(),NewGroup.GetGroupName());

			//	return PACKET_EXE_CONTINUE;

			//}

			//if ( i >= RELATION_GROUP_F5 )
			//{
			//	Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Add New Group=%s  failed!",
			//		pRelationPacket->m_AddNewGroup.GetGroupName());
			//	Assert( FALSE && "组太多，不能新建" );
			//	return PACKET_EXE_CONTINUE;
			//}

			//return PACKET_EXE_CONTINUE;
	//	}
	//	break;
	case REQ_SUREADDFRIEND:
		{//确定加为好友
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			REQUEST_ADD_RELATION_WITH_GROUP* pMyFriend = &(pRelationPacket->m_AddRelationWithGroup);

			if( strlen(pMyFriend->GetTargetName())<=0 )
			{
				g_pLog->FastSaveLog( LOG_FILE_1,"CGRelationHandler: Name=%s attempt to add a friend without GUID.", pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if ( strcmp(pHuman->GetName(), pMyFriend->GetTargetName()) == 0)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self as a friend.", pHuman->GetName() );	
				return PACKET_EXE_CONTINUE;
			}

			RELATION_GROUP grp = (RELATION_GROUP)pMyFriend->GetGroup();
			switch( grp )
			{ // 只能是以下分组
			case RELATION_GROUP_FRIEND_ALL:
			case RELATION_GROUP_F1:
			case RELATION_GROUP_F2:
			case RELATION_GROUP_F3:
			case RELATION_GROUP_F4:
			case RELATION_GROUP_F5:
				break;
			default:
				AssertEx( FALSE, "Invalid Friend Group.");
				return PACKET_EXE_CONTINUE;
			}

			// 看看是否还有空位
			if( pMyRelation->IsGroupFull( grp ) )
			{ 
				nErr = RET_ERR_FRIENDGROUPISFULL;
				break;
			}

			// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			RELATION_TYPE RelationType;

			if ( pMyFriend->GetTargetGUID() == INVALID_ID )
			{
				const _OWN_RELATION* mRel = pMyRelation->GetRelationInfo( pMyFriend->GetTargetName() ); 

				//如果在关系列表中没有找到，那可能是第一次加为好友
				if ( NULL == mRel )
				{
					RelationType = RELATION_TYPE_STRANGER;
				}
				else
				{
					RelationType = pMyRelation->GetRelationType( mRel->m_Member.m_MemberGUID );
				}
			}
			else
			{
				RelationType = pMyRelation->GetRelationType( pMyFriend->GetTargetGUID() );
			}

			switch( RelationType )
			{
			case RELATION_TYPE_FRIEND:
				nErr = RET_ERR_ISFRIEND;
				break;
			case RELATION_TYPE_BLACKNAME:
				{
					return PACKET_EXE_CONTINUE;
					//nErr = RET_ERR_ISBLACKNAME;
				}
				break;
			case RELATION_TYPE_ENEMY:
			case RELATION_TYPE_STRANGER:
				{ // 正确关系
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);

					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_ADD_RELATION_WITH_GROUP& Relation = pRelation->m_AddRelationWithGroup;
					Relation.CleanUp();
					Relation.SetTargetGUID( pMyFriend->GetTargetGUID() );
					Relation.SetTargetName( pMyFriend->GetTargetName() );
					Relation.SetRelationType( pMyFriend->GetRelationType() );
					Relation.SetGroup( pMyFriend->GetGroup() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s as a new friend.",
						pHuman->GetName(), pMyFriend->GetTargetName() );

					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				nErr = RET_ERR_RELATIONUNKNOWN;
				Assert(FALSE);
				break;
			}
		}
		break;

	default :
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		break;
	}

	if ( nErr )
	{
		Msg.GetRelation()->m_Type = nErr;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X Error code %d.",
			pHuman->GetGUID(), nErr );
	}

	pGamePlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
