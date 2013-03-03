/********************************************************************************
*	�ļ�����	GWRegisterTeamRecruitHandler.cpp
*	�����ˣ�	�����
*	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
*
*	����˵����	�ӳ��������ն�Ա����Ϣ
*	�޸ļ�¼��
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
	GUID_t guid = pPacket->GetGUID(); // ��������Ϣ����ҵ� GUID

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWRegisterTeamRecruit...User GUID=%X not find!", 
			guid ) ;
		return PACKET_EXE_CONTINUE;
	}

	if ( pPacket->GetType() == LEADER_RECRUIT )
	{//�ӳ�����������ʾ
		if( pUser->GetTeamID() == INVALID_ID )
		{ 
			//Assert( FALSE && "�����û�ж��飡" );
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
			//Assert(FALSE&&"���Ƕӳ��������������ն�Ա��") ;
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
	{//��Ա����������ʾ
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
		//Assert(FALSE&&"�������ʱ���ʹ���");
	}

	Log::SaveLog( WORLD_LOGFILE, "GWRegisterTeamRecruit...Success!  GUID=%X", guid );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
