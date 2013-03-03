
#include "stdafx.h"
#include "GWCityApplyNewCity.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "GuildManager.h"
#include "WGCityApplyNewCity.h"

uint GWCityApplyNewCityHandler::Execute( GWCityApplyNewCity* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		//��ڳ���ID
		SceneID_t	PortSceneID		=	pPacket->GetSceneID();
		//������GUID
		GUID_t		ApplicantGuid	=	pPacket->GetPlayerGuid();

		WGCityError MsgError;

		USER* pUser = g_pOnlineUser->FindUser( ApplicantGuid ) ;
		if( pUser==NULL )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User GUID=%X not find!", 
				ApplicantGuid ) ;

			return PACKET_EXE_CONTINUE ;
		}

		//����Ҫ�ж���ǰ�����Ƿ���Ȩ���������У���ǰ�����Ƿ����ʸ��������
		GuildID_t guildID = pUser->GetGuildID();
		Guild* pGuild = g_pGuildManager->GetGuild( guildID );

		//if(!pGuild)
		//{
		//	Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User Guild not find! guildID = %d", 
		//		guildID ) ;
		//	return PACKET_EXE_CONTINUE ;
		//}

		//�Ϸ�
		//֪ͨServerȥload���г��������س��г�����
		WGCityApplyNewCity MsgToServer;
		MsgToServer.SetSceneID(PortSceneID);
		MsgToServer.SetPlayerGuid(ApplicantGuid);
		pServerPlayer->SendPacket( &MsgToServer );

		Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User GUID=%X Finished", 
			ApplicantGuid ) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
