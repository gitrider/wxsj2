//CGCountryAppointHandler.cpp
#include "stdafx.h"
#include "CGCountryAppoint.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

UINT CGCountryAppointHandler::Execute( CGCountryAppoint* pPacket, Player* pPlayer )
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

    CGCountryAppoint *pMsg = (CGCountryAppoint *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_APPOINT);
    pMsg->SetCountryAppointData(pPacket->GetCountryAppointData());
    g_pServerManager->SendPacket( pMsg, INVALID_ID );

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
