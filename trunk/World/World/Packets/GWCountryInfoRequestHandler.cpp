//GWCountryInfoRequestHandler.cpp
#include "stdafx.h"
#include "GWCountryInfoRequest.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "Guild.h"
#include "GuildManager.h"
#include "TopListManager.h"
#include "TopList.h"
#include "GameDefine_Guild.h"
#include "WGCountryInfoConfirm.h"


UINT GWCountryInfoRequestHandler::Execute( GWCountryInfoRequest* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryInfoRequestHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    _COUNTRY country;
    g_pCountryManager->GetCountryInfo(pUser->GetCountry(), country);   

    WGCountryInfoConfirm Msg;
    Msg.SetPlayerID( pUser->GetPlayerID() );

    COUNTRY_CGW_COUNTRY_INFO info;
    info.m_CountryID    =   pUser->GetCountry();
    memcpy(&info.m_CountryInfo, &country, sizeof(_COUNTRY));
    Msg.SetCountryInfo( &info );
    pServerPlayer->SendPacket( &Msg );

    Log::SaveLog( WORLD_LOGFILE, "GWCountryInfoRequestHandler...User GUID=%X send CountryInfo!", 
        guid );

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}