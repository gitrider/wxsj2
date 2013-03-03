//GWCountryRemoveHandler.cpp
#include "stdafx.h"
#include "GWCountryRemove.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "WGCountryReturn.h"


UINT GWCountryRemoveHandler::Execute( GWCountryRemove* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetCountryRemoveData()->m_KindomGuid;

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryRemoveHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();

    {
        USER* pTargetUser = g_pOnlineUser->FindUser( pPacket->GetCountryRemoveData()->m_szTargetName );
        if( pTargetUser == NULL )
        {
            Log::SaveLog( WORLD_LOGFILE, "GWCountryRemoveHandler...TargetUser GUID=%X not find!", 
                pTargetUser->GetGUID() );
            return PACKET_EXE_CONTINUE;
        }

        if (pTargetUser->GetCountry() == nCountry)//同一个国家
        {
            g_pCountryManager->AppointPosition(pUser->GetCountry() , pPacket->GetCountryRemoveData()->m_nOfficialPosition, INVALID_GUID, "");      

            WGCountryReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _COUNTRY_RETURN CountryReturn;
            CountryReturn.m_ReturnType        = COUNTRY_RETURN_REMOVE;
            CountryReturn.m_cPosition         = pPacket->GetCountryRemoveData()->m_nOfficialPosition;
            CountryReturn.m_cCountry          = (CHAR)nCountry;
            Msg.SetCountryReturn( &CountryReturn );
            pServerPlayer->SendPacket( &Msg );

            Log::SaveLog( WORLD_LOGFILE, "GWCountryRemoveHandler...User GUID=%X Appoint !", 
                guid );

        }
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
