//CGCountryVoteOnHandler.cpp
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "CGCountryVoteOn.h"
#include "GWCountryVoteOn.h"


UINT CGCountryVoteOnHandler::Execute( CGCountryVoteOn* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
    Assert( pGamePlayer ) ;

    Obj_Human* pHuman = pGamePlayer->GetHuman() ;
    Assert( pHuman ) ;

    Scene* pScene = pHuman->getScene() ;
    if( pScene==NULL )
    {
        Assert(FALSE) ;
        return PACKET_EXE_ERROR ;
    }

    //检查线程执行资源是否正确
    Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

    if (pHuman->GetLevel() >= 50)
    {
        GWCountryVoteOn *pMsg = (GWCountryVoteOn *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_VOTEON);
        pMsg->SetResult(pPacket->GetResult()) ;
        if (pHuman->GetGUID() != pPacket->GetGuid())
        {
            pMsg->SetGuid(pHuman->GetGUID()) ;
            g_pLog->FastSaveLog( LOG_FILE_1, "CGCountryVoteOnHandler: Player Guid=%X delate Player Guid error !", pHuman->GetGUID());
        }
        else
        {
            pMsg->SetGuid(pPacket->GetGuid());
        }
        g_pServerManager->SendPacket( pMsg, INVALID_ID );
    }
    else
    {
        g_pLog->FastSaveLog( LOG_FILE_1, "CGCountryVoteOnHandler: Player Guid=%X Level less than 50 !", pHuman->GetGUID());
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
