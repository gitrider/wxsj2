//�ͻ�������������뽨������
#include "stdafx.h"
#include "CGGuildApply.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWGuild.h"
#include "HumanItemLogic.h"
#include "TimeManager.h"
#include "GCGuildReturn.h"
UINT CGGuildApplyHandler::Execute( CGGuildApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

    HumanDB* pHumanDB = pHuman->GetDB();
    Assert(pHumanDB);

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	BYTE	GuildNameSize		=	pPacket->GetGuildNameSize();	
	CHAR*	pszGuildName		=	pPacket->GetGuildName();		

	BYTE	GuildDescSize		=	pPacket->GetGuildDescSize();	
	CHAR*	pszGuildDesc		=	pPacket->GetGuildDesc();		

	// �ύ����������룬�ȿ۳���Ǯ���жϵȼ����Ƿ��а��
    // �Ƿ��д������ĵ��ߣ��Ƿ��˳����24Сʱ����  m_LeaveGuildTime�����ʼ��
	if (pHuman->GetLevel()   >= GUILD_CREATE_LEVEL_LIMIT &&
		pHuman->GetMoney()   >= GUILD_CREATE_MONEY_LIMIT &&
		pHuman->GetGuildID() == INVALID_GUILD_ID         &&
        INVALID_ID           != HumanItemLogic::GetBagItemPosByType(pHuman, GUILD_CREATE_ITEM_INDEX)) 
    //g_pTimeManager->DiffTime(pHumanDB->GetLeaveGuildTime(),
    //g_pTimeManager->GetANSITime()) >= GUILD_REJOIN_TIME_LIMIT  &&
	{
		pHuman->SetMoney(pHuman->GetMoney() - GUILD_CREATE_MONEY_LIMIT);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_GUILD_CREATE_COST;	
		MoneyLogParam.Count		=	GUILD_CREATE_MONEY_LIMIT;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

        ITEM_LOG_PARAM	ItemLogParam;
        ItemLogParam.OpType	= ITEM_USE;
        BOOL bScuess = HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, GUILD_CREATE_ITEM_INDEX, 1);

        if (bScuess)
        {
            SaveItemLog(&ItemLogParam);

            GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

            pMsg->SetGUID( pHuman->GetGUID() );
            _GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
            pGuildPacket->m_uPacketType = GUILD_PACKET_GW_CREATE;

            GUILD_CGW_CREATE* pPacketCreate;
            pPacketCreate = (GUILD_CGW_CREATE*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE));

            pPacketCreate->m_NameSize = GuildNameSize;
            strncpy(pPacketCreate->m_szGuildName, pszGuildName, GuildNameSize);

            pPacketCreate->m_DescSize = GuildDescSize;
            strncpy(pPacketCreate->m_szGuildDesc, pszGuildDesc, GuildDescSize);

            g_pServerManager->SendPacket( pMsg, INVALID_ID );


            g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildApplyHandler::Name=%s"
                ,pHuman->GetName()) ;
        }
        else
        {
            GCGuildReturn Msg;
            _GUILD_RETURN ret;
            ret.m_ReturnType = GUILD_RETURN_ERASE_ITEM_FAIL;
            Msg.SetGuildReturn( &ret );

            pGamePlayer->SendPacket( &Msg );

            g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildApplyHandler::Name=%s �۳�������ʧ��"
                ,pHuman->GetName()) ;
        }
        return PACKET_EXE_CONTINUE ;
	}
	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
