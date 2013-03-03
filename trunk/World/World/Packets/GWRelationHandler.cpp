
#include "stdafx.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "Log.h"
#include "ChatCenter.h"
#include "GuildManager.h"
#include "GWRelation.h"
#include "WGRelation.h"
#include "WGChat.h"
#include "Team.h"
#include "MailCenter.h"

uint GWRelationHandler::Execute( GWRelation* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GW_RELATION* pRelationPacket = pPacket->GetRelation();
	WGRelation Msg;
	WGRelation TargetMsg;
	Msg.GetRelation()->CleanUp();
	TargetMsg.GetRelation()->CleanUp();
	UCHAR uErr = 0;

	UINT UserPoint			= 0;
	UINT TargetUserPoint	= 0;

	_RELATION* pRelationData	 = NULL;
	USER* pTargetUser			 = NULL;

	USER* pUser = g_pOnlineUser->FindUser( pRelationPacket->GetGUID() );
	if ( pUser == NULL )
	{ //没有发现在“在线用户列表”里对应此 GUID 的用户
		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%X not find!",
			pRelationPacket->GetGUID() );

		//玩家不在线
		//Assert( FALSE && "找不到信息发送者" );
		return PACKET_EXE_CONTINUE;
	}

	Msg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );

	switch( pRelationPacket->m_Type )
	{
	case REQ_RELATIONINFO:
		{
			REQUEST_RELATION_INFO& ReqRelation = pRelationPacket->m_RequestInfo;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // 玩家可能不存在也可能不在线，由 Server 来判断
				uErr = RET_TARGETNOTONLINE;

				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( ReqRelation.GetTargetGUID() );

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%X not find!",
					ReqRelation.GetTargetGUID() );
				break;
			}

			Msg.GetRelation()->m_Type = RET_RELATIONINFO;

			RETURN_RELATION_INFO& Relation = Msg.GetRelation()->m_RelationInfo;

			Relation.CleanUp();

			pRelationData = Relation.GetRelationData();
			// pRelationData 的数据在 switch 之后填充

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...get User info of Name=%s!",
				pTargetUser->GetName() );
		}
		break;
	case REQ_VIEWPLAYER:
		{
			REQUEST_VIEW_PLAYER& Recv = pRelationPacket->m_ViewPlayer;

			if ( strlen( Recv.GetTargetName() ) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( Recv.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // 玩家可能不存在也可能不在线
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					Recv.GetTargetName() );
				break;
			}

			//如果被邀请的人掉线，则给邀请人返回掉线提示信息
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					Recv.GetTargetName() );	
				break;
			}

			Msg.GetRelation()->m_Type = RET_VIEWPLAYER;

			RETURN_VIEW_PLAYER& Send = Msg.GetRelation()->m_ViewPlayer;
			Send.CleanUp();

			// 填充数据
			Send.SetGUID( pTargetUser->GetGUID() );
			Send.SetName( pTargetUser->GetName() );
			// ...

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...View Player: %s!",
				pTargetUser->GetName() );
		}
		break;

	case REQ_FRIENDADDPOINT:
		{
			REQUEST_RELATION_INFO& RegRelation = pRelationPacket->m_RequestInfo;

			if ( strlen( RegRelation.GetTargetName() ) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( RegRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // 玩家可能不存在也可能不在线
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					RegRelation.GetTargetName() );
				break;
			}
			
			pUser->SetPoint( RegRelation.GetTargetGUID(), pTargetUser->GetGUID() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_REJECTADDFRIEND:
		{ //被邀请的人传回来拒绝接受添加好友的请求
			REQUEST_ADD_RELATION_WITH_GROUP& ReqRelation = pRelationPacket->m_AddRelationWithGroup;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}
			if ( pTargetUser == NULL )
			{ //如果邀请的人不在线，则不向邀请人发送任何消息
				return PACKET_EXE_CONTINUE;
			}

			//如果被邀请的人掉线，则给邀请人返回掉线提示信息
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				return PACKET_EXE_CONTINUE;
			}

			//向邀请人发送拒绝接受加为好友邀请的信息
			WGRelation NotifyMsg;

			NotifyMsg.GetRelation()->CleanUp();
			NotifyMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
			NotifyMsg.GetRelation()->m_Type = RET_REJECTADDFRIEND;
			NotifyMsg.GetRelation()->m_NotifyFriend.CleanUp();
			NotifyMsg.GetRelation()->m_NotifyFriend.SetGUID( pUser->GetGUID() );
			NotifyMsg.GetRelation()->m_NotifyFriend.SetName( pUser->GetName() );

			ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
			if( pTServerPlayer==NULL )
			{
				Assert(FALSE);
				break;
			}
			pTServerPlayer->SendPacket( &NotifyMsg );
			return PACKET_EXE_CONTINUE;

		}
		break;
	case REQ_ADDFRIEND:
	//case REQ_TEMPFRIEND_TO_FRIEND:
		{ // 检查需要添加的玩家是否在线，并发回提示被加玩家是否同意加为好友消息
			REQUEST_ADD_RELATION_WITH_GROUP& ReqRelation = pRelationPacket->m_AddRelationWithGroup;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ //没有发现在“在线用户列表”里对应此名字的用户 (这里注释错了，在玩家正确操作离开游戏后，仍有段时间其他玩家可以通过FindUser()函数获得离开游戏玩家的数据)
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				//Assert( FALSE && "找不到该好友" );
				break;
			}

			// 判断 pTargetUser 是否拒绝被加为好友
			if ( pTargetUser->IsCannotAddFriend() )
			{
				uErr = RET_ERR_CANNOT_ADDFRIEND;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added!",
					ReqRelation.GetTargetName() );
				break;
			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );
			if ( RelationType != RELATION_TYPE_NONE )
			{
				if (RelationType == RELATION_TYPE_ENEMY)
				{
					// 如果该玩家是仇人，则也可以加为好友
				}
				else if ( RelationType == RELATION_TYPE_BLACKNAME )//(temp)判断被邀请为好友的人是否已经被加入黑名单
				{
					uErr = RET_ERR_ISBLACKNAME;

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}
				else
				{
					uErr = RET_ERR_ISFRIEND;//(temp)如果被邀请的人已经是好友了

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
					ReqRelation.GetTargetName() );			
					break;
				}	
			}

			//如果被邀请的人掉线，则给邀请人返回掉线提示信息
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );	
				break;
			}
			
			//向被邀请加为好友的玩家发出是否同意的消息
			if ( pTargetUser->UserStatus() == US_NORMAL )
			{
				WGRelation NotifyMsg;

				NotifyMsg.GetRelation()->CleanUp();
				NotifyMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				NotifyMsg.GetRelation()->m_Type = RET_ASKADDFRIEND;
				NotifyMsg.GetRelation()->m_NotifyFriend.CleanUp();
				NotifyMsg.GetRelation()->m_NotifyFriend.SetGUID( pUser->GetGUID() );
				NotifyMsg.GetRelation()->m_NotifyFriend.SetName( pUser->GetName() );

				ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
				if( pTServerPlayer==NULL )
				{
					Assert(FALSE);
					break;
				}
				pTServerPlayer->SendPacket( &NotifyMsg );
				return PACKET_EXE_CONTINUE;
			}
			
		}
		break;
	//case REQ_ADDTOBLACKLIST:
	//case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
	//	{ // 检查需要添加的是否在线，并发回详细信息，这里不做合法性判断
		//	REQUEST_ADD_RELATION& ReqRelation = pRelationPacket->m_AddRelation;

		//	if ( ReqRelation.GetTargetGUID() != INVALID_ID )
		//	{
		//		pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
		//	}
		//	else if ( strlen(ReqRelation.GetTargetName()) > 0 )
		//	{
		//		pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
		//	}

		//	if ( pTargetUser == NULL )
		//	{ //没有发现在“在线用户列表”里对应此名字的用户
		//		uErr = RET_ERR_TARGETNOTEXIST;

		//		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
		//			ReqRelation.GetTargetName() );
		//		Assert( FALSE && "找不到该人" );
		//		break;
		//	}

		//	//如果被邀请的人掉线，则给邀请人返回掉线提示信息
		//	if ( pTargetUser->UserStatus() == US_OFFLINE )
		//	{
		//		uErr = RET_ERR_TARGETNOTEXIST;

		//		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
		//			ReqRelation.GetTargetName() );	
		//		break;
		//	}


		//	RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );
		//	if ( RelationType != RELATION_TYPE_NONE )
		//	{
		//		if ( RelationType == RELATION_TYPE_FRIEND )
		//		{
		//			pUser->DelRelation( pTargetUser->GetGUID() );
		//		}
		//		else
		//		{
		//			switch( RelationType )
		//			{
		//			case RELATION_TYPE_BLACKNAME:
		//				uErr = RET_ERR_ISBLACKNAME;
		//				break;
		//			case RELATION_TYPE_MARRY:
		//				uErr = RET_ERR_SPOUSETOBLACKLIST;
		//				break;
		//			case RELATION_TYPE_MASTER:
		//				uErr = RET_ERR_MASTERTOBLACKLIST;
		//				break;
		//			case RELATION_TYPE_PRENTICE:
		//				uErr = RET_ERR_PRENTICETOBLACKLIST;
		//				break;
		//			case RELATION_TYPE_BROTHER:
		//				uErr = RET_ERR_BROTHERTOBLACKLIST;
		//				break;
		//			default:
		//				Assert(FALSE);
		//				return PACKET_EXE_CONTINUE;
		//			}

		//			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added4!",
		//				ReqRelation.GetTargetName() );
		//			break;
		//		}
		//	}

		//	// 加入黑名单
		//	_RELATION_USER RUser;
		//	RUser.m_UserGUID = pTargetUser->GetGUID();
		//	RUser.m_RelationType = RELATION_TYPE_BLACKNAME;
		//	RUser.m_nFriendPoint = 0;
		//	pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

		//	// 传回去在线信息
		//	if ( pRelationPacket->m_Type == REQ_ADDTOBLACKLIST )
		//	{
		//		Msg.GetRelation()->m_Type = RET_ADDTOBLACKLIST;
		//	}
		//	else if ( pRelationPacket->m_Type == REQ_TEMPFRIEND_ADDTO_BLACKLIST )
		//	{
		//		Msg.GetRelation()->m_Type = RET_TEMPFRIEND_ADDTO_BLACKLIST;
		//	}

		//	RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

		//	Relation.CleanUp();
		//	Relation.SetRelationType( ReqRelation.GetRelationType() );

		//	pRelationData = Relation.GetRelationData();
		//	// pRelationData 的数据在 switch 之后填充

		//	Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to black list!",
		//		ReqRelation.GetTargetName() );
	//	}
	//	break;
	case REQ_ADDTOENEMY:
		{ 
			// 检查需要添加的是否在线，并发回详细信息，这里不做合法性判断
			REQUEST_ADD_RELATION& ReqRelation = pRelationPacket->m_AddRelation;


			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ //没有发现在“在线用户列表”里对应此名字的用户
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				Assert( FALSE && "找不到该人" );
				break;
			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );

			// 如果要添加到仇人列表中的玩家是自己的好友或者黑名单中的人物，则双方先都把对方删除
			if ( RelationType == RELATION_TYPE_FRIEND || RelationType == RELATION_TYPE_BLACKNAME )
			{
				pUser->DelRelation( pTargetUser->GetGUID() );
				pTargetUser->DelRelation( pUser->GetGUID() );

				WGRelation MyDelFriendMsg;
				MyDelFriendMsg.GetRelation()->CleanUp();
				MyDelFriendMsg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );
				MyDelFriendMsg.GetRelation()->m_Type = RET_DELFRIENDFORADDENEMY;
				MyDelFriendMsg.GetRelation()->m_RelationGUID.CleanUp();
				MyDelFriendMsg.GetRelation()->m_RelationGUID.SetTargetGUID( pTargetUser->GetGUID() );
				pServerPlayer->SendPacket( &MyDelFriendMsg );


				WGRelation DelFriendMsg;
				DelFriendMsg.GetRelation()->CleanUp();
				DelFriendMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				DelFriendMsg.GetRelation()->m_Type = RET_DELFRIENDFORADDENEMY;
				DelFriendMsg.GetRelation()->m_RelationGUID.CleanUp();
				DelFriendMsg.GetRelation()->m_RelationGUID.SetTargetGUID( pUser->GetGUID() );
				ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
				if( pTServerPlayer==NULL )
				{
					Assert(FALSE);
					break;
				}
				pTServerPlayer->SendPacket( &DelFriendMsg );

				// 将删除的好友加入到好友递减列表
				time_t NowTime = g_pTimeManager->GetANSITime();

				pUser->AddRelationDelPoint( pTargetUser->GetGUID(), NowTime );
				pTargetUser->AddRelationDelPoint( pUser->GetGUID(), NowTime );

			}

			// 加入仇人名单
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_ENEMY;

			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

			// 传回去在线信息
			Msg.GetRelation()->m_Type = RET_ADDENEMY;
			RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

			Relation.CleanUp();
			Relation.SetRelationType( ReqRelation.GetRelationType() );

			pRelationData = Relation.GetRelationData();
			// pRelationData 的数据在 switch 之后填充

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to enemy list!",
				ReqRelation.GetTargetName() );
		}

		break;
	case REQ_DELFROMENEMY:
		{
			pUser->DelRelation( pRelationPacket->m_RelationGUID.GetTargetGUID() );
			return PACKET_EXE_CONTINUE;
		}
		break;

		// 删除好友得单独处理，因为是双方都要把对方删除
	case REQ_DELFRIEND:
		{
			// 获得当前的时间
			time_t NowTime = g_pTimeManager->GetANSITime();

			pUser->DelRelation(pRelationPacket->m_DelFriend.GetGUID());
			const CHAR* name = pRelationPacket->m_DelFriend.GetName();

			pTargetUser = g_pOnlineUser->FindUser( pRelationPacket->m_DelFriend.GetGUID() );

			if ( pTargetUser != NULL )	// 这个时候对方有可能是离线状态，但是服务器有数据，就可以删除
			{
				// 被删除的好友加入到好友度递减列表中
				pTargetUser->AddRelationDelPoint( pUser->GetGUID(), NowTime );

				pTargetUser->DelRelation( pUser->GetGUID() );

				// 向被删的人发送删除好友信息
				WGRelation DelMsg;
				DelMsg.GetRelation()->CleanUp();
				DelMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				DelMsg.GetRelation()->m_Type = RET_DELFRIENDBYMAIL;
				DelMsg.GetRelation()->m_RelationGUID.CleanUp();
				DelMsg.GetRelation()->m_RelationGUID.SetTargetGUID( pUser->GetGUID() );

				ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
				if( pTServerPlayer==NULL )
				{
					Assert(FALSE);
					break;
				}
				pTServerPlayer->SendPacket( &DelMsg );
			}
			else
			{
				// 如果被删除的人不在线
				//发一封可执行邮件
				//在下次次玩家上线时会由Server执行此邮件对应的脚本，这里只是被删除的好友执行删除操作
				//g_pMailCenter->SendScriptMail( name, MAIL_DELFRIEND,pRelationPacket->m_DelFriend.GetGUID(), pUser->GetGUID() );
				
				g_pMailCenter->SendScriptMail( name, MAIL_DELFRIEND, (UINT)(NowTime), pUser->GetGUID() );
			}

			return PACKET_EXE_CONTINUE;
		}
		break;

		// 通过可执行邮件删除好友
	case REQ_DELFRIENDBYMAIL:
		{
			// 加入到好友递减列表
			pUser->AddRelationDelPoint(pRelationPacket->m_DelFriendByMail.GetGUID(), (time_t)(pRelationPacket->m_DelFriendByMail.GetTIME()));
		
			// 不管对方是否在线，直接删除
			pUser->DelRelation(pRelationPacket->m_DelFriendByMail.GetGUID());

			//向自己的客户端发送删除好友信息
			WGRelation DelMsg;

			DelMsg.GetRelation()->CleanUp();
			DelMsg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );

			DelMsg.GetRelation()->m_Type = RET_DELFRIENDBYMAIL;
			DelMsg.GetRelation()->m_RelationGUID.CleanUp();
			DelMsg.GetRelation()->m_RelationGUID.SetTargetGUID( pRelationPacket->m_DelFriendByMail.GetGUID() );

			pServerPlayer->SendPacket( &DelMsg );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_TRANSITION:
		{
			RELATION_GUID_UCHAR& Relation = pRelationPacket->m_TransitionRelation;
			pUser->DelRelation( Relation.GetTargetGUID() );

			_RELATION_USER RUser;
			RUser.m_UserGUID = Relation.GetTargetGUID();
			RUser.m_RelationType = (RELATION_TYPE)Relation.GetRelationType();
		// 从好友移动到黑名单或者从黑名单移动到好友不改变两个人之间的好友度
		//	RUser.m_nFriendPoint = 0;
			pUser->AddRelation( Relation.GetTargetGUID(), &RUser );
			

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//*********如果在这将好友移动到黑名单，没有对好友的身份进行验证（师徒，配偶）等关系，但是策划没有写对这些关系进行验证，所以移入黑名单暂时在这处理*****//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//这里执行的是从别的组移动到相应的组(对方有可能不在线)，如果是移动到黑名单，则返回一个信息，在这不用考虑
			
			if( Relation.GetRelationType() == RELATION_TYPE_BLACKNAME )
			{// 向客户端发回该玩家已经移动到黑名单消息
				WGRelation BlackListMsg;

				BlackListMsg.GetRelation()->CleanUp();
				BlackListMsg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );
				BlackListMsg.GetRelation()->m_Type = RET_ASKADDTOBLACKLIST;
				BlackListMsg.GetRelation()->m_NotifyFriend.CleanUp();
				BlackListMsg.GetRelation()->m_NotifyFriend.SetGUID( Relation.GetTargetGUID() );
				//BlackListMsg.GetRelation()->m_NotifyFriend.SetName( pUser->GetName() );

				pServerPlayer->SendPacket( &BlackListMsg );
			}
			else if ( Relation.GetRelationType() == RELATION_TYPE_FRIEND )
			{// 向客户端发回该玩家已经移动到好友消息
				WGRelation FriendListMsg;

				FriendListMsg.GetRelation()->CleanUp();
				FriendListMsg.GetRelation()->SetPlayerID( pUser->GetPlayerID() );
				FriendListMsg.GetRelation()->m_Type = RET_ASKTHROWTOFRIENDLIST;
				FriendListMsg.GetRelation()->m_NotifyFriend.CleanUp();
				FriendListMsg.GetRelation()->m_NotifyFriend.SetGUID( Relation.GetTargetGUID() );

				pServerPlayer->SendPacket( &FriendListMsg );
			}

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_NEWGOODFRIEND:
		{ // 加入一个亲密好友
			pUser->AddGoodFriend( pRelationPacket->m_RelationGUID.GetTargetGUID() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_RELATIONONLINE:
		{ // 获取好友的在线列表
			Msg.GetRelation()->m_Type = RET_ONLINELIST;
			RETURN_ONLINE_LIST* pSend = &(Msg.GetRelation()->m_RelationOnline);
			pSend->CleanUp();
			pUser->FillOnlineList(pSend);
		}
		break;
	case REQ_MODIFYMOOD:
		{
			pUser->SetMood( pRelationPacket->m_ModifyMood.GetMood() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_MODIFYSETTINGS:
		{
			pUser->SetSettings( pRelationPacket->m_Settings.GetSettings() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_NOTIFY_ADDTEMPFRIEND:
		{
			REQUEST_VIEW_PLAYER& Recv = pRelationPacket->m_ViewPlayer;

			if ( strlen(Recv.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( Recv.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ // 玩家可能不存在也可能不在线，由 Server 来判断
				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					Recv.GetTargetName() );
				return PACKET_EXE_CONTINUE;
			}

			Msg.GetRelation()->m_Type = RET_NOTIFY_ADDTEMPFRIEND;
			REQUEST_VIEW_PLAYER* pSend = &(Msg.GetRelation()->m_PlayerName);
			pSend->CleanUp();
			pSend->SetTargetName( pUser->GetName() );
		}
		break;
	//case REQ_ADD_NEW_GROUP:
	//	{ 
			//// 检查需要添加的是否在线，并发回详细信息，这里不做合法性判断
			//REQUEST_ADD_NEW_GROUP& NewGroup = pRelationPacket->m_AddNewGroup;
			//_RELATION_DB_LOAD     &pRelation  = pUser->GetFullUserData()->m_Relation;

			////检查玩家自定义组情况
			//for( int i=0;i<RELATION_GROUP_F5;i++)
			//{
			//	int len = (int)strlen(pRelation.m_szName[i] );
			//	if ( len > 0 )
			//	{
			//		//判断是否同名
			//		if ( strcmp(pRelation.m_szName[i],NewGroup.GetGroupName()) == 0 )
			//		{
			//			//Assert( FALSE && "组同名，不能新建" );
			//			return PACKET_EXE_CONTINUE;						
			//		}

			//		continue;
			//	}

			//	//更新玩家组名,组ID
			//	strncpy((char*)pRelation.m_szName[i],(char*)NewGroup.GetGroupName(),NewGroup.m_uNameSize);
			//	NewGroup.m_uGroupID = i + RELATION_GROUP_F1;	
			//	break;

			//}

			//if ( i >= RELATION_GROUP_F5 )
			//{
			//	Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Add New Group=%s  failed!",
			//		NewGroup.GetGroupName());
			//	Assert( FALSE && "组太多，不能新建" );
			//	return PACKET_EXE_CONTINUE;
			//}

			//// 传回去信息
			//Msg.GetRelation()->m_Type = RET_NEW_GROUP;
			//Msg.GetRelation()->m_AddNewGroup.SetGroupName(NewGroup.GetGroupName());
	//	}

	//	break;

	case REQ_SUREADDFRIEND:
		{//确定加为好友
			// 检查需要添加的好友是否在线，并发回新好友的详细信息，这里不做合法性判断
			REQUEST_ADD_RELATION_WITH_GROUP& ReqRelation = pRelationPacket->m_AddRelationWithGroup;

			if ( ReqRelation.GetTargetGUID() != INVALID_ID )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetGUID() );
			}
			else if ( strlen(ReqRelation.GetTargetName()) > 0 )
			{
				pTargetUser = g_pOnlineUser->FindUser( ReqRelation.GetTargetName() );
			}

			if ( pTargetUser == NULL )
			{ //没有发现在“在线用户列表”里对应此名字的用户
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				//Assert( FALSE && "找不到该好友" );
				break;
			}

			// 判断 pTargetUser 是否拒绝被加为好友
			if ( pTargetUser->IsCannotAddFriend() )
			{
				uErr = RET_ERR_CANNOT_ADDFRIEND;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added!",
					ReqRelation.GetTargetName() );
				break;
			}

			//如果被邀请的人掉线，则给邀请人返回掉线提示信息
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );	
				break;

			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );
			if ( RelationType != RELATION_TYPE_NONE )
			{
				if (RelationType == RELATION_TYPE_ENEMY)
				{// 如果该玩家是仇人，则先删除仇人关系，再加为好友,只在本地删除
					
					pUser->DelRelation( pTargetUser->GetGUID() );

				}
				else if ( RelationType == RELATION_TYPE_BLACKNAME )//(temp)判断被邀请为好友的人是否已经被加入黑名单
				{
					uErr = RET_ERR_ISBLACKNAME;

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}
				else
				{
					uErr = RET_ERR_ISFRIEND;//(temp)如果被邀请的人已经是好友了

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}	
			}

			// 判断确定加好友的玩家在对方关系列表中的关系，如果在对方的仇人列表中，在对方也要将玩家从仇人列表中删除
			RELATION_TYPE TargetRelationType = pTargetUser->GetRelationType( pUser->GetGUID() );
			if ( TargetRelationType == RELATION_TYPE_ENEMY )
			{
				pTargetUser->DelRelation( pUser->GetGUID() );
			}

			// 加入好友名单
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_FRIEND;
			RUser.m_nFriendPoint = pUser->GetPoint( pTargetUser->GetGUID() );	// 通过好友度递减列表来获得好友度
			UserPoint = RUser.m_nFriendPoint;

			// 从好友度递减列表中移除
			if ( RUser.m_nFriendPoint != 0)
			{
				pUser->DelRelationDelPoint( pTargetUser->GetGUID() );
			}	
			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );


			// 双方都将对方加为好友，而不是单向的添加好友
			_RELATION_USER RTarUser;
			RTarUser.m_UserGUID = pUser->GetGUID();
			RTarUser.m_RelationType = RELATION_TYPE_FRIEND;
			RTarUser.m_nFriendPoint = pTargetUser->GetPoint( pUser->GetGUID() );
			TargetUserPoint = RTarUser.m_nFriendPoint;

			// 从好友递减列表中移除
			if ( RTarUser.m_nFriendPoint != 0 )
			{
				pTargetUser->DelRelationDelPoint( pUser->GetGUID() );
			}
			pTargetUser->AddRelation( pUser->GetGUID(), &RTarUser );


			// 传回去在线信息
			if ( pRelationPacket->m_Type == REQ_SUREADDFRIEND )
			{
				Msg.GetRelation()->m_Type = RET_ADDFRIEND;
				TargetMsg.GetRelation()->m_Type = RET_ADDFRIEND;
			}

			RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

			Relation.CleanUp();
			Relation.SetRelationType( ReqRelation.GetRelationType() );
			Relation.SetGroup( ReqRelation.GetGroup() );

			pRelationData = Relation.GetRelationData();
			// pRelationData 的数据在 switch 之后填充

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to friend list!",
			ReqRelation.GetTargetName() );

			if ( pTargetUser->UserStatus() == US_NORMAL )
			{ // 通知被加的人
				WGRelation NotifyMsg;

				NotifyMsg.GetRelation()->CleanUp();
				NotifyMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				NotifyMsg.GetRelation()->m_Type = RET_ADDFRIENDNOTIFY;
				RETURN_NOTIFY_FRIEND& NotifyFriend = NotifyMsg.GetRelation()->m_NotifyFriend;

				NotifyFriend.CleanUp();
				NotifyFriend.SetGUID( pUser->GetGUID() );
				NotifyFriend.SetName( pUser->GetName() );

				ServerPlayer* pTServerPlayer = g_pServerManager->GetServerPlayer( pTargetUser->GetServerID() );
				if( pTServerPlayer==NULL )
				{
					Assert(FALSE);
					break;
				}

				pTServerPlayer->SendPacket( &NotifyMsg );


				//在这也需要向被加为好友的人传回去在线消息
				TargetMsg.GetRelation()->SetPlayerID( pTargetUser->GetPlayerID() );
				TargetMsg.GetRelation()->m_AddRelation.CleanUp();
				TargetMsg.GetRelation()->m_AddRelation.SetRelationType( ReqRelation.GetRelationType() );
				TargetMsg.GetRelation()->m_AddRelation.SetGroup( ReqRelation.GetGroup() );

				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetGUID( pUser->GetGUID() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetName( pUser->GetName() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetLevel( pUser->GetLevel() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetMenPai( pUser->GetMenpai() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetPortrait( pUser->GetPortrait() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetGuildID( pUser->GetGuildID() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetGuildName( g_pGuildManager->GetGuildName( pUser->GetGuildID()) );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetOnlineFlag(1);
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetMood( pUser->GetMood() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetTitle( pUser->GetTitle() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetSceneID( pUser->GetSceneID() );
				TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetPoint( TargetUserPoint );
				if (pUser->GetTeamID() != INVALID_ID)
				{
					Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
					if (pTeam !=NULL)
					{
						TargetMsg.GetRelation()->m_AddRelation.GetRelationData()->SetTeamSize( pTeam->MemberCount() );
					}
				}

				pTServerPlayer->SendPacket( &TargetMsg );
			}
		}
		break;
	default :
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
	}

	if ( uErr > 0 )
	{
		Msg.GetRelation()->m_Type = uErr;
	}

	if ( pRelationData != NULL && pTargetUser != NULL )
	{
		pRelationData->SetGUID( pTargetUser->GetGUID() );
		pRelationData->SetName( pTargetUser->GetName() );
		pRelationData->SetLevel( pTargetUser->GetLevel() );
		pRelationData->SetMenPai( pTargetUser->GetMenpai() );
		pRelationData->SetPortrait( pTargetUser->GetPortrait() );
		pRelationData->SetGuildID( pTargetUser->GetGuildID() );
		pRelationData->SetGuildName( g_pGuildManager->GetGuildName(pTargetUser->GetGuildID()) );
		pRelationData->SetPoint( UserPoint );

		if ( pTargetUser->UserStatus() == US_NORMAL )
		{ // 只有在线才发剩余信息
			pRelationData->SetOnlineFlag(1);
			pRelationData->SetMood( pTargetUser->GetMood() );
			pRelationData->SetTitle( pTargetUser->GetTitle() );
			pRelationData->SetSceneID( pTargetUser->GetSceneID() );

			if ( pTargetUser->GetTeamID() != INVALID_ID )
			{
				Team* pTeam = g_pTeamList->GetTeam( pTargetUser->GetTeamID() );
				if ( pTeam != NULL )
				{
					pRelationData->SetTeamSize( pTeam->MemberCount() );
				}
			}
		}
	}

	pServerPlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
