/********************************************************************************
 *	�ļ�����	GWFingerHandler.cpp
 *	ȫ·����	d:\Prj\World\World\Packets\GWFingerHandler.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2006 �� 4 �� 18 ��	10:05
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "AllUser.h"
#include "Log.h"
#include "GuildManager.h"

#include "GWFinger.h"
#include "WGFinger.h"

UINT GWFingerHandler::Execute(GWFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GW_FINGER* pRecv = pPacket->GetFinger();

	USER* pUser = g_pOnlineUser->FindUser( pRecv->GetGUID() );
	if ( pUser == NULL )
	{ //û�з����ڡ������û��б����Ӧ�� GUID ���û�
		Log::SaveLog( WORLD_LOGFILE, "GWFingerHandler...User GUID=%X not find!",
			pRecv->GetGUID() );

		return PACKET_EXE_CONTINUE;
	}

	WGFinger Msg;
	WG_FINGER* pSend = Msg.GetFinger();
	pSend->CleanUp();
	pSend->SetPlayerID( pUser->GetPlayerID() );

	switch( pRecv->m_Type )
	{
	case FREQ_GUID:	// GUID ����
		{
			GUID_t guid = pRecv->m_FingerByGUID.GetTargetGUID();
			BOOL bOnlineFlag = pRecv->m_FingerByGUID.GetOnlineFlag();
			USER* pTargetUser;

			// �ȴ� OnlineUser �в���
			pTargetUser = g_pOnlineUser->FindUser(guid);

			// �ҵ���ֱ�ӷ���
			if ( pTargetUser != NULL )
			{
				pSend->m_Type = FRET_PLAYERLIST;
				pSend->m_FingerPlayerList.CleanUp();
				pSend->m_FingerPlayerList.AddPlayerInfo( pTargetUser->GetUserSimpleData() );
				FINGER_PLAYER_INFO* pPlayerInfo;
				pPlayerInfo = pSend->m_FingerPlayerList.GetLastPlayerInfo();
				Assert( pPlayerInfo );
				pPlayerInfo->SetOnlineFlag( TRUE );
				pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );
			}
			else if ( bOnlineFlag )
			{ // û���ҵ����ж��Ƿ���Ҫ����������ң�����Ҫ���ش�����Ϣ
				pSend->m_Type = FRET_ERR_NORESULT;
			}
			else
			{ // ��Ҫ�����������в���
				pSend->m_Type = FRET_PLAYERLIST;
				pSend->m_FingerPlayerList.CleanUp();

				if ( g_pAllUser->Select( guid, &(pSend->m_FingerPlayerList) ) < 1 )
				{ // �ҵ��˷��أ����򷵻ش�����Ϣ
					pSend->m_Type = FRET_ERR_NORESULT;
				}
			}
		}
		break;
	case FREQ_NAME: // NAME ����
		{
			const CHAR* szName = pRecv->m_FingerByName.GetTargetName();
			BOOL bOnlineFlag = pRecv->m_FingerByName.GetOnlineFlag();
			BOOL bPreciseFlag = pRecv->m_FingerByName.GetPreciseFlag();
			INT nPosition = pRecv->m_FingerByName.GetPosition();

			pSend->m_FingerPlayerList.CleanUp();

			BOOL bRes;
			if ( bOnlineFlag )
			{ // ���߲���
				bRes = g_pOnlineUser->Select( szName, &(pSend->m_FingerPlayerList), bPreciseFlag, nPosition ) > 0;
			}
			else
			{ // ���߲���
				bRes = g_pAllUser->Select( szName, &(pSend->m_FingerPlayerList), bPreciseFlag, nPosition ) > 0;
			}

			if ( bRes )
			{ //	�ҵ���
				//��FindUserģ��������ô��?������pSend->m_FingerPlayerList���ѱ����������Ϣ���޸ģ������
				//USER *pTargetUser = g_pOnlineUser->FindUser(pRecv->m_FingerByName.GetTargetName());
				//pSend->m_FingerPlayerList.CleanUp();
				//pSend->m_FingerPlayerList.AddPlayerInfo( pTargetUser->GetUserSimpleData() );
				//FINGER_PLAYER_INFO* pPlayerInfo;
				//pPlayerInfo = pSend->m_FingerPlayerList.GetLastPlayerInfo();
				//Assert( pPlayerInfo );
				//pPlayerInfo->SetOnlineFlag( TRUE );

				pSend->m_Type = FRET_PLAYERLIST;
			}
			else
			{ //	û�ҵ�
				pSend->m_Type = FRET_ERR_NORESULT;
			}
		}
		break;
	case FREQ_ADVANCED:	// �߼�����
		{
			UINT uCount;

			pSend->m_FingerPlayerList.CleanUp();

			// �����������ӿڣ�Ȼ�����
			uCount = g_pOnlineUser->Select( pRecv->m_AdvancedFinger.GetMenPai(),
											pRecv->m_AdvancedFinger.GetGuildID(),
											pRecv->m_AdvancedFinger.GetSex(),
											pRecv->m_AdvancedFinger.GetBottomLevel(),
											pRecv->m_AdvancedFinger.GetTopLevel(),
											&(pSend->m_FingerPlayerList),
											pRecv->m_AdvancedFinger.GetPosition() );
			if ( uCount == 0 )
			{ //	û�ҵ�
				pSend->m_Type = FRET_ERR_NORESULT;
			}
			else
			{ //	�ҵ���
				pSend->m_Type = FRET_PLAYERLIST;
			}
		}
		break;
	default:
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
	}

	Log::SaveLog( WORLD_LOGFILE, "GWFingerHandler...%s finger by type=%d!",
		pUser->GetName(), pRecv->m_Type );

	pServerPlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
