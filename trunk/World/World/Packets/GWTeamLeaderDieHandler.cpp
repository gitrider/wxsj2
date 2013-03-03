
#include "stdafx.h"
#include "GWTeamLeaderDie.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"

uint GWTeamLeaderDieHandler::Execute( GWTeamLeaderDie* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();


	USER* pUser = g_pOnlineUser->FindUser( sGuid );
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderDieHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	if( pUser->GetTeamID() != INVALID_ID )
	{ // ���������ĳ������
		Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
		if( pTeam )
		{
			USER* pNewLeader = NULL;

			if( pTeam->Leader()->m_Member == pUser->GetGUID() )
			{ // ����Ƕӳ�����˳����һ�������ߵ����

				for( INT i=0; i<pTeam->MemberCount(); i++ )
				{
					TEAMMEMBER* pMember = pTeam->Member( i );
					if( pMember==NULL )
					{
						Assert(FALSE);
						continue;
					}

					if ( pMember->m_Member == pUser->GetGUID() || pMember->m_IsMomentLeave )
					{
						//��Ա��ʱ���,���Һ����Լ�
						continue ;
					}

					pNewLeader = g_pOnlineUser->FindUser( pMember->m_Member );
					if( pNewLeader==NULL )
					{
						AssertEx(FALSE, "���߶�Ա��");
						continue;
					}

					if( pNewLeader->UserStatus() != US_NORMAL )
					{
						pNewLeader = NULL;
						continue;
					}

					break;

				}//end for

				if( pNewLeader != NULL )
				{ // �����¶ӳ�
					g_pOnlineUser->AppointTeamLeader(pUser, pNewLeader);
					Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaderDieHandler...Success!  sGUID=%X dGUID=%X", 
						sGuid, pNewLeader->GetGUID() );
				}

			}
		}
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
