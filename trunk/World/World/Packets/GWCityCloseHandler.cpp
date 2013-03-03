
/*
�Ϸ�����ס�ó��У��ر���PlayerGuid�ŵ���ʱ�ṹ�б���
����Ϣ15.WGCityClose,��ָ��Server
�Ƿ�������Ϣ6.WGCityError֪ͨServer

*/
#include "stdafx.h"
#include "GWCityClose.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "WGCityClose.h"
uint GWCityCloseHandler::Execute( GWCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

		_CITY_GUID	  CityGuid		= pPacket->GetCityGuid();
		GUID_t		  PlayerGuid	= pPacket->GetPlayerGuid();
		WGCityError MsgError;

		USER* pUser = g_pOnlineUser->FindUser( PlayerGuid ) ;
		if( pUser==NULL )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID=%X not find!", 
				PlayerGuid) ;
			return PACKET_EXE_CONTINUE ;
		}

		City* pDelCity = g_pCityManager->GetCityByGuid(CityGuid);
		if(!pDelCity)
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID world=%d poolpos=%d  not find!", 
				CityGuid.m_World, CityGuid.m_PoolPos) ;
			return PACKET_EXE_CONTINUE ;
		}

		if(pDelCity->IsLock())
		{//�Ѿ�����ɾ���˳�����
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID world=%d poolpos=%d  not find!", 
				CityGuid.m_World, CityGuid.m_PoolPos) ;
			return PACKET_EXE_CONTINUE ;
		}
		
		pDelCity->SetApplicantGuid(PlayerGuid);
		pDelCity->SetLock(TRUE);
		
		WGCityClose MsgToServer;
		MsgToServer.SetSceneID(pDelCity->GetSceneID());
		MsgToServer.SetPlayerID(pUser->GetPlayerID());
		pServerPlayer->SendPacket(&MsgToServer);
		
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
