
//GWCountryVoteOnHandler.cpp
#include "stdafx.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"
#include "CountryManager.h"

#include "GWCountryVoteOn.h"
#include "MailCenter.h"


UINT GWCountryVoteOnHandler::Execute( GWCountryVoteOn* pPacket, Player* pPlayer )
{
    __ENTER_FUNCTION

    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    GUID_t guid = pPacket->GetGuid();

    USER* pUser = g_pOnlineUser->FindUser( guid );
    if( pUser == NULL )
    {
        Log::SaveLog( WORLD_LOGFILE, "GWCountryVoteOnHandler...User GUID=%X not find!", 
            guid );
        return PACKET_EXE_CONTINUE;
    }

    INT nCountry = pUser->GetCountry();
    Country *pCountry = g_pCountryManager->GetCountry(nCountry);

    if (pCountry)
    {
        if ( pUser->GetLevel() >= 50)
        {
            if (pCountry->VoteOn(pPacket->GetResult(), guid))
            {
                Log::SaveLog( WORLD_LOGFILE, "GWCountryVoteOnHandler...User GUID=%X Voteon Scuess !", 
                    guid);

                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "���ѳɹ������˵���������ͶƱ����ȴ������" );
                g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

                // ÿ��ͶƱ����ͳ��
                // �����ʸ���66%�������ɹ�������ͶƱ
                g_pCountryManager->BroadcastVoteResult(pCountry, TRUE);
            }
            else
            {
                CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
                sprintf(szMailContent, "���Ѿ�Ͷ��һƱ���޷�����ͶƱ��" );
                g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);
            }
        }
    }

    return PACKET_EXE_CONTINUE ;

    __LEAVE_FUNCTION

    return PACKET_EXE_ERROR ;
}
