

#include "stdafx.h"
#include "GWAskUserData.h"
#include "Log.h"
#include "OnlineUser.h"
#include "WGRetUserData.h"
#include "ServerPlayer.h"
#include "GuildManager.h"
#include "CountryManager.h"

uint GWAskUserDataHandler::Execute( GWAskUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;
	WGRetUserData Msg ;
	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		
		Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...User GUID=%X not find!", 
			pPacket->GetGUID() ) ;
		//�����������Login ģʽ
		if( g_Config.m_WorldInfo.m_EnableShareMem )
		{
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_NOTFINDUSERDATA ) ;

			pServerPlayer->SendPacket( &Msg ) ;

			return PACKET_EXE_CONTINUE ;
		}

		//����������login ģʽ
		//��������
		pUser = g_pOnlineUser->LoadUser( pPacket->GetGUID(), pPacket->GetCountry(), pPacket->GetProfession(),pPacket->GetAccount()) ;

		if( pUser )
		{
			_HUMAN_DB_LOAD* pDBHuman = &(pUser->GetFullUserData()->m_Human);

			pUser->SetTeamID( pUser->GetFullUserData()->m_Human.m_TeamID );

			//��ʱ��������
			strncpy( pDBHuman->m_Name, pPacket->GetAccount(), MAX_CHARACTER_NAME-1 ) ;
			pDBHuman->m_Sex		= (BOOL)(pPacket->GetGender()) ;
			pDBHuman->m_nCountry = pPacket->GetCountry();
			pDBHuman->m_MenPai   = pPacket->GetProfession();
			pDBHuman->m_FaceModel = pPacket->GetFaceModel();
			pUser->SetKey( pPacket->GetKey() );
			//��ʱ��������

			g_pOnlineUser->OnUserLogin( pUser ) ;
            g_pCountryManager->AddCountryUser((COUNTRY_ATTRIBUTE)pUser->GetCountry(), pUser->GetGUID());

			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATA ) ;
			Msg.SetUserData( pUser->GetFullUserData() ) ;


			//�������ݿ�, �����û�״̬
			g_pOnlineUser->DelData( pUser->GetFullUserData() ) ; //modified by wuwenbin
			pUser->SetFullUserData( NULL );	//Ϊʲô�ÿգ����FindUserʱ���FULLUSERDATA�ṹΪ��
			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			//���û����ڷ�������PlayerID��¼���û�������
			pUser->SetPlayerID( pPacket->GetPlayerID() );

			pServerPlayer->SendPacket( &Msg ) ;

			Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...Load GUID=%X OK!", 
				pPacket->GetGUID() ) ;
		}
		else
		{
			Log::SaveLog( WORLD_LOGFILE, "ERROR:GWAskUserDataHandler...Load GUID=%X false!", 
				pPacket->GetGUID() ) ;
		}
//��������
//

		return PACKET_EXE_CONTINUE ;
	}

	int iUserStatus = pUser->UserStatus() ;

	if( pUser->GetKey() != pPacket->GetKey() )
	{//��֤����󣬾�����Ҳ���ܵ�����
		Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
		Msg.SetReturn( UDR_KEYERROR ) ;

		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler.Key Error..User GUID=%X Status=%d", 
			pPacket->GetGUID(), iUserStatus ) ;

		return PACKET_EXE_CONTINUE ;
	}

	//���û����ڷ�������PlayerID��¼���û�������
	pUser->SetPlayerID( pPacket->GetPlayerID() );

	switch( iUserStatus )
	{
	case US_NONE:
		{
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_NOTFINDUSERDATA ) ;

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case US_HOLDUSERDATA:
		{
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATA ) ;
			Msg.SetUserData( pUser->GetFullUserData() ) ;

			g_pOnlineUser->DelData( pUser->GetFullUserData() ) ;//�������ݿ� // modified by wuwenbin
			pUser->SetFullUserData( NULL );	//Ϊʲô�ÿգ����FindUserʱ���FULLUSERDATA�ṹΪ��
			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case US_NORMAL:
	case US_OFFLINE:
		{
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERDATALIVING ) ;


			pUser->SetUserStatus( US_NORMAL );
			pUser->ResetKickTime();

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case US_CRASH_DOWN://ȡ���������������,��������ʽ��֤��������������
		{
			Assert(FALSE);
			/*
			WGRetUserData Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERSERVERCRASH ) ;
			pServerPlayer->SendPacket( &Msg ) ;
			*/
		}
		break;
	case US_WAIT_SHUTDOWN:
		{
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetReturn( UDR_USERSERVERCRASH ) ;
			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	default :
		{
			Assert(FALSE) ;
		}
		break ;
	};


	Log::SaveLog( WORLD_LOGFILE, "GWAskUserDataHandler...User GUID=%X Status=%d", 
		pPacket->GetGUID(), iUserStatus ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
