//GWCountryCollectHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"

#include "GWCountryCollect.h"
#include "WGCountryCollect.h"


UINT GWCountryCollectHandler::Execute( GWCountryCollect* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetCollectData()->m_Guid;

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryCollectHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }


    INT nCountry = pUser->GetCountry();
    _COUNTRY *pCountry = g_pCountryManager->GetCountryInfo(nCountry);
    if (pCountry)
    {
        // 是否国王
        if (pCountry->m_GuoWang == pUser->GetGUID())
        {
            for (INT i=0;  i< MAX_WORLD_USER; ++i)
            {
                GUID_t guid = pCountry->m_aUser[i];
                USER* pCountryUser = g_pOnlineUser->FindUser( guid );

                if (pCountryUser)
                {
                    if (pCountry->m_GuoWang != guid)
                    {
                        //转发
                        ID_t ServerID = g_Config.SceneID2ServerID(pCountryUser->GetSceneID());
                        ServerPlayer *pCountrySPlayer = g_pServerManager->GetServerPlayer(ServerID);

                        WGCountryCollect Msg;
                        Msg.SetPlayerID( pCountryUser->GetPlayerID() );
                        Msg.SetCollectData( pPacket->GetCollectData() );

                        pCountrySPlayer->SendPacket( &Msg );

                        Log::SaveLog( WORLD_LOGFILE, "GWCountryCollectHandler...User GUID=%X send GWCountryCollect to player(%s)!", 
                            guid , pCountryUser->GetName());
                    }
                }
            }
        }
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
