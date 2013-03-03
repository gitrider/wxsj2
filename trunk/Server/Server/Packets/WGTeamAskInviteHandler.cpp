

#include "stdafx.h"
#include "WGTeamAskInvite.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCTeamAskInvite.h"



uint WGTeamAskInviteHandler::Execute( WGTeamAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		Assert(FALSE);
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

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamAskInviteHandler: ServerPlayer GUID=%X ",
			pPacket->GetGUID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCTeamAskInvite tMsg;
		tMsg.SetGUID( pPacket->GetGUID() );
//		tMsg.SetMemberCount( pPacket->GetMemberCount() );

		for( INT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			const WGTeamAskInvite::InviterInfo* pwInfo = pPacket->GetInviterInfo(i);
			GCTeamAskInvite::InviterInfo gInfo;

			gInfo.m_NickSize = pwInfo->m_NickSize;
			strncpy( (CHAR*)gInfo.m_szNick, pwInfo->m_szNick, gInfo.m_NickSize );
			gInfo.m_uFamily = pwInfo->m_uFamily;
			//gInfo.m_Scene = pwInfo->m_Scene;
			gInfo.m_Scene = g_pSceneManager->GetSceneResID(pwInfo->m_Scene);
			gInfo.m_Level = pwInfo->m_Level;

			gInfo.m_uDataID = pwInfo->m_uDataID;

			//if ( gInfo.m_Scene != pScene->SceneID() )
			//{
			//	gInfo.m_DetailFlag = 0;
			//}
			//else
			{
				gInfo.m_DetailFlag = TRUE;

				GamePlayer* pwGamePlayer = g_pPlayerPool->GetPlayer( pwInfo->m_PlayerID );
				if( pwGamePlayer==NULL )
				{
					//Assert(FALSE);
					gInfo.m_DetailFlag = 0;
					tMsg.AddInviterInfo( gInfo );
					continue;
				}

				Obj_Human* pwHuman = pwGamePlayer->GetHuman();
				Assert( pwHuman );

				
				gInfo.m_WeaponID = pwHuman->GetEquipID(HEQUIP_MAINHAND);
				gInfo.m_AssihandID = pwHuman->GetEquipID(HEQUIP_ASSIHAND);

				gInfo.m_CapID = pwHuman->GetEquipID(HEQUIP_HEAD);
				gInfo.m_ScapularID = pwHuman->GetEquipID(HEQUIP_SHOULDER);
				gInfo.m_ArmourID = pwHuman->GetEquipID(HEQUIP_BACK);
				gInfo.m_CuffID = pwHuman->GetEquipID(HEQUIP_HAND);
				gInfo.m_FootID = pwHuman->GetEquipID(HEQUIP_FEET);
				gInfo.m_uFaceMeshID = pwHuman->__GetFaceModel();
				gInfo.m_uHairColor = pwHuman->__GetHairColor();
				gInfo.m_uHairMeshID = pwHuman->__GetHairModel();
			}

			tMsg.AddInviterInfo( gInfo );
		}

		pGamePlayer->SendPacket( &tMsg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamAskInviteHandler: GamePlayer GUID=%X ",
			pPacket->GetGUID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
