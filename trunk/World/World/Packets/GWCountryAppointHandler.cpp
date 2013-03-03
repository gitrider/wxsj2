//GWCountryAppointHandler.cpp
#include "stdafx.h"
#include "GWCountryAppoint.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "WGCountryReturn.h"


UINT GWCountryAppointHandler::Execute( GWCountryAppoint* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetCountryAppointData()->m_KindomGuid;

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryAppointHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();

    {
        USER* pTargetUser = g_pOnlineUser->FindUser( pPacket->GetCountryAppointData()->m_szTargetName );
        if( pTargetUser == NULL )
        {
            Log::SaveLog( WORLD_LOGFILE, "GWCountryAppointHandler...TargetUser Name=%s not find!", pPacket->GetCountryAppointData()->m_szTargetName);

            WGCountryReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _COUNTRY_RETURN CountryReturn;
            CountryReturn.m_ReturnType        = COUNTRY_ERROR_USER_NOT_FIND;
            Msg.SetCountryReturn( &CountryReturn );
            pServerPlayer->SendPacket( &Msg );
            return PACKET_EXE_CONTINUE;
        }

        if (pTargetUser->GetCountry() == nCountry)//同一个国家
        {
            g_pCountryManager->AppointPosition(nCountry, pPacket->GetCountryAppointData()->m_nOfficialPosition, pTargetUser->GetGUID(), pPacket->GetCountryAppointData()->m_szTargetName);      

            WGCountryReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _COUNTRY_RETURN CountryReturn;
            CountryReturn.m_ReturnType        = 
                COUNTRY_RETURN_APPOINT;
            CountryReturn.m_cPosition         = pPacket->GetCountryAppointData()->m_nOfficialPosition;
            CountryReturn.m_cCountry          = (CHAR)nCountry;
            if (pPacket->GetCountryAppointData()->m_cNameSize > 0)
            {
                strncpy((CHAR*)CountryReturn.m_szName,pPacket->GetCountryAppointData()->m_szTargetName, MAX_CHARACTER_NAME);
            }
            
            Msg.SetCountryReturn( &CountryReturn );

            Log::SaveLog( WORLD_LOGFILE, "GWCountryAppointHandler...User GUID=%X Appoint !", 
                guid );

            pServerPlayer->SendPacket( &Msg );
        }
        else
        {
            WGCountryReturn Msg;
            Msg.SetPlayerID( pUser->GetPlayerID() );
            _COUNTRY_RETURN CountryReturn;
            CountryReturn.m_ReturnType        = COUNTRY_ERROR_COUNTRY_ID;
            Msg.SetCountryReturn( &CountryReturn );
            Log::SaveLog( WORLD_LOGFILE, "GWCountryAppointHandler...User GUID=%X ContryID is error !", guid );
            pServerPlayer->SendPacket( &Msg );
        }
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
