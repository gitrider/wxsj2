//CGCountryRequestKindomHandler.cpp
#include "stdafx.h"
#include "CGCountryRequestKindom.h"
#include "GWCountryRequestKindom.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

UINT CGCountryRequestKindomHandler::Execute( CGCountryRequestKindom* pPacket, Player* pPlayer )
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

    if (pHuman->GetMoney() > KINDOM_GOLD)
    {
        INT nGold = pHuman->GetMoney();
        nGold -= KINDOM_GOLD;
        pHuman->SetMoney(nGold);

        GWCountryRequestKindom *pMsg = (GWCountryRequestKindom *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_REQUEST_KINDOM);
        pMsg->SetGetKindomRequestData(pPacket->GetKindomRequestData());
        g_pServerManager->SendPacket( pMsg, INVALID_ID );

        g_pLog->FastSaveLog( LOG_FILE_1, "CGCountryRequestKindomHandler: Player Guid=%X want to be Kindom subGold=%d !", pHuman->GetGUID(), KINDOM_GOLD);
    }
    

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
