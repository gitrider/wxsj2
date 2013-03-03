/*
验证申请的合法性，并转发
*/

#include "stdafx.h"
#include "CGExchangeApplyI.h"
#include "GCExchangeApplyI.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeApplyIHandler::Execute( CGExchangeApplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t		TargetID = pPacket->GetObjID();
	Obj_Human* pSourceHuman = pHuman;//交易发起者
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( TargetID );//交易对象

    if (pSourceHuman->IsInLoginProtectTime() || pDestHuman->IsInLoginProtectTime())
    {//在上线保护时间内，不可操作
        GCExchangeError Msg;
        Msg.SetID(EXCHANGE_MSG::ERR_IN_PROTECT_TIME);
        pGamePlayer->SendPacket(&Msg);
        g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d or ID = %d ERR_IN_PROTECT_TIME", pSourceHuman->GetID(), pDestHuman->GetID()) ;

        return PACKET_EXE_CONTINUE;
    }

	//验证
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	INT iSettingData = pDestHuman->GetDB()->GetSetting(SETTING_TYPE_GAME)->m_SettingData;
	if(SETTINGFLAGISTRUE(iSettingData, GSF_REFUSE_TRADE))
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_REFUSE_TRADE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERR_REFUSE_TRADE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//发起者正在交易中
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pSourceHuman->GetID(), pSourceHuman->m_ExchangBox.m_Status) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//目标正在交易中
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pDestHuman->GetID(),	pDestHuman->m_ExchangBox.m_Status ) ;
		return PACKET_EXE_CONTINUE;
	}
	//在使用战斗技能时不能交易
	//在使用生活技能时不能交易 
	//战斗中不能交易add by gh 2010/04/28
	if(pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pSourceHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::使用技能中") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || pDestHuman->GetCharacterLogic() == CHARACTER_LOGIC_USE_ABILITY)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_USE_SKILL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::目标使用技能中") ;
		return PACKET_EXE_CONTINUE ;
	}
	//行走中不能交易
	if(pSourceHuman->IsMoving())
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_MOVE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::移动中") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->IsMoving())
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_MOVE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::目标移动中") ;
		return PACKET_EXE_CONTINUE ;
	}
    //在摆摊时不能交易
	if(pSourceHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::摆摊中") ;
		return PACKET_EXE_CONTINUE ;
	}
	if(pDestHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_OPEN_STALL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler::目标摆摊中") ;
		return PACKET_EXE_CONTINUE ;
	}
	// 死亡或者掉线不能交易
	if(pSourceHuman->GetHP() <= 0 || pSourceHuman->GetPlayer()->IsDisconnect())
	{//发起者死亡或者掉线
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERROR:DEAD OR OFFLINE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->GetHP() <= 0 || pDestHuman->GetPlayer()->IsDisconnect()) // 验证过掉线
	{//目标死亡或者掉线
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_DEAD_OR_OFFLINE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERROR:TARGET DEAD OR OFFLINE ", pDestHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	// end of add
    //交易的玩家之间的距离超过xx米不能交易
    FLOAT fDistance = MySqrt( pDestHuman->getWorldPos(), pSourceHuman->getWorldPos() );
    if (fDistance > EXCHANGE_MAX_DISTANCE)
    {
        GCExchangeError Msg;
        Msg.SetID(EXCHANGE_MSG::ERR_TOO_FAR);
        pHuman->GetPlayer()->SendPacket(&Msg);
        g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 超过交易范围[%s]]",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
        return PACKET_EXE_CONTINUE;
    }

	//操作
	//发送消息向目标申请
	GCExchangeApplyI Msg;
	Msg.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: SOURCEID = %d DESTID = %d", pSourceHuman->GetID(), pDestHuman->GetID() ) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
