
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
        
        if (pGuild->GetChieftainGUID() == guid && pGuild->GetGuildLevel() >= 0)//�ǰ���
        {
            if (pCountry)
            {
                // �й���
                if (pCountry->GetCountryInfo()->m_GuoWang != INVALID_GUID)
                {
                    INT nRet = pCountry->Delate(pUser->GetGUID());

                    switch (nRet)
                    {
                    case Country::RET_REPEAT://�����Ѿ�ʹ���˵���Ȩ
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "���Ѿ��������������������԰�����ȴ����������Ĳ�����" );
                            g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
                        }
                        break;
                    case Country::RET_SCUESS://�ɹ�Ͷ������Ʊ
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "���Ѿ���������������������Ŀǰ����״̬Ϊ%d/3��ֻ�е���3/3״̬�����ܴ�������ͶƱ��", pCountry->GetGuildMasterVote() );
                            g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

                            WGCountryReturn Msg;
                            Msg.SetPlayerID( pUser->GetPlayerID() );
                            _COUNTRY_RETURN CountryReturn;
                            CountryReturn.m_ReturnType        = COUNTRY_RETURN_DELATE;
                            Msg.SetCountryReturn( &CountryReturn );

                            pServerPlayer->SendPacket( &Msg );
                        }
                        break;
                    case Country::RET_OK://3Ʊ��   ��������ͶƱ
                        {
                            CHAR szContex[MAX_CHAT_SIZE] = {0};
                            sprintf(szContex, "�ҹ������Ѿ���ʼ���е���������ͶƱ�׶Σ����λ����������룡");

                            WGChat Msg ;
                            Msg.SetChatType( CHAT_TYPE_COUNTRY ) ;
                            Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
                            Msg.SetContex( szContex ) ;
                            Msg.SetSourNameSize( (BYTE)strlen("����") ) ;
                            Msg.SetSourName( "����" ) ;
                            Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

                            //����������Ϣ
                            Msg.SetCountryID( pUser->GetCountry() );
                            g_pServerManager->BroadCastServer( &Msg ) ;

                            //��ʼͶƱ��ʱ
                            pCountry->VoteOnBegin();
                        }
                        break;
                    case Country::RET_FULL://������ͶƱ��
                        {

                        }
                        break;
                    case Country::RET_FAIL://ͶƱʧ��
                        {
                            CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                            sprintf(szMailContent, "�Ѿ���ʼȫ��ͶƱ�����ڲ��ܷ���������" );
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
