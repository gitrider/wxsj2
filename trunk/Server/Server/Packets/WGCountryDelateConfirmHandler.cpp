/********************************************************************************
*	�ļ�����	WGCountryDelateConfirmHandler.cpp
*	ȫ·����	d:\Prj\Server\Server\Packets\WGCountryDelateConfirmHandler.cpp
*	�����ˣ�	Amos
*	����ʱ�䣺	2008 �� 5 �� 28 ��	16:45
*
*	����˵����	
*	�޸ļ�¼��
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

#include "WGCountryDelateConfirm.h"
#include "GCCountryDelateBegin.h"

uint WGCountryDelateConfirmHandler::Execute( WGCountryDelateConfirm* pPacket, Player* pPlayer )
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
    {//�������յ��������������������
        Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
        pScene->SendPacket( pPacket, PlayerID );
        return PACKET_EXE_NOTREMOVE;
    }
    else if( pPlayer->IsGamePlayer() )
    {//�����յ�Cache�����Ϣ
        Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

        GCCountryDelateBegin Msg;
        Msg.SetCurNum(pPacket->GetCurNum());
        Msg.SetNameSize(pPacket->GetNameSize());
        Msg.SetName(pPacket->GetName());

        pGamePlayer->SendPacket( &Msg );

        g_pLog->FastSaveLog( LOG_FILE_1, "WGCountryDelateConfirmHandler: Send Delate Begin GUID=%X", pHuman->GetGUID() );
    }
    else
    {
        Assert(FALSE);
    }

    return PACKET_EXE_CONTINUE;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR;
}
