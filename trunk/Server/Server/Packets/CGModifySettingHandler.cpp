

#include "stdafx.h"
#include "CGModifySetting.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCRetSetting.h"
#include "PacketFactoryManager.h"
#include "GWRelation.h"
#include "ServerManager.h"

using namespace Packets;


uint CGModifySettingHandler::Execute( CGModifySetting* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;


	pHuman->GetDB()->SetSettingData( pPacket->GetType(), pPacket->GetValue() ) ;

	if ( (SETTING_TYPE)pPacket->GetType() == SETTING_TYPE_GAME )
	{
		GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
		GW_RELATION* pRelation = pMsg->GetRelation();
		pRelation->CleanUp();
		pRelation->m_Type = REQ_MODIFYSETTINGS;
		pRelation->SetGUID( pHuman->GetGUID() );
		pRelation->m_Settings.SetSettings( pPacket->GetValue()->m_SettingData & 0xffff );

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGModifySettingHandler::SOCKET=%d Type=%d Value_Type=%d Value_Data=%d OK!",
		pPlayer->GetSocket()->getSOCKET(),
		pPacket->GetType(),
		pPacket->GetValue()->m_SettingType,
		pPacket->GetValue()->m_SettingData ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
