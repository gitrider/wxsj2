
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
	{ //û�з����ڡ������û��б����Ӧ�� GUID ���û�
		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User GUID=%X not find!",
			pRelationPacket->GetGUID() );

		//��Ҳ�����
		//Assert( FALSE && "�Ҳ�����Ϣ������" );
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
			{ // ��ҿ��ܲ�����Ҳ���ܲ����ߣ��� Server ���ж�
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
			// pRelationData �������� switch ֮�����

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
			{ // ��ҿ��ܲ�����Ҳ���ܲ�����
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					Recv.GetTargetName() );
				break;
			}

			//�����������˵��ߣ���������˷��ص�����ʾ��Ϣ
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

			// �������
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
			{ // ��ҿ��ܲ�����Ҳ���ܲ�����
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
		{ //��������˴������ܾ�������Ӻ��ѵ�����
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
			{ //���������˲����ߣ����������˷����κ���Ϣ
				return PACKET_EXE_CONTINUE;
			}

			//�����������˵��ߣ���������˷��ص�����ʾ��Ϣ
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				return PACKET_EXE_CONTINUE;
			}

			//�������˷��;ܾ����ܼ�Ϊ�����������Ϣ
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
		{ // �����Ҫ��ӵ�����Ƿ����ߣ���������ʾ��������Ƿ�ͬ���Ϊ������Ϣ
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
			{ //û�з����ڡ������û��б����Ӧ�����ֵ��û� (����ע�ʹ��ˣ��������ȷ�����뿪��Ϸ�����ж�ʱ��������ҿ���ͨ��FindUser()��������뿪��Ϸ��ҵ�����)
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				//Assert( FALSE && "�Ҳ����ú���" );
				break;
			}

			// �ж� pTargetUser �Ƿ�ܾ�����Ϊ����
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
					// ���������ǳ��ˣ���Ҳ���Լ�Ϊ����
				}
				else if ( RelationType == RELATION_TYPE_BLACKNAME )//(temp)�жϱ�����Ϊ���ѵ����Ƿ��Ѿ������������
				{
					uErr = RET_ERR_ISBLACKNAME;

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}
				else
				{
					uErr = RET_ERR_ISFRIEND;//(temp)�������������Ѿ��Ǻ�����

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
					ReqRelation.GetTargetName() );			
					break;
				}	
			}

			//�����������˵��ߣ���������˷��ص�����ʾ��Ϣ
			if ( pTargetUser->UserStatus() == US_OFFLINE )
			{
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );	
				break;
			}
			
			//�������Ϊ���ѵ���ҷ����Ƿ�ͬ�����Ϣ
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
	//	{ // �����Ҫ��ӵ��Ƿ����ߣ���������ϸ��Ϣ�����ﲻ���Ϸ����ж�
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
		//	{ //û�з����ڡ������û��б����Ӧ�����ֵ��û�
		//		uErr = RET_ERR_TARGETNOTEXIST;

		//		Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
		//			ReqRelation.GetTargetName() );
		//		Assert( FALSE && "�Ҳ�������" );
		//		break;
		//	}

		//	//�����������˵��ߣ���������˷��ص�����ʾ��Ϣ
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

		//	// ���������
		//	_RELATION_USER RUser;
		//	RUser.m_UserGUID = pTargetUser->GetGUID();
		//	RUser.m_RelationType = RELATION_TYPE_BLACKNAME;
		//	RUser.m_nFriendPoint = 0;
		//	pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

		//	// ����ȥ������Ϣ
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
		//	// pRelationData �������� switch ֮�����

		//	Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to black list!",
		//		ReqRelation.GetTargetName() );
	//	}
	//	break;
	case REQ_ADDTOENEMY:
		{ 
			// �����Ҫ��ӵ��Ƿ����ߣ���������ϸ��Ϣ�����ﲻ���Ϸ����ж�
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
			{ //û�з����ڡ������û��б����Ӧ�����ֵ��û�
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				Assert( FALSE && "�Ҳ�������" );
				break;
			}

			RELATION_TYPE RelationType = pUser->GetRelationType( pTargetUser->GetGUID() );

			// ���Ҫ��ӵ������б��е�������Լ��ĺ��ѻ��ߺ������е������˫���ȶ��ѶԷ�ɾ��
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

				// ��ɾ���ĺ��Ѽ��뵽���ѵݼ��б�
				time_t NowTime = g_pTimeManager->GetANSITime();

				pUser->AddRelationDelPoint( pTargetUser->GetGUID(), NowTime );
				pTargetUser->AddRelationDelPoint( pUser->GetGUID(), NowTime );

			}

			// �����������
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_ENEMY;

			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );

			// ����ȥ������Ϣ
			Msg.GetRelation()->m_Type = RET_ADDENEMY;
			RETURN_ADD_RELATION& Relation = Msg.GetRelation()->m_AddRelation;

			Relation.CleanUp();
			Relation.SetRelationType( ReqRelation.GetRelationType() );

			pRelationData = Relation.GetRelationData();
			// pRelationData �������� switch ֮�����

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

		// ɾ�����ѵõ���������Ϊ��˫����Ҫ�ѶԷ�ɾ��
	case REQ_DELFRIEND:
		{
			// ��õ�ǰ��ʱ��
			time_t NowTime = g_pTimeManager->GetANSITime();

			pUser->DelRelation(pRelationPacket->m_DelFriend.GetGUID());
			const CHAR* name = pRelationPacket->m_DelFriend.GetName();

			pTargetUser = g_pOnlineUser->FindUser( pRelationPacket->m_DelFriend.GetGUID() );

			if ( pTargetUser != NULL )	// ���ʱ��Է��п���������״̬�����Ƿ����������ݣ��Ϳ���ɾ��
			{
				// ��ɾ���ĺ��Ѽ��뵽���Ѷȵݼ��б���
				pTargetUser->AddRelationDelPoint( pUser->GetGUID(), NowTime );

				pTargetUser->DelRelation( pUser->GetGUID() );

				// ��ɾ���˷���ɾ��������Ϣ
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
				// �����ɾ�����˲�����
				//��һ���ִ���ʼ�
				//���´δ��������ʱ����Serverִ�д��ʼ���Ӧ�Ľű�������ֻ�Ǳ�ɾ���ĺ���ִ��ɾ������
				//g_pMailCenter->SendScriptMail( name, MAIL_DELFRIEND,pRelationPacket->m_DelFriend.GetGUID(), pUser->GetGUID() );
				
				g_pMailCenter->SendScriptMail( name, MAIL_DELFRIEND, (UINT)(NowTime), pUser->GetGUID() );
			}

			return PACKET_EXE_CONTINUE;
		}
		break;

		// ͨ����ִ���ʼ�ɾ������
	case REQ_DELFRIENDBYMAIL:
		{
			// ���뵽���ѵݼ��б�
			pUser->AddRelationDelPoint(pRelationPacket->m_DelFriendByMail.GetGUID(), (time_t)(pRelationPacket->m_DelFriendByMail.GetTIME()));
		
			// ���ܶԷ��Ƿ����ߣ�ֱ��ɾ��
			pUser->DelRelation(pRelationPacket->m_DelFriendByMail.GetGUID());

			//���Լ��Ŀͻ��˷���ɾ��������Ϣ
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
		// �Ӻ����ƶ������������ߴӺ������ƶ������Ѳ��ı�������֮��ĺ��Ѷ�
		//	RUser.m_nFriendPoint = 0;
			pUser->AddRelation( Relation.GetTargetGUID(), &RUser );
			

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//*********������⽫�����ƶ�����������û�жԺ��ѵ���ݽ�����֤��ʦͽ����ż���ȹ�ϵ�����ǲ߻�û��д����Щ��ϵ������֤�����������������ʱ���⴦��*****//
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//����ִ�е��Ǵӱ�����ƶ�����Ӧ����(�Է��п��ܲ�����)��������ƶ������������򷵻�һ����Ϣ�����ⲻ�ÿ���
			
			if( Relation.GetRelationType() == RELATION_TYPE_BLACKNAME )
			{// ��ͻ��˷��ظ�����Ѿ��ƶ�����������Ϣ
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
			{// ��ͻ��˷��ظ�����Ѿ��ƶ���������Ϣ
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
		{ // ����һ�����ܺ���
			pUser->AddGoodFriend( pRelationPacket->m_RelationGUID.GetTargetGUID() );
			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_RELATIONONLINE:
		{ // ��ȡ���ѵ������б�
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
			{ // ��ҿ��ܲ�����Ҳ���ܲ����ߣ��� Server ���ж�
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
			//// �����Ҫ��ӵ��Ƿ����ߣ���������ϸ��Ϣ�����ﲻ���Ϸ����ж�
			//REQUEST_ADD_NEW_GROUP& NewGroup = pRelationPacket->m_AddNewGroup;
			//_RELATION_DB_LOAD     &pRelation  = pUser->GetFullUserData()->m_Relation;

			////�������Զ��������
			//for( int i=0;i<RELATION_GROUP_F5;i++)
			//{
			//	int len = (int)strlen(pRelation.m_szName[i] );
			//	if ( len > 0 )
			//	{
			//		//�ж��Ƿ�ͬ��
			//		if ( strcmp(pRelation.m_szName[i],NewGroup.GetGroupName()) == 0 )
			//		{
			//			//Assert( FALSE && "��ͬ���������½�" );
			//			return PACKET_EXE_CONTINUE;						
			//		}

			//		continue;
			//	}

			//	//�����������,��ID
			//	strncpy((char*)pRelation.m_szName[i],(char*)NewGroup.GetGroupName(),NewGroup.m_uNameSize);
			//	NewGroup.m_uGroupID = i + RELATION_GROUP_F1;	
			//	break;

			//}

			//if ( i >= RELATION_GROUP_F5 )
			//{
			//	Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Add New Group=%s  failed!",
			//		NewGroup.GetGroupName());
			//	Assert( FALSE && "��̫�࣬�����½�" );
			//	return PACKET_EXE_CONTINUE;
			//}

			//// ����ȥ��Ϣ
			//Msg.GetRelation()->m_Type = RET_NEW_GROUP;
			//Msg.GetRelation()->m_AddNewGroup.SetGroupName(NewGroup.GetGroupName());
	//	}

	//	break;

	case REQ_SUREADDFRIEND:
		{//ȷ����Ϊ����
			// �����Ҫ��ӵĺ����Ƿ����ߣ��������º��ѵ���ϸ��Ϣ�����ﲻ���Ϸ����ж�
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
			{ //û�з����ڡ������û��б����Ӧ�����ֵ��û�
				uErr = RET_ERR_TARGETNOTEXIST;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s not find!",
					ReqRelation.GetTargetName() );
				//Assert( FALSE && "�Ҳ����ú���" );
				break;
			}

			// �ж� pTargetUser �Ƿ�ܾ�����Ϊ����
			if ( pTargetUser->IsCannotAddFriend() )
			{
				uErr = RET_ERR_CANNOT_ADDFRIEND;

				Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added!",
					ReqRelation.GetTargetName() );
				break;
			}

			//�����������˵��ߣ���������˷��ص�����ʾ��Ϣ
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
				{// ���������ǳ��ˣ�����ɾ�����˹�ϵ���ټ�Ϊ����,ֻ�ڱ���ɾ��
					
					pUser->DelRelation( pTargetUser->GetGUID() );

				}
				else if ( RelationType == RELATION_TYPE_BLACKNAME )//(temp)�жϱ�����Ϊ���ѵ����Ƿ��Ѿ������������
				{
					uErr = RET_ERR_ISBLACKNAME;

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}
				else
				{
					uErr = RET_ERR_ISFRIEND;//(temp)�������������Ѿ��Ǻ�����

					Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s can't be added2!",
						ReqRelation.GetTargetName() );			
					break;
				}	
			}

			// �ж�ȷ���Ӻ��ѵ�����ڶԷ���ϵ�б��еĹ�ϵ������ڶԷ��ĳ����б��У��ڶԷ�ҲҪ����Ҵӳ����б���ɾ��
			RELATION_TYPE TargetRelationType = pTargetUser->GetRelationType( pUser->GetGUID() );
			if ( TargetRelationType == RELATION_TYPE_ENEMY )
			{
				pTargetUser->DelRelation( pUser->GetGUID() );
			}

			// �����������
			_RELATION_USER RUser;
			RUser.m_UserGUID = pTargetUser->GetGUID();
			RUser.m_RelationType = RELATION_TYPE_FRIEND;
			RUser.m_nFriendPoint = pUser->GetPoint( pTargetUser->GetGUID() );	// ͨ�����Ѷȵݼ��б�����ú��Ѷ�
			UserPoint = RUser.m_nFriendPoint;

			// �Ӻ��Ѷȵݼ��б����Ƴ�
			if ( RUser.m_nFriendPoint != 0)
			{
				pUser->DelRelationDelPoint( pTargetUser->GetGUID() );
			}	
			pUser->AddRelation( pTargetUser->GetGUID(), &RUser );


			// ˫�������Է���Ϊ���ѣ������ǵ������Ӻ���
			_RELATION_USER RTarUser;
			RTarUser.m_UserGUID = pUser->GetGUID();
			RTarUser.m_RelationType = RELATION_TYPE_FRIEND;
			RTarUser.m_nFriendPoint = pTargetUser->GetPoint( pUser->GetGUID() );
			TargetUserPoint = RTarUser.m_nFriendPoint;

			// �Ӻ��ѵݼ��б����Ƴ�
			if ( RTarUser.m_nFriendPoint != 0 )
			{
				pTargetUser->DelRelationDelPoint( pUser->GetGUID() );
			}
			pTargetUser->AddRelation( pUser->GetGUID(), &RTarUser );


			// ����ȥ������Ϣ
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
			// pRelationData �������� switch ֮�����

			Log::SaveLog( WORLD_LOGFILE, "GWRelationHandler...User Name=%s added to friend list!",
			ReqRelation.GetTargetName() );

			if ( pTargetUser->UserStatus() == US_NORMAL )
			{ // ֪ͨ���ӵ���
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


				//����Ҳ��Ҫ�򱻼�Ϊ���ѵ��˴���ȥ������Ϣ
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
		{ // ֻ�����߲ŷ�ʣ����Ϣ
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
