//GWCountryRequestKindomHandler.cpp
#include "stdafx.h"
#include "GWCountryRequestKindom.h"
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
#include "WGCountryReturn.h"


UINT GWCountryRequestKindomHandler::Execute( GWCountryRequestKindom* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetKindomRequestData()->m_Guid;

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryRequestKindomHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    //�ж���ҵĵȼ�������ȫ������ǰ500��������<=500�����Ƿ�Ϊ�������Ƿ���һ����Ŀ����Ϸ��
    TopList *pLevelList =  g_pTopListManager->GetTopList(LEVEL_TOP_LIST);
    if (pLevelList)
    {
        //INT nLevelIndex = pLevelList->GetSortIndex(guid, pUser->GetLevel());

        //ȫ������ǰ500��������<=500
        //if (nLevelIndex <= 500)
        {
            GuildID_t Guild_id = pUser->GetGuildID();

            Guild *pGuild =  g_pGuildManager->GetGuild(Guild_id);
            if (pGuild)
            {
                if (pGuild->IsAuthorized(guid, GUILD_AUTHORITY_CHIEFTAIN))
                {//�Ƿ�Ϊ����
                    
                    Country *pCountry = g_pCountryManager->GetCountry(pUser->GetCountry());
                    Assert(pCountry);

                    if (pCountry)
                    {
                        _COUNTRY *pInfo = pCountry->GetCountryInfo();
                        Assert(pInfo);

                        if (pInfo)
                        {
                            GUID_t idKindom = pInfo->m_GuoWang;

                            if (idKindom == INVALID_GUID)
                            {
                                g_pCountryManager->AppointPosition(pUser->GetCountry(), _COUNTRY::COUNTRY_GUOWANG, guid, pUser->GetName());   

                                WGCountryReturn Msg;
                                Msg.SetPlayerID( pUser->GetPlayerID() );
                                _COUNTRY_RETURN CountryReturn;
                                CountryReturn.m_ReturnType        = COUNTRY_RETURN_REQUEST_KINDOM;
                                Msg.SetCountryReturn( &CountryReturn );

                                Log::SaveLog( WORLD_LOGFILE, "GWCountryRequestKindomHandler...User GUID=%X Appoint Kindom!", 
                                    guid );

                                pServerPlayer->SendPacket( &Msg );
                                return PACKET_EXE_CONTINUE ;
                            }
                        }
                    }
                }
            }
        }
    }

    WGCountryReturn Msg;
    Msg.SetPlayerID( pUser->GetPlayerID() );
    _COUNTRY_RETURN CountryReturn;
    CountryReturn.m_ReturnType        = COUNTRY_RETURN_REQUEST_KINDOM_FAIL;
    Msg.SetCountryReturn( &CountryReturn );

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}