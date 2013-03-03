//GWCountryDonativeHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"

#include "GWCountryDonative.h"


UINT GWCountryDonativeHandler::Execute( GWCountryDonative* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryDonativeHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }


    INT nCountry = pUser->GetCountry();
    _COUNTRY *pCountry = g_pCountryManager->GetCountryInfo(nCountry);

    if (pCountry)
    {
        pCountry->AddGold(pPacket->GetDonativeGold());
        Log::SaveLog( WORLD_LOGFILE, "GWCountryDonativeHandler...User GUID=%X AddGold = %d", 
            guid, pPacket->GetDonativeGold());
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
