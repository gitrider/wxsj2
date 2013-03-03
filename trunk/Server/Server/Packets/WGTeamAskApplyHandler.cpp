

#include "stdafx.h"
#include "WGTeamAskApply.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCTeamAskApply.h"


uint WGTeamAskApplyHandler::Execute( WGTeamAskApply* pPacket, Player* pPlayer )
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
	{//�������յ��������������������
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamAskApplyHandler: ServerPlayer sGUID=%X",
			pPacket->GetSourGUID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//�����յ�Cache�����Ϣ
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCTeamAskApply tMsg;
		tMsg.SetSourGUID( pPacket->GetSourGUID() );
		tMsg.SetDestGUID( pPacket->GetDestGUID() );
		tMsg.SetSourName( pPacket->GetSourName() );
		tMsg.SetDestName( pPacket->GetDestName() );
		tMsg.SetFamily( pPacket->GetFamily() );
		tMsg.SetScene(g_pSceneManager->GetSceneResID(pPacket->GetScene()));
		tMsg.SetLevel( pPacket->GetLevel() );
		tMsg.SetDataID( pPacket->GetSex() );

		//if ( pPacket->GetScene() != pScene->SceneID() )
		//{
		//	tMsg.SetDetailFlag( 0 );
		//}
		//else
		{
			tMsg.SetDetailFlag( TRUE );

			GamePlayer* pSourGamePlayer = g_pPlayerPool->GetPlayer( pPacket->GetSourPlayerID() );
			if( pSourGamePlayer==NULL )
			{
				Assert(FALSE);
				tMsg.SetDetailFlag( 0 );
			}
			else
			{
				Obj_Human* pSourHuman = pSourGamePlayer->GetHuman();
				Assert( pSourHuman );
				
				tMsg.SetWeaponID( pSourHuman->GetEquipID(HEQUIP_MAINHAND) );
				tMsg.SetAssihandID( pSourHuman->GetEquipID(HEQUIP_ASSIHAND) );

				tMsg.SetCapID( pSourHuman->GetEquipID(HEQUIP_HEAD) );
				tMsg.SetScapularID( pSourHuman->GetEquipID(HEQUIP_SHOULDER) );
				tMsg.SetArmourID( pSourHuman->GetEquipID(HEQUIP_BACK) );
				tMsg.SetCuffID( pSourHuman->GetEquipID(HEQUIP_HAND) );
				tMsg.SetFootID( pSourHuman->GetEquipID(HEQUIP_FEET) );
				tMsg.SetFaceModel( pSourHuman->__GetFaceModel() );
				tMsg.SetHairColor( pSourHuman->__GetHairColor() );
				tMsg.SetHairModel( pSourHuman->__GetHairModel() );				
			}
		}

		pGamePlayer->SendPacket( &tMsg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamAskApplyHandler: GamePlayer sGUID=%X, dGUID=%X ",
			pPacket->GetSourGUID(), pPacket->GetDestGUID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
