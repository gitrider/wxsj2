//GWCountryFetchHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"

#include "GWCountryFetch.h"
#include "WGCountryFetch.h"


UINT GWCountryFetchHandler::Execute( GWCountryFetch* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryFetchHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }


    INT nCountry = pUser->GetCountry();
    _COUNTRY *pCountry = g_pCountryManager->GetCountryInfo(nCountry);

    if (pCountry)
    {
        pCountry->SubGold(pPacket->GetGold());
        Log::SaveLog( WORLD_LOGFILE, "GWCountryFetchHandler...User GUID=%X FetchGold = %d", 
            guid, pPacket->GetGold());

        WGCountryFetch Msg;
        Msg.SetPlayerID( pUser->GetPlayerID() );
        Msg.SetGold( pPacket->GetGold() );

        pServerPlayer->SendPacket( &Msg );
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
