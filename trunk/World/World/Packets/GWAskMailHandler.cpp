

#include "stdafx.h"
#include "GWAskMail.h"
#include "Log.h"
#include "MailCenter.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "WGMail.h"
#include "WGNotifyMail.h"

uint GWAskMailHandler::Execute( GWAskMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;
	if( pUser==NULL )
	{
		//��Ҳ����ߣ��ʼ���ʱ������
		//Assert(FALSE) ;

		Log::SaveLog( WORLD_LOGFILE, "GWAskMailHandler...User GUID=%X not find!",
			pPacket->GetGUID() );
		return PACKET_EXE_CONTINUE ;
	}

	ID_t ServerID = pUser->GetServerID();
	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
	if( pServerPlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGMail Msg ;
	Msg.SetPlayerID( pUser->GetPlayerID() ) ;

	if( pPacket->GetAskType()==ASK_TYPE_LOGIN )
	{
		g_pMailCenter->AskScriptMail( pUser->GetName(), Msg.GetMailList() ) ;

		UINT uMailCount = g_pMailCenter->CheckMail(pUser->GetName());
		if (uMailCount > 0)
		{
			WGNotifyMail NotifyMsg;
			NotifyMsg.SetPlayerID( pUser->GetPlayerID() );
			NotifyMsg.SetMailCount( (UCHAR)(uMailCount>255?255:uMailCount) );
			pServerPlayer->SendPacket( &NotifyMsg );
		}

		if( Msg.GetMailList()->m_Count < 1 )
		{ // ���û�п�ִ�еĽű��ʼ�,�����Ϣ�����ط�.�����ת���ͻ���,����ͨ�ʼ���ʣ��������������Ϊ0
			return PACKET_EXE_CONTINUE;
		}
	}
	else if( pPacket->GetAskType()==ASK_TYPE_MAIL )
	{
		while(g_pMailCenter->AskMail( pUser->GetName(), Msg.GetMailList() ))
		{
			INT nCount;
			nCount = Msg.GetMailList()->m_Count;
			if( nCount != 1 )
			{ // ����������� 1 ��Ļ�����ʽӦ��������ͬ
				Assert( nCount == 1 );
				return PACKET_EXE_CONTINUE;
			}

			if( pUser->ValidateMail( &(Msg.GetMailList()->m_aMail[0]) ) == 0 )
			{ // �ʼ���Ч
				break;
			}
			else
			{ // ��Ч������
				Msg.GetMailList()->CleanUp();
			}
		}

		if( Msg.GetMailList()->m_Count < 1 )
		{ // һ����Ч�ʼ���û��
			return PACKET_EXE_CONTINUE;
		}
	}
	else
	{
		Assert(FALSE) ;
	}

	pServerPlayer->SendPacket( &Msg ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWAskMailHandler...Name=%s MailCount=%d LeftCount=%d",
		pUser->GetName(), Msg.GetMailList()->m_Count, Msg.GetMailList()->m_TotalLeft );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
