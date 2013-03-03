/********************************************************************************
 *	文件名：	WGGuildReturnHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\WGGuildReturnHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	17:12
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGGuildReturn.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "TimeManager.h"
#include "GCGuildReturn.h"
#include "HumanItemLogic.h"
#include "GCNotifyEquip.h"
#include "GWGuild.h"
#include "PacketFactoryManager.h"

uint WGGuildReturnHandler::Execute( WGGuildReturn* pPacket, Player* pPlayer )
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
			pPacket->GetGuildReturn()->m_ReturnType );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GUILD_RETURN_TYPE ReturnType = (GUILD_RETURN_TYPE)pPacket->GetGuildReturn()->m_ReturnType;

		switch( ReturnType )
		{
		case GUILD_RETURN_RESPONSE:
			{
				if( pPacket->GetGuildReturn()->m_GUID != pHuman->GetGUID() )
				{
					break;
				}
			}
			/* go through */
		case GUILD_RETURN_CREATE:
			{
                pHuman->SetGuildID( pPacket->GetGuildReturn()->m_GuildID );
                CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
                sprintf(zsMsgBangPaiTitle, "%s帮主",pPacket->GetGuildReturn()->m_GuildName);
                pHuman->SetCurGuildTitle(GUANZHI_TITLE_ID);
                pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
                pHuman->UpdateTitlesToClient();
			}
			break;
		case GUILD_RETURN_JOIN:
			{ // 给玩家设上 GuildID
				pHuman->SetGuildID( INVALID_ID );
			}
			break;
        case GUILD_RETURN_INVIT://邀请加入帮会的返回结果
            {
                pHuman->SetGuildID(pPacket->GetGuildReturn()->m_GuildID);

                //给与称号
                if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
                {
                    CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
                    sprintf(zsMsgBangPaiTitle, "%s帮众",pPacket->GetGuildReturn()->m_GuildName);
                    pHuman->SetCurGuildTitle(GUANZHI_TITLE_ID);
                    pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
                    pHuman->UpdateTitlesToClient();
                    pHuman->SetGuildID( pPacket->GetGuildReturn()->m_GuildID );
                }
            }
            break;
		case GUILD_RETURN_EXPEL:
		case GUILD_RETURN_LEAVE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
                    CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					pHuman->SetGuildID( INVALID_ID );
					pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));

                    HumanDB* pHumanDB = pHuman->GetDB();
                    if (pHumanDB)
                    {
                        pHumanDB->SetLeaveGuildTime(g_pTimeManager->GetANSITime());
                        pHumanDB->RemoveTitle(BANGPAI_TITLE_ID);
                        pHumanDB->RemoveTitle(GUANZHI_TITLE_ID);
                    }
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{//给与称号
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s帮众",pPacket->GetGuildReturn()->m_GuildName);
                    pHuman->SetCurGuildTitle(BANGPAI_TITLE_ID);
					pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
					pHuman->SetGuildID( pPacket->GetGuildReturn()->m_GuildID );
				}
			}
			break;
		case GUILD_RETURN_PROMOTE:
		case GUILD_RETURN_DEMOTE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_PositionName);
					pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				if( pPacket->GetGuildReturn()->m_GUID == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_PositionName);
					pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
				else if(pPacket->GetGuildReturn()->m_GUIDChanged == pHuman->GetGUID() )
				{
					CHAR zsMsgBangPaiTitle[MAX_NICK_NAME] = {0};
					sprintf(zsMsgBangPaiTitle, "%s%s", pPacket->GetGuildReturn()->m_GuildName, pPacket->GetGuildReturn()->m_ChangedPositionName);
					pHuman->SetGuildTitleName(zsMsgBangPaiTitle, (BYTE)strlen(zsMsgBangPaiTitle));
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		case GUILD_RETURN_FOUND:
		case GUILD_RETURN_REJECT:
        case GUILD_RETURN_ALREADY_IN:
			break;
        case GUILD_RETURN_LEVEL_UP:
            {
                //失败的话   得返还扣除的金钱
                if (!pPacket->GetGuildReturn()->m_bScuess)
                {
                    INT nLevel = pPacket->GetGuildReturn()->m_GuildLevel;
                    ++nLevel;
                    Assert(nLevel < MAX_GUILD_LEVEL);

                    INT nMoney  = pHuman->GetMoney() + g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_Gold;
                    pHuman->SetMoney(nMoney);
                }
                else//把帮会人数上限，黄金家族，家族数等信息同步到GL
                {
                    INT nLevel = pPacket->GetGuildReturn()->m_GuildLevel;

                    GUILD_CGW_LEVELINFO levelInfo;
                    levelInfo.m_MaxUser             =   g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_MaxGuildUser;
                    levelInfo.m_MaxFamily           =   g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_MaxFamily;
                    levelInfo.m_MaxGoldFamilyUser   =   g_GuildLevelInfoTbl.m_TableInfo[nLevel].m_MaxGoldFamilyUser;

                    GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
                    pMsg->SetGUID(pHuman->GetGUID());
                    pMsg->SetGuildPacket((_GUILD_CGW_PACKET*)&levelInfo);
                    pMsg->GetGuildPacket()->m_uPacketType   =   GUILD_PACKET_GW_LEVEL_INFO;
                    g_pServerManager->SendPacket( pMsg, INVALID_ID );
                }
            }
            break;
		case GUILD_RETURN_DISMISS:
			{
                // 这里以后还得处理离线玩家称号的情况，现在无解
				pHuman->SetGuildID( INVALID_ID );
				pHuman->SetGuildTitleName("", 0);

                HumanDB* pHumanDB = pHuman->GetDB();
                if (pHumanDB)
                {
                    pHumanDB->RemoveTitle(BANGPAI_TITLE_ID);
                    pHumanDB->RemoveTitle(GUANZHI_TITLE_ID);
                }
				pHuman->UpdateTitlesToClient();
			}
			break;
        case GUILD_RETURN_CREATE_FAILED:
            {
                pHuman->SetMoney(pHuman->GetMoney()+GUILD_CREATE_MONEY_LIMIT);

                MONEY_LOG_PARAM	MoneyLogParam;
                MoneyLogParam.CharGUID	=	pHuman->GetGUID();
                MoneyLogParam.OPType	=	MONEY_GUILD_CREATE_COST;	
                MoneyLogParam.Count		=	GUILD_CREATE_MONEY_LIMIT;
                MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
                MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
                MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
                SaveMoneyLog(&MoneyLogParam);

                ITEM_LOG_PARAM	ItemLogParam;
                ItemLogParam.OpType	= ITEM_SYSTEM_GIVE_BACK;
                BOOL bScuess = HumanItemLogic::CreateMultiItemToBag(&ItemLogParam, pHuman, GUILD_CREATE_ITEM_INDEX, 1);

                if(!bScuess)
                {
                    g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler: GamePlayer (ReturnType=%d),Create Item failed![GUID=%d] ",
                        pPacket->GetGuildReturn()->m_ReturnType, pHuman->GetGUID());
                }
            }
            break;

        case GUILD_RETURN_ADD_FAMILY_USER:
            {
                
            }
            break;

        case GUILD_RETURN_BEADD_FAMILY:
            {
                pHuman->SetFamilyID(pPacket->GetGuildReturn()->m_cFamilyID);
            }
            break;

        case GUILD_RETURN_REMOVE_FAMILY_USER:
            {
            }
            break;

        case GUILD_RETURN_CREATE_FAMILY:
            {
                pHuman->SetFamilyID(pPacket->GetGuildReturn()->m_cFamilyID);
            }
            break;
        case GUILD_RETURN_MODIFY_DESC:
            {
            }
            break;
        case GUILD_RETURN_FAMILY_DISMISS:
            {
            }
            break;
		default:
			Assert(FALSE);
			return 0;
		}

		GCGuildReturn Msg;

		Msg.SetGuildReturn( pPacket->GetGuildReturn() );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGGuildReturnHandler: GamePlayer (ReturnType=%d) ",
			pPacket->GetGuildReturn()->m_ReturnType );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
