#include "stdafx.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerManager.h"
#include "MailCenter.h"
#include "WGUpdateTitle.h"
#include "GuildManager.h"

using namespace Packets;

uint GWUpdateTitleHandler::Execute(GWUpdateTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );

    ID_t ServerID = g_Config.SceneID2ServerID(pUser->GetSceneID());
    ServerPlayer *pSPlayer = g_pServerManager->GetServerPlayer(ServerID);

	if( pUser==NULL )
	{//û�з����ڡ������û��б����Ӧ��GUID���û�
		Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "�Ҳ�������" );
		return PACKET_EXE_CONTINUE;
	}

    GuildID_t guild_id = pUser->GetGuildID();
    Guild* pGuild = g_pGuildManager->GetGuild(guild_id);
    if (pGuild)
    {
        //ֻ�а�������Ȩ�ް���������޸ĳƺ�
        if (pGuild->IsAuthorized(pUser->GetGUID() , GUILD_AUTHORITY_CHIEFTAIN))
        {
            USER* pTargetUser = g_pOnlineUser->FindUser(pPacket->GetGUID());
            if (!pTargetUser)
            {
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "��Ҫ�޸ĳƺŵ����%sĿǰ�����ߣ�����������ʱ�ٽ����޸ġ�", pTargetUser->GetName());
                g_pMailCenter->SendNormalMail(pTargetUser->GetName(), szMailContent);
            }

            //ת����Ϣ
            ID_t targetServerID = g_Config.SceneID2ServerID(pTargetUser->GetSceneID());
            ServerPlayer *pTargetSPlayer = g_pServerManager->GetServerPlayer(targetServerID);

            if (pTargetSPlayer)
            {
                WGUpdateTitle Msg;
                Msg.SetPlayerID(pTargetUser->GetPlayerID());
                Msg.SetTitleID(pPacket->GetTitleID());
                Msg.SetTitleType(pPacket->GetTitleType());
                Msg.SetTitleSize(pPacket->GetPacketSize());
                Msg.SetTitle(pPacket->GetTitle());
                pTargetSPlayer->SendPacket(&Msg);

                Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X Changed Title to Title=%s", 
                    pPacket->GetGUID(), pPacket->GetTitle() );
            }
        }
    }


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

return PACKET_EXE_ERROR;
}
