
/**	
 *
 *	���ܣ�	�������ؽ���Ĵ���
 *
 *	�޸ļ�¼��
 *			080318	���ʵ��
 *			
 *			
 */

#include "StdAfx.h"
#include "GCFinger.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event/GMEventSystem.h"


uint GCFingerHandler::Execute(GCFinger* pPacket, Player* pPlayer)
{
__ENTER_FUNCTION

	// �ж�������
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
//		CHAR szText[_MAX_PATH * 10];

		GC_FINGER* pFinger = pPacket->GetFinger();

//		pFinger->m_FingerPlayerList	// RETURN_FINGER_PLAYERLIST
		
		if( pFinger )
		{
			switch( pFinger->m_Type )	// FINGER_RETURN_TYPE
			{
			// ������������б�
			case FRET_PLAYERLIST:
				{
					INT nPlayerCount = pFinger->m_FingerPlayerList.GetPlayerCount();	// ����
					FINGER_PLAYER_INFO* pPlayerInfo = NULL;

					FingerPlayerInfo_t  FingerPlayerInfo;

					// ������ݳ�
					CDataPool::GetMe()->Finger_ClearInfo();

					//
					INT nCount = 0;

					for( INT i = 0; i < nPlayerCount; ++i )
					{
						pPlayerInfo = pFinger->m_FingerPlayerList.GetPlayerInfo( i );

						if( pPlayerInfo )
						{
							FingerPlayerInfo.CleanUp();

							FingerPlayerInfo.m_nGUID		= pPlayerInfo->GetGUID();			// GUID
							FingerPlayerInfo.m_sName		= pPlayerInfo->GetName();			// ����
							FingerPlayerInfo.m_bOnlineFlag	= pPlayerInfo->GetOnlineFlag();		// �Ƿ�����
							FingerPlayerInfo.m_nLevel		= pPlayerInfo->GetLevel();			// ��ɫ�ȼ�
							FingerPlayerInfo.m_nSex			= pPlayerInfo->GetSex();			// �Ա�
							FingerPlayerInfo.m_nMenPai		= pPlayerInfo->GetMenPai();			// ���� MENPAI_ATTRIBUTE
							FingerPlayerInfo.m_nGuildID		= pPlayerInfo->GetGuildID();		// ���ID�����ڷ�������ˣ�
							FingerPlayerInfo.m_sGuildName	= pPlayerInfo->GetGuildName();		// ������ƣ����� Server �����ͻ��ˣ�
							FingerPlayerInfo.m_nPortrait	= pPlayerInfo->GetPortrait();		// ͷ��

							// ���浽���ݳ���
							CDataPool::GetMe()->Finger_SetPlayerInfoByIndex( i, &FingerPlayerInfo );

							nCount++;
						}
					}

					CDataPool::GetMe()->Finger_SetPlayerInfoNum( nCount );

//					FINGER_PLAYER_INFO* pLastPlayerInfo = pFinger->m_FingerPlayerList.GetLastPlayerInfo();
//					INT nFingerFlag = pFinger->m_FingerPlayerList.GetFingerFlag();		// �Ƿ���ҹ�ȫ����Χ(û����һҳ)
//					INT nFingerPosition = pFinger->m_FingerPlayerList.GetPosition();	// ��ǰ����λ��

					CEventSystem::GetMe()->PushEvent( GE_OPEN_FRIEND_SEARCH_REASON );
				}
				break;
			
			//
			case FRET_ERR_START:
				{
				}
				break;

			// û���κ�ƥ����
			case FRET_ERR_NORESULT:
				{
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "û���ҵ�ƥ��Ľ����" );
				}
				break;

			// 
			case FRET_NONE:
				{
				}
				break;
			};
		}									

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
