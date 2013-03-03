/********************************************************************************
 *	�ļ�����	WGFingerHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGFingerHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 4 �� 27 ��	16:22
 *
 *	����˵����	��Ҳ��ҵķ�����Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ServerManager.h"

#include "WGFinger.h"
#include "GCFinger.h"

UINT WGFingerHandler::Execute(WGFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetFinger()->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL ) return PACKET_EXE_CONTINUE;

	WG_FINGER* pRecv = pPacket->GetFinger();

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: ServerPlayer (Type=%d) ",
			pRecv->m_Type );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCFinger Msg;
		GC_FINGER* pSend = Msg.GetFinger();
		pSend->CleanUp();
		pSend->m_Type = pRecv->m_Type;

		switch( pRecv->m_Type )
		{
		case FRET_PLAYERLIST:
			{
				RETURN_FINGER_PLAYERLIST& PlayerList = pRecv->m_FingerPlayerList;
				INT nCount;
				nCount = PlayerList.GetPlayerCount();

				pSend->m_FingerPlayerList.CleanUp();

				for( INT i=0; i<nCount; ++i )
				{
					pSend->m_FingerPlayerList.AddPlayerInfo( PlayerList.GetPlayerInfo(i) );
				}

				pSend->m_FingerPlayerList.SetFingerFlag( PlayerList.GetFingerFlag() );
				pSend->m_FingerPlayerList.SetPosition( PlayerList.GetPosition() );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: Type=%d Count=%d",
					pRecv->m_Type, PlayerList.GetPlayerCount() );
			}
			break;
		case FRET_ERR_NORESULT:
			g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: Type=%d",
				pRecv->m_Type );
			break;
		default:
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
