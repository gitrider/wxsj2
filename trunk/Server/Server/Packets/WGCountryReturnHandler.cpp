/********************************************************************************
*	文件名：	WGCountryReturnHandler.cpp
*	全路径：	d:\Prj\Server\Server\Packets\WGCountryReturnHandler.cpp
*	创建人：	Amos
*	创建时间：	2008 年 4 月 15 日	16:45
*
*	功能说明：	
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGCountryReturn.h"
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
#include "GCCountryReturn.h"

uint WGCountryReturnHandler::Execute( WGCountryReturn* pPacket, Player* pPlayer )
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

        g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler: ServerPlayer (ReturnType=%d) ",
            pPacket->GetCountryReturn()->m_ReturnType );

        return PACKET_EXE_NOTREMOVE;
    }
    else if( pPlayer->IsGamePlayer() )
    {//场景收到Cache里的消息
        Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

        COUNTRY_RETURN_TYPE ReturnType = (COUNTRY_RETURN_TYPE)pPacket->GetCountryReturn()->m_ReturnType;

        switch( ReturnType )
        {
        case COUNTRY_RETURN_REQUEST_KINDOM:
            {
            }
            break;
        case COUNTRY_RETURN_APPOINT:
            {
            }
            break;
        case COUNTRY_RETURN_REMOVE:
            {
            }
            break;
        case COUNTRY_ERROR_COUNTRY_ID:
            {
            }
            break;
        case COUNTRY_ERROR_USER_NOT_FIND:
            {
            }
            break;
        case COUNTRY_RETURN_DELATE:
            {
                INT nGold = pHuman->GetMoney();
                nGold -= DELATE_GOLD;
                if (nGold < 0)
                    nGold = 0;

                pHuman->SetMoney(nGold);

                g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler:(COUNTRY_RETURN_DELATE) Human Guid=%X SubMoney=%d ",
                                     pHuman->GetGUID(), DELATE_GOLD );
            }
            break;
        case COUNTRY_RETURN_DELATE_SCUESS:
            {
                INT nGold = pHuman->GetMoney();
                nGold += DELATE_GOLD;

                pHuman->SetMoney(nGold);

                g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler:(COUNTRY_RETURN_DELATE_SCUESS) Human Guid=%X GiveBack Money=%d ",
                    pHuman->GetGUID(), DELATE_GOLD );
            }
            break;
        case COUNTRY_RETURN_REQUEST_KINDOM_FAIL:
            {
                INT nGold = pHuman->GetMoney();
                nGold += KINDOM_GOLD;

                pHuman->SetMoney(nGold);

                g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler:(REQUEST_KINDOM_FAIL) Human Guid=%X GiveBack Money=%d ",
                    pHuman->GetGUID(), KINDOM_GOLD );
            }
            break;
        }

        GCCountryReturn Msg;
        Msg.SetCountryReturn( pPacket->GetCountryReturn() );

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
