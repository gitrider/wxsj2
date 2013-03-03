#include "stdafx.h"
#include "WGRetTeamDistribMode.h"
#include "GCRetTeamDistribMode.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCTeamAskInvite.h"



uint WGRetTeamDistribModeHandler::Execute( WGRetTeamDistribMode* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		PlayerID_t PlayerID = pPacket->getLeaderID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	BYTE nMode = pPacket->getMode();
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	if (nMode>= ITEM_DISTRIB_NUM)
	{
		return PACKET_EXE_CONTINUE;
	}
	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );
		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetTeamDistribMode: ServerPlayer GUID=%X ",pPacket->getLeaderID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
		
		TeamInfo* pTeamInf = pHuman->GetTeamInfo();
		
		if (pTeamInf == NULL)
		{
			return PACKET_EXE_CONTINUE;
		}
		TeamID_t teamID = pTeamInf->GetTeamID();
		if (teamID == INVALID_INDEX)
		{
			return PACKET_EXE_CONTINUE;
		}
		GCRetTeamDistribMode Msg;
		Msg.setMode(nMode);
		Obj_Human* pOther = NULL;
		for( INT i=0; i<pTeamInf->GetTeamMemberCount(); ++i )
		{
			const TEAMMEMBER* pMember = pTeamInf->GetTeamMember( i );

			pOther = (Obj_Human*)pHuman->getScene()->GetObjManager()->GetObj( pMember->m_ObjID );
			if (pOther != NULL)
			{
				if( pOther->GetObjType() == Obj::OBJ_TYPE_HUMAN )
				{
					TeamInfo* pTeamInfo = pOther->GetTeamInfo();
					if (pTeamInfo->GetTeamID() == teamID)
					{
						pTeamInfo->SetDistribMode(nMode);
						GamePlayer* pTGamePlayer = (GamePlayer*)pOther->GetPlayer();
						if (pTGamePlayer)
						{
							pTGamePlayer->SendPacket(&Msg);
						}
					}
				}
			}

		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetTeamDistribMode: GamePlayer PlayerID=%X ",PlayerID );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
