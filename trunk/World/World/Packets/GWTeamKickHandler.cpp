/********************************************************************************
 *	�ļ�����	GWTeamKickHandler.cpp
 *	ȫ·����	d:\PrjMaintance\World\World\Packets\GWTeamKickHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	
 *
 *	����˵����	�ӳ�����Ա�޳�����
 *	�޸ļ�¼��
 *				2006 �� 1 �� 19 ��	15:30�����ٹ��ı������Ƿ�����
*********************************************************************************/


#include "stdafx.h"
#include "GWTeamKick.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "ServerManager.h"

uint GWTeamKickHandler::Execute( GWTeamKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID(); // �ӳ�
	GUID_t dGuid = pPacket->GetDestGUID(); // ��ù��

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	TeamID_t tid = pSourUser->GetTeamID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	if( pTeam==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d not find team!", 
			sGuid, tid );

		return PACKET_EXE_CONTINUE;
	}

	INT nMemberCount = pTeam->MemberCount();
	INT i;
	for( i=0; i<nMemberCount; ++i )
	{
		if ( pTeam->Member(i)->m_Member == dGuid )
		{
			break;
		}
	}

	if( i >= nMemberCount )
	{ // �����߲��ڶ�����
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User dGUID=%X not in sTeamID=%d(sGUID=%X)!", 
			dGuid, tid, sGuid );

		return PACKET_EXE_CONTINUE;
	}

	if( pTeam->Leader()->m_Member != sGuid )
	{ // �����߲��Ƕӳ�
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetErrorCode( TEAM_ERROR_KICKNOTLEADER );

		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d not leader!", 
			sGuid, tid );

		return PACKET_EXE_CONTINUE;
	}

	WGTeamResult Msg;
	Msg.SetReturn( TEAM_RESULT_TEAMKICK );
	Msg.SetGUID( dGuid );
	Msg.SetTeamID( tid );
	Msg.SetFamily( pSourUser->GetMenpai() );
	Msg.SetDataID( pSourUser->GetSex() );
	Msg.SetIcon( pSourUser->GetPortrait() );
	Msg.SetLevel( pSourUser->GetLevel() );

	//֪ͨ�������ڵ����
	for( int i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pUser==NULL )
		{//�����Ա����,���û������ǿ�
			continue;
		}
		
		ID_t ServerID = pUser->GetServerID();
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		if( pServerPlayer==NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pUser->GetPlayerID() );

		pServerPlayer->SendPacket( &Msg );
	}

	//�޳���Ա
	TEAMMEMBER Member;
	Member.m_Member = dGuid;
	pTeam->DelMember( &Member );

	//���ñ����ߵĶ�����Ϣ
	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if ( pDestUser!=NULL )
	{
		pDestUser->SetTeamID( INVALID_ID );
	}

	Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d Success!", 
		sGuid, tid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
