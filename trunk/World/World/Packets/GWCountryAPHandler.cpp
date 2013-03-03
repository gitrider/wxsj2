//GWCountryAPHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"

#include "GWCountryAP.h"


UINT GWCountryAPHandler::Execute( GWCountryAP* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryAPHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();
    Country *pCountry = g_pCountryManager->GetCountry(nCountry);

    if (pCountry)
    {
        if (pUser->GetGUID() == pCountry->GetCountryInfo()->m_GuoWang)
        {
            BYTE cType  = pPacket->GetType();
            INT nAP     = pPacket->GetAP();

            switch (cType)
            {
            case 0:
                pCountry->AddActionPoint(nAP);
                break;
            case 1:
                pCountry->SubActionPoint(nAP);
                break;
            }

            Log::SaveLog( WORLD_LOGFILE, "GWCountryAPHandler...User GUID=%X ChangeAP = %d", 
                guid, nAP);
        }
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
