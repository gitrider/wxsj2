/********************************************************************************
 *	文件名：	User.cpp
 *	全路径：	d:\Prj\World\World\WorldData\User.cpp
 *	创建人：	胡繁
 *	创建时间：	2006 年 4 月 5 日	16:25
 *
 *	功能说明：	
 *	修改记录：
 *********************************************************************************/

#include "stdafx.h"
#include "User.h"

#include "Config.h"
#include "OnlineUser.h"

#include "ServerManager.h"
#include "CountryManager.h"
#include "ServerPlayer.h"

#include "WGRelation.h"

#include "MailCenter.h"
#include "WGMail.h"
#include "WGNotifyMail.h"

#include "Log.h"

// 增加一个关系人
VOID USER::AddRelation( GUID_t guid, const _RELATION_USER* pRUser )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );
	Assert( pRUser );

	if( m_RelationData.m_RelationList.m_nCount >= MAX_RELATION_SIZE )
	{
		Assert( m_RelationData.m_RelationList.m_nCount < MAX_RELATION_SIZE );
		return;
	}

	switch(pRUser->m_RelationType)
	{
	case RELATION_TYPE_FRIEND:
		if( m_RelationData.m_RelationList.m_nFriendCount >= MAX_FRIEND_SIZE )
		{
			Assert( m_RelationData.m_RelationList.m_nFriendCount < MAX_FRIEND_SIZE );
			return;
		}
		++(m_RelationData.m_RelationList.m_nFriendCount);
		break;
	case RELATION_TYPE_BLACKNAME:
		if( m_RelationData.m_RelationList.m_nBlackCount >= MAX_BLACK_SIZE )
		{
			Assert( m_RelationData.m_RelationList.m_nBlackCount < MAX_BLACK_SIZE );
			return;
		}
		++(m_RelationData.m_RelationList.m_nBlackCount);
		break;
	case RELATION_TYPE_ENEMY:		//仇人达到上限后，新的替换旧的
		if( m_RelationData.m_RelationList.m_nEnemyCount < MAX_ENEMY_SIZE )
		++(m_RelationData.m_RelationList.m_nEnemyCount);
		break;
	default:
		break;
	}

	INT nStartPos = m_RelationData.m_RelationList.m_nCount;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		//添加仇人，判断仇人是否已满上限
		if ( pRUser->m_RelationType == RELATION_TYPE_ENEMY  && 
			 m_RelationData.m_RelationList.m_nEnemyCount >= MAX_ENEMY_SIZE )
		{
			//新的替换旧的
			if ( m_RelationData.m_RelationList.m_RelationUsers[i].m_RelationType == RELATION_TYPE_ENEMY )
			{
				m_RelationData.m_RelationList.m_RelationUsers[i] = *pRUser;
				m_RelationData.m_RelationGUIDTable.Add( guid, &(m_RelationData.m_RelationList.m_RelationUsers[i]) );
				break;
			}
			 
			continue;
		}
		
		if( m_RelationData.m_RelationList.m_RelationUsers[nStartPos].m_UserGUID == INVALID_ID )
		{
			m_RelationData.m_RelationList.m_RelationUsers[nStartPos] = *pRUser;
			++(m_RelationData.m_RelationList.m_nCount);
			m_RelationData.m_RelationGUIDTable.Add( guid, &(m_RelationData.m_RelationList.m_RelationUsers[nStartPos]) );
			break;
		}

		if( (++nStartPos) >= MAX_RELATION_SIZE )
		{
			nStartPos = 0;
		}
	}

__LEAVE_FUNCTION
}

// 移除一个关系人
VOID USER::DelRelation( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		if( m_RelationData.m_RelationList.m_RelationUsers[i].m_UserGUID == guid )
		{
			--(m_RelationData.m_RelationList.m_nCount);
			switch(m_RelationData.m_RelationList.m_RelationUsers[i].m_RelationType)
			{
			case RELATION_TYPE_FRIEND:
				--(m_RelationData.m_RelationList.m_nFriendCount);
				break;
			case RELATION_TYPE_BLACKNAME:
				--(m_RelationData.m_RelationList.m_nBlackCount);
				break;
			case RELATION_TYPE_ENEMY:
				--(m_RelationData.m_RelationList.m_nEnemyCount);
				break;
			default:
				break;
			}
			m_RelationData.m_RelationList.m_RelationUsers[i].CleanUp();
			m_RelationData.m_RelationGUIDTable.Remove( guid );
			break;
		}
	}

__LEAVE_FUNCTION
}

// 得到和某玩家的关系状态
enum RELATION_TYPE USER::GetRelationType( GUID_t guid )
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;
	pRUser = (const _RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( pRUser != NULL )
	{
		return (RELATION_TYPE)pRUser->m_RelationType;
	}

__LEAVE_FUNCTION

	return RELATION_TYPE_NONE;
}

// 判断 guid 是否在亲密好友列表中
BOOL USER::IsGoodFriend(GUID_t guid)
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;
	pRUser = (const _RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( (pRUser != NULL) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
	{
		return TRUE;
	}

__LEAVE_FUNCTION

	return FALSE;
}

// 增加一个亲密好友
VOID USER::AddGoodFriend(GUID_t guid)
{
__ENTER_FUNCTION

	_RELATION_USER* pRUser;
	pRUser = (_RELATION_USER*)m_RelationData.m_RelationGUIDTable.Get(guid);
	if( (pRUser != NULL) )
	{
	//	pRUser->m_nFriendPoint = g_Config.m_ConfigInfo.m_nPromptPoint;
	}

__LEAVE_FUNCTION
}

// 填充好友在线列表
VOID USER::FillOnlineList( RETURN_ONLINE_LIST* pSend )
{
__ENTER_FUNCTION

	Assert( pSend );
	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		// 为了保持数据传输是一致性，在这里传回去的是好友的在线列表，而不是密友的在线列表
		//if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		if( pRUser->m_UserGUID != INVALID_ID )
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
			//if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // 在线玩家
			if( pFriend != NULL )
			{
				_RELATION_ONLINE OnlineRelation;
				OnlineRelation.SetGUID( pFriend->GetGUID() );
				OnlineRelation.SetMood( pFriend->GetMood() );
				pSend->AddOnlineRelation( &OnlineRelation );
			}
		}
	}

__LEAVE_FUNCTION
}

// 通知好友上线信息（具体是否是特别好友在客户端进行判断）
VOID USER::InformOnlineMsg()
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		//if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		if(pRUser->m_UserGUID != INVALID_ID)
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
		//	if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // 在线玩家
			if( pFriend != NULL )
			{
				pFriend->SendRelationOnlineMsg( this );
			}
		}
	}

__LEAVE_FUNCTION
}

// 通知在线好友下线信息（具体是否是特别好友在客户端进行判断）
VOID USER::InformOfflineMsg()
{
__ENTER_FUNCTION

	const _RELATION_USER* pRUser;

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		//if( (pRUser->m_UserGUID != INVALID_ID) && (pRUser->m_nFriendPoint >= g_Config.m_ConfigInfo.m_nPromptPoint) )
		if( pRUser->m_UserGUID != INVALID_ID )
		{
			USER* pFriend;

			pFriend = g_pOnlineUser->FindUser( pRUser->m_UserGUID );
			//if( pFriend != NULL && pFriend->IsGoodFriend( GetGUID() ) ) // 在线玩家
			if( pFriend != NULL )
			{
				pFriend->SendRelationOfflineMsg( this );
			}
		}
	}

__LEAVE_FUNCTION
}

// 给该玩家发送某好友上线的信息
VOID USER::SendRelationOnlineMsg(USER* pUser)
{
__ENTER_FUNCTION

	Assert( pUser );

	if( UserStatus() != US_NORMAL )
	{
		return;
	}

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert( pServerPlayer );
		return;
	}

	WGRelation Msg;

	WG_RELATION* pRelation = Msg.GetRelation();

	pRelation->SetPlayerID( GetPlayerID() );
	pRelation->m_Type = RET_RELATIONONLINE;
	pRelation->m_NotifyOnline.SetTargetName( pUser->GetName() );
	pRelation->m_NotifyOnline.SetMood( pUser->GetMood() );

	pServerPlayer->SendPacket( &Msg );

__LEAVE_FUNCTION
}

// 给该玩家发送某好友下线的信息
VOID USER::SendRelationOfflineMsg(USER* pUser)
{
__ENTER_FUNCTION

	Assert( pUser );

	if( UserStatus() != US_NORMAL )
	{
		return;
	}

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert( pServerPlayer );
		return;
	}

	WGRelation Msg;

	WG_RELATION* pRelation = Msg.GetRelation();

	pRelation->SetPlayerID( GetPlayerID() );
	pRelation->m_Type = RET_RELATIONOFFLINE;
	pRelation->m_RelationGUID.SetTargetGUID( pUser->GetGUID() );

	pServerPlayer->SendPacket( &Msg );

__LEAVE_FUNCTION
}

// 返回是否可以发送邮件，如果可以的话，则返回 0，否则返回错误号，错误号参见 enum RELATION_RETURN_TYPE
UCHAR USER::ValidateMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	if( pMail == NULL )
	{
		Assert( pMail );
		return RET_ERR_RELATIONUNKNOWN;
	}

	UCHAR uFlag = 0;

	if( pMail->m_uFlag == MAIL_TYPE_NORMAL || pMail->m_uFlag == MAIL_TYPE_NORMAL2 )
	{
		if( IsCannotRecvMail() )
		{ // 拒收所有邮件
			uFlag = RET_ERR_CANNOTRECEIVEMAIL;
		}
		else
		{
			RELATION_TYPE RelationType = GetRelationType(pMail->m_GUID);

			if( RelationType == RELATION_TYPE_BLACKNAME )
			{ // 黑名单也当作完全拒收处理，以保留颜面……
				uFlag = RET_ERR_CANNOTRECEIVEMAIL;
			}
			else if( (RelationType == RELATION_TYPE_NONE) && IsCannotRecvStrangerMail() )
			{ // 拒收陌生人邮件
				uFlag = RET_ERR_NOTRECVSTRANGEMAIL;
			}
		}
	}

	return uFlag;

__LEAVE_FUNCTION

	return RET_ERR_RELATIONUNKNOWN;
}

// 收到一封邮件
BOOL USER::RecvMail(const MAIL* pMail)
{
__ENTER_FUNCTION

	Assert( pMail );

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( GetServerID() );
	if( pServerPlayer == NULL )
	{
		Assert(pServerPlayer);
		return FALSE;
	}

	if( pMail->m_uFlag == MAIL_TYPE_SCRIPT )
	{ //如果发送的是脚本邮件，则直接发送在目的用户所在服务器执行
		WGMail Msg;
		Msg.SetPlayerID( m_PlayerID );
		Msg.GetMailList()->m_Count = 1;
		Msg.GetMailList()->m_aMail[0] = *pMail;
		Msg.GetMailList()->m_TotalLeft = 0;

		pServerPlayer->SendPacket( &Msg );
	}
	else
	{ //通知此用户有新邮件了
		UCHAR uFlag = ValidateMail(pMail);

		if( uFlag )
		{ // 邮件属于拒收邮件
			USER* pSUser = g_pOnlineUser->FindUser( pMail->m_szSourName );
			if ( pSUser != NULL )
			{
				ServerPlayer* pSServerPlayer = g_pServerManager->GetServerPlayer( pSUser->GetServerID() );
				if( pSServerPlayer == NULL )
				{
					Assert(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				WGRelation Msg;
				Msg.GetRelation()->CleanUp();
				Msg.GetRelation()->SetPlayerID( pSUser->GetPlayerID() );
				Msg.GetRelation()->m_Type = uFlag;
				pSServerPlayer->SendPacket( &Msg );
			}

			return FALSE;
		}

		UINT uMailCount = g_pMailCenter->MailTo( pMail );

		WGNotifyMail Msg;
		Msg.SetPlayerID( m_PlayerID );
		Msg.SetMailCount( (BYTE)(uMailCount>255?255:uMailCount) );

		pServerPlayer->SendPacket( &Msg );
	}

__LEAVE_FUNCTION

	return TRUE;
}

BOOL USER::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( m_LastTime==0 ) m_LastTime = uTime;
	UINT uDeltaTime = uTime-m_LastTime;
	m_LastTime = uTime;

	switch( m_UserStatus )
	{
	case US_NONE:
		return TRUE;
	case US_NORMAL:
	case US_HOLDUSERDATA:
	case US_OFFLINE:
	case US_CRASH_DOWN:
	case US_WAIT_SHUTDOWN:
		{
			if( m_KickTime>0 )
			{
				if( (UINT)m_KickTime<=uDeltaTime )
				{//踢除
					GUID_t guiduser = GetGUID();
					UINT uStatus = m_UserStatus ;
					g_pCountryManager->RemoveCountryUser((COUNTRY_ATTRIBUTE)GetCountry(), GetGUID());
					g_pOnlineUser->OnUserRemove( this );
					g_pOnlineUser->DelUser( this );
					Log::SaveLog( WORLD_LOGFILE, "HeartBeat...User GUID=%X Status=%d", 
						guiduser, uStatus );
				}
				else
				{//
					m_KickTime -= uDeltaTime;
				}
			}
		}
		break;
	};

__LEAVE_FUNCTION

	return TRUE;
}

VOID USER::ResetKickTime( )
{
	m_KickTime = g_Config.m_ConfigInfo.m_KickUserTime;
}

VOID USER::ResetSMU_KickTime()
{
	m_SMU_KickTime = g_Config.m_ConfigInfo.m_SMU_KickUserTime;
}

BOOL USER::AddKick(PlayerID_t serverId, BYTE initstatus)
{
	__ENTER_FUNCTION
		Assert(UserStatus() != US_WAIT_KICK);
	Assert(initstatus >= SK_NONE && initstatus < SK_SIZE);
	for(INT i = 0; i < OVER_MAX_SERVER; ++i)
	{
		if(m_Kick[i].m_ServerID == INVALID_ID)
		{
			m_Kick[i].m_ServerID = serverId;
			m_Kick[i].m_KickStatus = initstatus;
			return TRUE;
		}
	}
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL USER::ClearKick()
{
	__ENTER_FUNCTION
		Assert(UserStatus() != US_WAIT_KICK);

	for(INT i = 0; i < OVER_MAX_SERVER; ++i)	m_Kick[i].CleanUp();
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL USER::ChangeKick(PlayerID_t serverId, BYTE changestatus)
{
	__ENTER_FUNCTION
		Assert(UserStatus() == US_WAIT_KICK);
	Assert(changestatus >= SK_NONE && changestatus < SK_SIZE);

	for(INT i = 0; i < OVER_MAX_SERVER; ++i)
	{
		if(m_Kick[i].m_ServerID == INVALID_ID)	break;
		if(m_Kick[i].m_ServerID == serverId)
		{
			m_Kick[i].m_KickStatus = changestatus;
			return TRUE;
		}
	}
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL USER::IsKickFinish()
{
	__ENTER_FUNCTION
		if(UserStatus() != US_WAIT_KICK) return TRUE;

	for(INT i = 0; i < OVER_MAX_SERVER; ++i)
	{
		if(m_Kick[i].m_ServerID == INVALID_ID) break;
		if(m_Kick[i].m_KickStatus == SK_KICK_SRV_CRUSH)
		{
			g_pLog->FastSaveLog(LOG_FILE_4, "KICK:USER::IsKickFinish ServerCrush During Kick...User GUID=%X Status=%d", 
				GetGUID(), m_UserStatus);
			continue;
		}
		if(m_Kick[i].m_KickStatus != SK_KICK_OK && m_Kick[i].m_KickStatus != SK_KICK_NOUSR) return FALSE;
	}
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

SERVER_KICK* USER::GetKick(INT idx)
{
	__ENTER_FUNCTION
		Assert(idx >= 0 && idx < OVER_MAX_SERVER);
	return &(m_Kick[idx]);
	__LEAVE_FUNCTION
		return NULL;
}


time_t USER::AddRelationDelPoint( GUID_t guid, time_t nTime )
{
__ENTER_FUNCTION
	
	if( guid == INVALID_ID )
		Assert(FALSE);

	INT nOldPoint = -1;
	_RELATION_DEL_POINT_LIST* DelList = &(m_RelationData.m_RelationDelPointList);

	// 先刷新一下列表，去掉已经无用的值，空出空间
	UpDataDelPointList();


	// 查找递减列表里是否已经有了
	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		if ( DelList->m_RelationDelPoint[i].m_UserGUID == guid )
		{
			Assert(0);
			return 0;
		}
	}

	for ( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		if( m_RelationData.m_RelationList.m_RelationUsers[i].m_UserGUID == guid )
		{
			switch(m_RelationData.m_RelationList.m_RelationUsers[i].m_RelationType)
			{
			case RELATION_TYPE_FRIEND:
			case RELATION_TYPE_BLACKNAME:
				{
					nOldPoint = m_RelationData.m_RelationList.m_RelationUsers[i].m_nFriendPoint;
				}
			break;
			}
		}
	}

	// 如果好友度不够一天删除的，那就没有写入表的必要了
	if ((nOldPoint-FRIEND_DEL_NEMBER_ONE_DAY) <=0 )
	{
		return 0;
	}

	// 如果数量已经达到上限，则删除最先加入的
	if( DelList->m_nCount >= MAX_FRIEND_SIZE )
	{
		// 先找到最早的那个节点
		INT nEarly = 0;
		for ( INT i=0; i<MAX_FRIEND_SIZE; ++i)
		{
			if ( DelList->m_RelationDelPoint[nEarly].m_DelTime > DelList->m_RelationDelPoint[i].m_DelTime )
			{
				nEarly = i;
			}
		}
		DelList->m_RelationDelPoint[nEarly].CleanUp();
		--( DelList->m_nCount );
	}

	// 找到一个空的位置加入
	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		if( DelList->m_RelationDelPoint[i].m_UserGUID == INVALID_ID )
		{
			DelList->m_RelationDelPoint[i].m_UserGUID = guid;
			DelList->m_RelationDelPoint[i].m_nFriendPoint = nOldPoint;
			if ( nTime == 0 )
			{
				DelList->m_RelationDelPoint[i].m_DelTime = g_pTimeManager->GetANSITime();
			}
			else
			{
				DelList->m_RelationDelPoint[i].m_DelTime = nTime;
			}

			++(DelList->m_nCount);

			//在这有可能加入写入数据库的代码
			return (DelList->m_RelationDelPoint[i].m_DelTime);
			break;
		}
	}
	
__LEAVE_FUNCTION
return 0;
}


VOID USER::DelRelationDelPoint(GUID_t guid)
{
__ENTER_FUNCTION
	
	Assert( guid !=INVALID_ID );
	
	if ( m_RelationData.m_RelationDelPointList.m_nCount == 0 )
	{
		return;
	}

	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		if( m_RelationData.m_RelationDelPointList.m_RelationDelPoint[i].m_UserGUID == guid )
		{
			m_RelationData.m_RelationDelPointList.m_RelationDelPoint[i].CleanUp();
			--( m_RelationData.m_RelationDelPointList.m_nCount);

			// 在这也应该增加写入数据库代码
			break;
		}
	}

__LEAVE_FUNCTION
}


VOID USER::UpDataDelPointList()
{
__ENTER_FUNCTION

	if ( m_RelationData.m_RelationDelPointList.m_nCount == 0 )
	{
		return;
	}

	_RELATION_DEL_POINT_LIST* DelList = &(m_RelationData.m_RelationDelPointList);

	// 现在的时间 ,这里获得的时间最小单位是秒
	time_t tNowTime = g_pTimeManager->GetANSITime();

	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		if ( DelList->m_RelationDelPoint[i].m_UserGUID != INVALID_ID )
		{
			INT nSpendDay = 0;
			if ((tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)%(60*60*24) != 0)
			{
				nSpendDay = (INT)( (tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)/(60*60*24)+1 );
			}
			else
			{
				nSpendDay = (INT)( (tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)/(60*60*24) );
			}

			// 如果好友度降低到0或者0以下，就从列表中删除这个玩家的数据
			if ( (DelList->m_RelationDelPoint->m_nFriendPoint - (nSpendDay*FRIEND_DEL_NEMBER_ONE_DAY)) <= 0 )
			{
				DelList->m_RelationDelPoint[i].CleanUp();
			}
		}
	}

__LEAVE_FUNCTION
}

INT USER::GetPoint( GUID_t guid )
{
__ENTER_FUNCTION
	
	Assert( guid );

	if ( m_RelationData.m_RelationDelPointList.m_nCount == 0 )
	{
		return 0;
	}

	_RELATION_DEL_POINT_LIST* DelList = &(m_RelationData.m_RelationDelPointList);

	// 现在的时间，这里获得的时间最小单位是秒
	time_t	tNowTime = g_pTimeManager->GetANSITime();

	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		if ( DelList->m_RelationDelPoint[i].m_UserGUID == guid )
		{
			INT nSpendDay = 0;
			
			if ( (tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)%(60*60*24) != 0)
			{
				nSpendDay = (INT)( (tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)/(60*60*24)+1 );
			}
			else
			{
				nSpendDay = (INT)( (tNowTime - DelList->m_RelationDelPoint[i].m_DelTime)/(60*60*24) );
			}

			if ( (DelList->m_RelationDelPoint[i].m_nFriendPoint-( nSpendDay*FRIEND_DEL_NEMBER_ONE_DAY )) <= 0 )
			{
				DelList->m_RelationDelPoint[i].CleanUp();
				return 0;
			}
			else
			{
				return (DelList->m_RelationDelPoint[i].m_nFriendPoint-( nSpendDay*FRIEND_DEL_NEMBER_ONE_DAY ));
			}
		}
	}
	
__LEAVE_FUNCTION
return 0;
}


// 实时更新好友度
VOID USER::SetPoint(INT nPoint, GUID_t guid )
{
__ENTER_FUNCTION
	if ( nPoint == 0 )
	{
		return;
	}
	if ( guid == INVALID_ID )
	{
		return;
	}
	_RELATION_USER* pRUser;

	for ( INT i=0; i<MAX_FRIEND_SIZE; ++i )
	{
		pRUser = &(m_RelationData.m_RelationList.m_RelationUsers[i]);
		if ( (pRUser->m_RelationType == RELATION_TYPE_FRIEND) && (pRUser->m_UserGUID == guid) )
		{
			pRUser->m_nFriendPoint += nPoint;
			return;
		}
	}
__LEAVE_FUNCTION
}