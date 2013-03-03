/********************************************************************************
*	文件名：	GWRegisterTeamRecruitHandler.cpp
*	创建人：	王勇鸷
*	创建时间：	2008 年 5 月 14 日	17:57
*
*	功能说明：	队长发布征收队员的消息
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWRegisterTeamRecruit.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "Log.h"
#include "Team.h"
#include "TeamRecruitManager.h"

uint GWRegisterTeamRecruitHandler::Execute( GWRegisterTeamRecruit* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID(); // 被请求信息的玩家的 GUID

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWRegisterTeamRecruit...User GUID=%X not find!", 
			guid ) ;
		return PACKET_EXE_CONTINUE;
	}

	if ( pPacket->GetType() == LEADER_RECRUIT )
	{//队长发布征兵请示
		if( pUser->GetTeamID() == INVALID_ID )
		{ 
			//Assert( FALSE && "该玩家没有队伍！" );
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
			//Assert(FALSE&&"不是队长不可以申请招收队员！") ;
			return PACKET_EXE_CONTINUE ;
		}

		LEADER_RECRUIT_ATTRIB* pRecruit = pPacket->GetLeaderRecruit();

		LEADER_INFO_NODE *pData = g_pTeamRecruitManager->GetLeaderNullNode();
		pData->m_OwnerInfo.m_Level = pUser->GetLevel() ;
		pData->m_OwnerInfo.m_ManPai = pUser->GetMenpai();
		pData->m_OwnerInfo.m_Country = pUser->GetCountry();
		pData->m_OwnerInfo.m_NameLen = (UCHAR)strlen( pUser->GetName() );
		strncpy((char*)pData->m_OwnerInfo.m_szUserName,(char*)pUser->GetName(),pData->m_OwnerInfo.m_NameLen) ;

		pData->SetRecruitData(*pRecruit);
		g_pTeamRecruitManager->Remove(guid,LEADER_RECRUIT);
		g_pTeamRecruitManager->Add(guid,pData,LEADER_RECRUIT);
	}
	else if ( pPacket->GetType() == MEMBER_RECRUIT )
	{//队员发布征兵请示
		MEMBER_RECRUIT_ATTRIB* pRecruit = pPacket->GetMemberRecruit();

		MEMBER_INFO_NODE *pData = g_pTeamRecruitManager->GetMemberNullNode();

		pData->m_OwnerInfo.m_Level = pUser->GetLevel() ;
		pData->m_OwnerInfo.m_ManPai = pUser->GetMenpai();
		pData->m_OwnerInfo.m_Country = pUser->GetCountry();
		pData->m_OwnerInfo.m_NameLen = (UCHAR)strlen( pUser->GetName() );
		strncpy((char*)pData->m_OwnerInfo.m_szUserName,(char*)pUser->GetName(),pData->m_OwnerInfo.m_NameLen) ;

		pData->SetRecruitData(*pRecruit);
		g_pTeamRecruitManager->Remove(guid,MEMBER_RECRUIT);
		g_pTeamRecruitManager->Add(guid,pData,MEMBER_RECRUIT);
	}
	else
	{
		//Assert(FALSE&&"组队征兵时类型错误");
	}

	Log::SaveLog( WORLD_LOGFILE, "GWRegisterTeamRecruit...Success!  GUID=%X", guid );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
