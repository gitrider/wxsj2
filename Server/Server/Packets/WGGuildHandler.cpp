/********************************************************************************
 *	文件名：	WGGuildHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\WGGuildHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	12:09
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGGuild.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCGuild.h"

uint WGGuildHandler::Execute( WGGuild* pPacket, Player* pPlayer )
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

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildHandler: ServerPlayer (Type=%d) ",
			pPacket->GetGuildPacket()->m_uPacketType );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

        GUILD_PACKET_TYPE PacketType = (GUILD_PACKET_TYPE)pPacket->GetGuildPacket()->m_uPacketType;

        switch( PacketType )
        {
        case GUILD_PACKET_WG_CONFIRM_GUILD_INVITE:
            {
            }
            break;
        case GUILD_PACKET_WG_MODIFY_TITLE_NAME:
            {
                //GUILD_CGW_MODIFY_TITLE_NAME* pTitleName = (GUILD_CGW_MODIFY_TITLE_NAME*)(pPacket->GetGuildPacket()->GetPacket(GUILD_PACKET_WG_MODIFY_TITLE_NAME));
                //if (pTitleName->m_NameSize)
                //{
                //    pHuman->SetGuildTitleName(pTitleName->m_szTitleName, pTitleName->m_NameSize);
                //    pHuman->UpdateTitlesToClient();
                //}
            }
            break;
        }

		GCGuild Msg;
		Msg.SetGuildPacket( pPacket->GetGuildPacket() );                                                           
		Msg.GetGuildPacket()->m_uPacketType += GUILD_PACKET_WG_GC_SEPARATOR - GUILD_PACKET_GW_WG_SEPARATOR;
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildHandler: GamePlayer (Type=%d) ",
			pPacket->GetGuildPacket()->m_uPacketType );
	}
	else
	{
		Assert(FALSE);
	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
