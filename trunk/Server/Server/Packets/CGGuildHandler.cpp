
#include "stdafx.h"
#include "CGGuild.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GWGuild.h"
#include "GCGuildError.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

uint	CGGuildHandler::Execute(CGGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	_GUILD_CGW_PACKET* pGuildPacket = pPacket->GetGuildPacket();

	GUID_t guid = pHuman->GetGUID();

	switch( pGuildPacket->m_uPacketType )
	{ // 这里可以做一些预判断和处理
	case GUILD_PACKET_CG_ASKLIST:
		{
		}
		break;
    case GUILD_PACKET_CG_LEVELUP:
        {
            GUILD_CGW_LEVELUP*  pInfo = (GUILD_CGW_LEVELUP*)pGuildPacket->GetPacket(GUILD_PACKET_CG_LEVELUP);

            if (pHuman->GetGuildID() == pInfo->m_GuildGUID)
            {
                INT nLevel = pInfo->m_GuildLevel;
                ++nLevel;
                Assert(nLevel < MAX_GUILD_LEVEL);

                //扣钱
                if (pHuman->GetMoney() >= (UINT)g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_Gold)
                {
                    INT nMoney  = pHuman->GetMoney() - g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_Gold;
                    pHuman->SetMoney(nMoney);
                    pInfo->m_GP = g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_Contribute;  
                }
                else
                {
                    g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X GuildLevelUP too low money", pHuman->GetGUID() );
                    return PACKET_EXE_ERROR;
                }
            }
        }
        break;
	case GUILD_PACKET_CG_CREATE:
		{//合法性验证
			if( pHuman->GetLevel()<40 )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X too low level", pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			if( pHuman->GetMoney()<500000 )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X too low money", pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			//if()
		}
		break;
	case GUILD_PACKET_CG_JOIN:
		{
		}
		break;
    case GUILD_PACKET_CG_INVITE_CONFIRM:
        {
        }
        break;
    case GUILD_PACKET_CG_INVITE:
        {
        }
        break;
	case GUILD_PACKET_CG_ASKINFO:
		{
            GUILD_CGW_ASKINFO*  pGuildAskInfo = (GUILD_CGW_ASKINFO*)pGuildPacket->GetPacket(GUILD_PACKET_CG_ASKINFO);
            pGuildAskInfo->m_GuildGUID = pHuman->GetGuildID();
            //校验密码
            if (!pHuman->__GetProtectFlag())
            {
                if (pHuman->__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_GUILD))
                {
                    if (!pHuman->ComparePasswd(pGuildAskInfo->m_Password))
                    {
                        GCGuildError Msg;
                        Msg.SetErrorCode( GUILD_ERROR_PASSWORD );
                        pGamePlayer->SendPacket( &Msg );

                        return PACKET_EXE_CONTINUE;
                    }
                }
            }
		}
		break;
    case GUILD_PACKET_CG_CREATE_FAMILY:
        {
        }
        break;
	case GUILD_PACKET_CG_APPOINT:
		{
		}
		break;
	case GUILD_PACKET_CG_ADJUSTAUTHORITY:
		{
		}
		break;
	case GUILD_PACKET_CG_RECRUIT:
		{
		}
		break;
	case GUILD_PACKET_CG_EXPEL:
		{
		}
		break;
	case GUILD_PACKET_CG_WITHDRAW:
		{
		}
		break;
	case GUILD_PACKET_CG_DEPOSIT:
		{
		}
		break;
	case GUILD_PACKET_CG_LEAVE:
		{
		}
		break;
	case GUILD_PACKET_CG_DISMISS:
		{
		}
		break;
	case GUILD_PACKET_CG_DEMISE:
		{
		}
		break;

    case GUILD_PACKET_CG_ADD_FAMILY_USER:
        {

        }
        break;
    case GUILD_PACKET_CG_REMOVE_FAMILY_USER:
        {

        }
        break;
    case GUILD_PACKET_CG_MODIFY_DESC:
        {
        }
        break;
    case GUILD_PACKET_CG_FAMILY_DISMISS:
        {

        }
        break;
	default:
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
	pMsg->SetGUID(guid);
	pMsg->SetGuildPacket(pGuildPacket);
	pGuildPacket = pMsg->GetGuildPacket();
	pGuildPacket->m_uPacketType += GUILD_PACKET_CG_GW_SEPARATOR + 1;

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X", pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

return PACKET_EXE_ERROR;
}