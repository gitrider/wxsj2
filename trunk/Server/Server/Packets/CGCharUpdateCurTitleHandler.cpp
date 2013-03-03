//CGCharUpdateCurTitleHandler.cpp
#include "stdafx.h"
#include "CGCharUpdateCurTitle.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWUpdateTitle.h"
#include "GCCharAllTitles.h"

uint CGCharUpdateCurTitleHandler::Execute( CGCharUpdateCurTitle* pPacket, Player* pPlayer )
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

	BYTE		titleType	= pPacket->getTitleType();
	INT	    	titleID 	= pPacket->getTitleID();

	Assert(	titleType>=_TITLE::GUOJIA_TITLE&& titleType< _TITLE::MAX_NUM_TITLE);

    switch(titleType)
    {
    case 	_TITLE::GUOJIA_TITLE :
        {
            pHuman->SetCurCountryTitle(titleID);
            pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_GUOJIA_TITLE);
        }
        break;
    case	_TITLE::BANGPAI_TITLE:
        {
            //修改帮会称号，转发GL
            GWUpdateTitle* pMsg = (GWUpdateTitle*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_UPDATETITLE)) ;
            pMsg->SetGUID(pPacket->getGuid());
            pMsg->SetTitleID(pPacket->getTitleID());
            pMsg->SetTitleType(pPacket->getTitleType());
            pMsg->SetTitleSize(pPacket->getTitleSize());
            pMsg->SetTitle(pPacket->GetTitleName());

            g_pServerManager->SendPacket( pMsg, INVALID_ID );
            g_pLog->FastSaveLog(LOG_FILE_1, "CGCharUpdateCurTitleHandler: Player(Guild=%d) update Player(Guid=%d)'s TitleName!",
                pHuman->GetGUID(), pPacket->getGuid());
        }
        break;
    case	_TITLE::WANFA_TITLE:
        {
            pHuman->SetCurNormalTitle(titleID);
            pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_WANFA_TITLE);
        }
        break;
    }

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
