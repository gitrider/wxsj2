/********************************************************************************
 *	�ļ�����	WGUpdateTitleHandler.cpp
 *	ȫ·����	d:\Prj\Server\Server\Packets\WGUpdateTitleHandler.cpp
 *	�����ˣ�	Amos
 *	����ʱ�䣺	2008 �� 3 �� 28 ��	
 *
 *	����˵����	������ҳƺ�����
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGUpdateTitle.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCCharAllTitles.h"
#include "GWGuild.h"
#include "PacketFactoryManager.h"


uint WGUpdateTitleHandler::Execute( WGUpdateTitle* pPacket, Player* pPlayer )
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

        //���³ƺ�
        INT iType = pPacket->GetTitleType();

        switch (iType)
        {
        case _TITLE::GUOJIA_TITLE:
            {
                pHuman->SetCurCountryTitle(pPacket->GetTitleID());
                pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_GUOJIA_TITLE);

                if (pPacket->GetTitleSize() > 0)
                {
                    pHuman->SetCountryTitleName(pPacket->GetTitle(), pPacket->GetTitleSize());
                    pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_GUOJIA_TITLE);
                }
                break;
            }
        case _TITLE::BANGPAI_TITLE:
            {
                //����ֻ�����޸���Ұ��ƺ�����
                pHuman->SetCurGuildTitle(pPacket->GetTitleID());
                if (pPacket->GetTitleSize() > 0)
                {
                    pHuman->SetGuildTitleName(pPacket->GetTitle(), pPacket->GetTitleSize());
                    pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_BANGPAI_TITLE_NAME);
                    
                    //�����Զ���ƺ�
                    if (GUANZHI_TITLE_ID == pPacket->GetTitleID())
                    {
                        pHuman->GetDB()->SetOfficalTitleName(pPacket->GetTitle(), pPacket->GetTitleSize());
                    }
                }
                break;
            }
        case _TITLE::WANFA_TITLE:
            {
                pHuman->SetCurNormalTitle(pPacket->GetTitleID());
                pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_WANFA_TITLE);

                if (pPacket->GetTitleSize() > 0)
                {
                    pHuman->SetNormalTitleName(pPacket->GetTitle(), pPacket->GetTitleSize());
                    pHuman->UpdateTitlesToClient(GCCharAllTitles::UPDATE_CUR_WANFA_TITLE);
                }
                break;
            }
        }

		g_pLog->FastSaveLog( LOG_FILE_1, "WGUpdateTitleHandler Scuess! : GamePlayer (Guid=%d) ",
			pHuman->GetGUID());
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
