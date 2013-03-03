//GWCountryVoteRequestHandler.cpp
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

#include "GWCountryVoteRequest.h"
#include "WGCountryVoteConfirm.h"

extern TimeManager*	g_pTimeManager;


UINT GWCountryVoteRequestHandler::Execute( GWCountryVoteRequest* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryVoteRequestHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();
    Country *pCountry = g_pCountryManager->GetCountry(nCountry);

    if (pCountry->GetStartTime() != 0)
    {
        INT nCurCnt = 0, nAgreeCnt = 0, nPercent = 0;
        pCountry->GetVoteResult(nCurCnt, nAgreeCnt, nPercent);

        DWORD nTime     = pCountry->GetStartTime();
        INT nLeftTime   = (nTime + g_Config.m_ConfigInfo.m_nVoteTime*60*1000 ) - g_pTimeManager->CurrentTime();

        WGCountryVoteConfirm Msg;
        Msg.SetPlayerID( pUser->GetPlayerID() );
        Msg.SetCurNum(nCurCnt);
        Msg.SetGuid(pUser->GetGUID());
        Msg.SetPercent(nPercent);
        Msg.SetTime(nLeftTime);
        pServerPlayer->SendPacket( &Msg );

        Log::SaveLog( WORLD_LOGFILE, "GWCountryVoteRequestHandler...User GUID=%X VoteOn Begin!", 
            guid );
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

        return PACKET_EXE_ERROR ;
}
