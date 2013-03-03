//GWCountryAPHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "GuildManager.h"
#include "MailCenter.h"

#include "GWCountryDelateRequest.h"
#include "WGCountryDelateConfirm.h"


UINT GWCountryDelateRequestHandler::Execute( GWCountryDelateRequest* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryDelateRequestHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();
    Country *pCountry = g_pCountryManager->GetCountry(nCountry);

    INT nNum = pCountry->GetGuildMasterVote();
    
    if (nNum < g_Config.m_ConfigInfo.m_nDelateNum && pCountry->GetStartTime() == 0)//还可以弹劾
    {
        WGCountryDelateConfirm Msg;
        Msg.SetPlayerID( pUser->GetPlayerID() );
        Msg.SetCurNum(nNum);
        Msg.SetGuid(pUser->GetGUID());
        Msg.SetNameSize((BYTE)strlen(pCountry->GetKindomName()));
        Msg.SetName(pCountry->GetKindomName());

        pServerPlayer->SendPacket( &Msg );
    }
    else
    {
        CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
        sprintf(szMailContent, "目前弹劾状态为%d/%d。已经发起弹劾投票！",g_Config.m_ConfigInfo.m_nDelateNum, g_Config.m_ConfigInfo.m_nDelateNum);
        g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
    }


    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
