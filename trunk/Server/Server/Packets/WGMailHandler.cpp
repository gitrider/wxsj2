

#include "stdafx.h"
#include "WGMail.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"

#include "GCMail.h"
#include "GWAskMail.h"

#define SCRIPT_MAIL 888889

uint WGMailHandler::Execute( WGMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID() ;
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
	if( pGamePlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGMailHandler: ServerPlayer (Name=%s, MailCount=%d, LeftCount=%d)",
			pHuman->GetName(), pPacket->GetMailList()->m_Count, pPacket->GetMailList()->m_TotalLeft ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		MAIL_LIST* pMailList = pPacket->GetMailList() ;
		if( pMailList->m_Count>0 && pMailList->m_aMail[0].m_uFlag==MAIL_TYPE_SCRIPT )
		{//�������յ��ű��ʼ���ִ��֮
			for( INT i=0; i<pMailList->m_Count; i++ )
			{
				MAIL* pMail = &(pMailList->m_aMail[i]) ;
				pScene->GetLuaInterface()->ExeScript_DDDDDD( SCRIPT_MAIL, 
															 "ExecuteMail", 
															 pScene->SceneID(), 
															 pHuman->GetID(),
															 pMail->m_uParam0,
															 pMail->m_uParam1,
															 pMail->m_uParam2,
															 pMail->m_uParam3 ) ;
			}

			if( pMailList->m_TotalLeft>0 )
			{//������нű��ʼ�����������������ݷ���������
				GWAskMail Msg ;
				Msg.SetAskType( ASK_TYPE_LOGIN ) ;
				Msg.SetGUID( pHuman->GetGUID() ) ;
				g_pServerManager->SendPacket( &Msg, INVALID_ID ) ;
			}
		}
		else
		{//�������յ���ͨ�ʼ��������ͻ���
			GCMail Msg ;
			Msg.SetMailList( pMailList ) ;
			pGamePlayer->SendPacket( &Msg ) ;
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGMailHandler: GamePlayer (Name=%s, MailCount=%d, LeftCount=%d)",
			pHuman->GetName(), pPacket->GetMailList()->m_Count, pPacket->GetMailList()->m_TotalLeft ) ;
	}
	else
	{
		Assert(FALSE) ;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
