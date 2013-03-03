
//GWCountryDelateHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"
#include "Guild.h"
#include "GuildManager.h"

#include "GWCountryDelate.h"
#include "MailCenter.h"
#include "WGCountryReturn.h"
#include "WGChat.h"
#include "ChatCenter.h"


UINT GWCountryDelateHandler::Execute( GWCountryDelate* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryDelateHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }


    INT nCountry = pUser->GetCountry();
    Country *pCountry = g_pCountryManager->GetCountry(nCountry);

    if (pCountry)
    {
        GuildID_t guildID = pUser->GetGuildID();
        Guild *pGuild = g_pGuildManager->GetGuild(guildID);
        
        if (pGuild->GetChieftainGUID() == guid && pGuild->GetGuildLevel() >= 0)//是帮主
        {
            if (pCountry)
            {
                // 有国王
                if (pCountry->GetCountryInfo()->m_GuoWang != INVALID_GUID)
                {
                    INT nRet = pCountry->Delate(pUser->GetGUID());

                    switch (nRet)
                    {
                    case Country::RET_REPEAT://本周已经使用了弹劾权
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "您已经进行了联名弹劾，请稍安勿燥，等待其它帮主的操作。" );
                            g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                        }
                        break;
                    case Country::RET_SCUESS://成功投出弹劾票
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "您已经参与了联名弹劾国王！目前弹劾状态为%d/3。只有到达3/3状态，才能触发弹劾投票！", pCountry->GetGuildMasterVote() );
                            g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

                            WGCountryReturn Msg;
                            Msg.SetPlayerID( pUser->GetPlayerID() );
                            _COUNTRY_RETURN CountryReturn;
                            CountryReturn.m_ReturnType        = COUNTRY_RETURN_DELATE;
                            Msg.SetCountryReturn( &CountryReturn );

                            pServerPlayer->SendPacket( &Msg );
                        }
                        break;
                    case Country::RET_OK://3票了   发起联名投票
                        {
                            CHAR szContex[MAX_CHAT_SIZE] = {0};
                            sprintf(szContex, "我国现在已经开始进行弹劾国王的投票阶段，请各位国民积极参与！");

                            WGChat Msg ;
                            Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
                            Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
                            Msg.SetContex( szContex ) ;
                            Msg.SetSourNameSize( (BYTE)strlen("公告") ) ;
                            Msg.SetSourName( "公告" ) ;
                            Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

                            //发送聊天消息
                            Msg.SetCountryID( pUser->GetCountry() );
                            g_pServerManager->BroadCastServer( &Msg ) ;

                            //开始投票计时
                            pCountry->VoteOnBegin();
                        }
                        break;
                    case Country::RET_FULL://不能再投票了
                        {

                        }
                        break;
                    case Country::RET_FAIL://投票失败
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "已经开始全民投票，现在不能发动弹劾！" );
                            g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                        }
                        break;
                    }
                }
            }
        }
    }


    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
