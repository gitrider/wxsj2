/********************************************************************************
*	文件名：	GWAskTeamRecruitInfoHandler.cpp
*	创建人：	王勇鸷
*	创建时间：	2008 年 5 月 14 日	17:57
*
*	功能说明：	队长发布征收队员的消息
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWAskTeamRecruitInfo.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "Log.h"
#include "Team.h"
#include "TeamRecruitManager.h"
#include "WGRetTeamRecruitInfo.h"

uint GWAskTeamRecruitInfoHandler::Execute( GWAskTeamRecruitInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID(); // 被请求信息的玩家的 GUID

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWAskTeamRecruitInfo...User GUID=%X not find!", 
			guid ) ;
		return PACKET_EXE_CONTINUE;
	}

	WGRetTeamRecruitInfo Msg;
	Msg.SetPlayerID( pUser->GetPlayerID() );

	if ( pPacket->GetType() == LEADER_RECRUIT )
	{//队长请求得到征兵信息
		if( pUser->GetTeamID() == INVALID_ID )
		{ 
			Assert( FALSE && "该玩家没有队伍！" );
			return PACKET_EXE_CONTINUE;
		}

		Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
		if( pTeam==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}

		if ( pTeam->Leader()->m_Member != guid )
		{
			//Assert(FALSE&&"不是队长不可以请求！") ;
			return PACKET_EXE_CONTINUE ;
		}

		//得到自己的征兵条件
		LEADER_INFO_NODE *pLeaderInfo = 0;
		pLeaderInfo = (LEADER_INFO_NODE*)g_pTeamRecruitManager->Get(guid,LEADER_RECRUIT);
		LEADER_RECRUIT_ATTRIB *pMyself = &pLeaderInfo->m_LeaderRecruit;

		//查找满足条件的所有玩家
		IDTable* pTable = g_pTeamRecruitManager->GetMemberTable();

		for( uint i=0; i<MAX_TEAMS; i++ )
		{//分页请求，一次最多50条
			if ( Msg.GetMemberCount() >= ONCE_RECRUIT_LIST_LEN)
			{
				break;
			}			
			
			BOOL IsContinue = FALSE;

			MEMBER_INFO_NODE *pMemberInfo = &g_pTeamRecruitManager->GetMemberRecruitInfo(i);
			if ( pMemberInfo->m_NodeState == NODE_NULL )
			{
				continue;
			}

			RECRUIT_OWNER_INFO *pOther = &pMemberInfo->m_OwnerInfo ;

			if ( pOther->m_Level <= pMyself->m_MaxLevel && pOther->m_Level >= pMyself->m_MinLevel )
			{//等级匹配
				for( int i=0;i<COUATTRIBUTE_NUMBER;i++ )
				{
					if ( pMyself->m_IsInCountry[i] == TRUE && i == pOther->m_Country )
					{//国家匹配
						IsContinue = TRUE;
						break;
					}
				}

				if ( IsContinue == FALSE )
					continue ;

				IsContinue = FALSE;

				//组队目的
				for( int i=0;i<TARGET_NUMBER;i++ )
				{
					if ( pMyself->m_IsTarget[i] == TRUE && 
						pMemberInfo->m_MemberRecruit.m_IsTarget[i] == TRUE )
					{//组队目标有一个匹配就OK
						IsContinue = TRUE;
						break;
					}
				}	

				if ( IsContinue == FALSE )
					continue ;

				IsContinue = FALSE;

				//职业范围
				for( int i=0;i<MATTRIBUTE_NUMBER;i++ )
				{
					if ( pMyself->m_IsMenPaiID[i] == TRUE && i == pOther->m_ManPai )
					{//职业有一个匹配就OK
						IsContinue = TRUE;
						break;
					}
				}	

				if ( IsContinue == TRUE )
				{//增加
					RETURN_RECRUIT_INFO info;
					memset(&info,0,sizeof(RETURN_RECRUIT_INFO));
					info.m_Level = pOther->m_Level;
					info.m_ManPai = pOther->m_ManPai;
					info.m_Country = pOther->m_Country;
					info.m_NameLen = pOther->m_NameLen;

					memcpy(info.m_TeamTarget,pMemberInfo->m_MemberRecruit.m_IsTarget,sizeof(info.m_TeamTarget));
					strncpy((char*)info.m_szUserName,(char*)pOther->m_szUserName,info.m_NameLen);
					memcpy(info.m_szTeamEnounce,pMemberInfo->m_MemberRecruit.m_szTeamEnounce,TEAM_ENOUNCE_LEN-1);

					Msg.AddMember(info);
				}
			}
		}

	}
	else if ( pPacket->GetType() == MEMBER_RECRUIT )
	{//队员请求得到征兵信息
		//得到自己的征兵条件
		MEMBER_INFO_NODE *pMemberInfo = 0;
		pMemberInfo = (MEMBER_INFO_NODE*)g_pTeamRecruitManager->Get(guid,MEMBER_RECRUIT);
		MEMBER_RECRUIT_ATTRIB *pMyself = &pMemberInfo->m_MemberRecruit;

		//查找满足条件的所有玩家
		IDTable* pTable = g_pTeamRecruitManager->GetLeaderTable();

		for( uint i=0; i<MAX_TEAMS; i++ )
		{//分页请求，一次最多50条
			if ( Msg.GetMemberCount() >= ONCE_RECRUIT_LIST_LEN)
			{
				break;
			}			

			BOOL IsContinue = FALSE;

			LEADER_INFO_NODE *pLeaderInfo = &g_pTeamRecruitManager->GetLeaderRecruitInfo(i);
			if ( pLeaderInfo->m_NodeState == NODE_NULL )
			{
				continue;
			}

			RECRUIT_OWNER_INFO *pOther = &pLeaderInfo->m_OwnerInfo ;

			if ( pOther->m_Level <= pMyself->m_MaxLevel && pOther->m_Level >= pMyself->m_MinLevel )
			{//等级匹配
				for( int i=0;i<COUATTRIBUTE_NUMBER;i++ )
				{
					if ( pMyself->m_IsInCountry[i] == TRUE && i == pOther->m_Country )
					{//国家匹配
						IsContinue = TRUE;
						break;
					}
				}

				if ( IsContinue == FALSE )
					continue ;

				IsContinue = FALSE;

				//组队目的
				for( int i=0;i<TARGET_NUMBER;i++ )
				{
					if ( pMyself->m_IsTarget[i] == TRUE && 
						pLeaderInfo->m_LeaderRecruit.m_IsTarget[i] == TRUE )
					{//组队目标有一个匹配就OK
						IsContinue = TRUE;
						break;
					}
				}	

				if ( IsContinue == FALSE )
					continue ;

				if ( IsContinue == TRUE )
				{//增加
					RETURN_RECRUIT_INFO info;
					memset(&info,0,sizeof(RETURN_RECRUIT_INFO));

					info.m_Level = pOther->m_Level;
					info.m_ManPai = pOther->m_ManPai;
					info.m_Country = pOther->m_Country;
					info.m_NameLen = pOther->m_NameLen;

					memcpy(info.m_TeamTarget,pLeaderInfo->m_LeaderRecruit.m_IsTarget,sizeof(info.m_TeamTarget));
					strncpy((char*)info.m_szUserName,(char*)pOther->m_szUserName,info.m_NameLen);
					memcpy(info.m_szTeamEnounce,pLeaderInfo->m_LeaderRecruit.m_szTeamEnounce,TEAM_ENOUNCE_LEN-1);

					Msg.AddMember(info);
				}

			}
		}
	}
	else
	{
		Assert(FALSE&&"组队征兵时类型错误");
	}

	pServerPlayer->SendPacket( &Msg ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWAskTeamRecruitInfo...Success!  GUID=%X", guid );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
