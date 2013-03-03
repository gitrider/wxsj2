//CGCountryDonativeHandler.cpp
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "CGCountryDonative.h"
#include "GWCountryDonative.h"


UINT CGCountryDonativeHandler::Execute( CGCountryDonative* pPacket, Player* pPlayer )
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

    INT nGold = pHuman->GetMoney();
    nGold -= pPacket->GetDonativeGold();

    if (nGold >=0)
    {
        pHuman->SetMoney(nGold);

        GWCountryDonative *pMsg = (GWCountryDonative *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_DONATIVE);
        pMsg->SetDonativeGold(pPacket->GetDonativeGold());
        pMsg->SetGuid(pHuman->GetGUID());
        g_pServerManager->SendPacket( pMsg, INVALID_ID );
    }
    else
    {
        g_pLog->FastSaveLog( LOG_FILE_1, "CGCountryDonativeHandler: Player Guild=%X don't have enough money!", pHuman->GetGUID());
    }


    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
