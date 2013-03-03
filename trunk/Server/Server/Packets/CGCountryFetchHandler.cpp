//CGCountryFetchHandle.cpp
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "CGCountryFetch.h"
#include "GWCountryFetch.h"


UINT CGCountryFetchHandler::Execute( CGCountryFetch* pPacket, Player* pPlayer )
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

    GWCountryFetch *pMsg = (GWCountryFetch *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_FETCH);
    pMsg->SetGold(pPacket->GetDonativeGold());
    pMsg->SetGuid(pHuman->GetGUID());
    g_pServerManager->SendPacket( pMsg, INVALID_ID );

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
