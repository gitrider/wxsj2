#include "stdafx.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"

#include "WGCallOfHuman.h"


uint WGCallOfHumanHandler::Execute( WGCallOfHuman* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetCallOfTargetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE && "WGCallOfHumanHandler::Execute pGamePlayer==NULL");
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE && "WGCallOfHumanHandler::Execute pScene==NULL");
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGCallOfHumanHandler: CallerGUID=%u, SceneID=%d, Pos=%f,%f, Duration=%u",
			pPacket->GetCallOfInfo()->m_guidCaller,
			pPacket->GetCallOfInfo()->m_SceneID,
			pPacket->GetCallOfInfo()->m_Pos.m_fX,
			pPacket->GetCallOfInfo()->m_Pos.m_fZ,
			pPacket->GetCallOfInfo()->m_uDuration);
		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		if(MyGetCurrentThreadID()!=pScene->m_ThreadID)
		{
			Assert( FALSE && "WGCallOfHumanHandler::Execute MyGetCurrentThreadID()!=pScene->m_ThreadID");
			return PACKET_EXE_CONTINUE;
		}

		const _CALLOF_INFO *pCallOfInfo = pPacket->GetCallOfInfo();
		pHuman->SetCallOfInfo(pCallOfInfo);

		g_pLog->FastSaveLog( LOG_FILE_1, "WGCallOfHumanHandler: CallerGUID=%u, SceneID=%d, Pos=%f,%f, Duration=%u",
			pPacket->GetCallOfInfo()->m_guidCaller,
			pPacket->GetCallOfInfo()->m_SceneID,
			pPacket->GetCallOfInfo()->m_Pos.m_fX,
			pPacket->GetCallOfInfo()->m_Pos.m_fZ,
			pPacket->GetCallOfInfo()->m_uDuration);
		return PACKET_EXE_CONTINUE ;
	}
	else
	{
		Assert(FALSE && "WGCallOfHumanHandler::Execute (!pPlayer->IsServerPlayer() && !pPlayer->IsGamePlayer())");
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
