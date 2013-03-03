/********************************************************************************
*	文件名：	WGCountryCollectHandler.cpp
*	全路径：	d:\Prj\Server\Server\Packets\WGCountryCollectHandler.cpp
*	创建人：	Amos
*	创建时间：	2008 年 5 月 26 日	16:45
*
*	功能说明：	
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "TimeManager.h"
#include "GCGuildReturn.h"
#include "HumanItemLogic.h"
#include "GCItemInfo.h"

#include "WGCountryCollect.h"
#include "GCCountryCollect.h"

uint WGCountryCollectHandler::Execute( WGCountryCollect* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    PlayerID_t PlayerID = pPacket->GetPlayerID();
    GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
    if( pGamePlayer==NULL )
    {
        Assert(FALSE);
        return PACKET_EXE_CONTINUE;
    }

    Obj_Human* pHuman = pGamePlayer->GetHuman();
    Assert( pHuman );

    Scene* pScene = pHuman->getScene();
    if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

    if( pPlayer->IsServerPlayer() )
    {//服务器收到世界服务器发来的数据
        Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
        pScene->SendPacket( pPacket, PlayerID );
        g_pLog->FastSaveLog( LOG_FILE_1, "WGCountryCollectHandler:" );

        return PACKET_EXE_NOTREMOVE;
    }
    else if( pPlayer->IsGamePlayer() )
    {//场景收到Cache里的消息
        Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

        GCCountryCollect Msg;
        Msg.SetCollectData(pPacket->GetCollectData());

        pGamePlayer->SendPacket( &Msg );
    }
    else
    {
        Assert(FALSE);
    }

    return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR;
}
