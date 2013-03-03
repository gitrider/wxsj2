
#include "stdafx.h"
#include "CGAskEndSubTrain.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCSubTrainResult.h"
#include "GCRetSubTrainSetting.h"

uint	CGAskEndSubTrainHandler::Execute(CGAskEndSubTrain* pPacket, Player* pPlayer )
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

	GCSubTrainResult Result;
	const _SUBTRAIN_SETTING& subTrainSetting = pHuman->GetSettingList()->m_SubTrainSetting;
	if (!subTrainSetting.m_bSubTrainOpen)
	{
		Result.SetResult(SUBTRAIN_MODEERROR_CLOSE_STOPFAI);
		pGamePlayer->SendPacket( &Result );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskEndSubTrainHandler  has closed" ) ;
		return PACKET_EXE_CONTINUE;
	}

	double fTime  = ((double)g_pTimeManager->GetANSITime()/60.0 - subTrainSetting.m_SubTrainBeginTime);
	double dHours = fTime/60.0;
	if (fTime>30)
	{
		dHours = 0.5;
	}

	double fExp   = dHours*(double)subTrainSetting.m_SubTrainExpRate;
	double fMoney = dHours*(double)subTrainSetting.m_SubTrainMoneyRate;
	
	if (fTime < 15)
	{
		Result.SetResult(SUBTRAIN_MODEERROR_CLOSE_TIMEFAIL);
		pGamePlayer->SendPacket( &Result );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskEndSubTrainHandler  time < 15min" ) ;
		return PACKET_EXE_CONTINUE;
	}
	

	_SUBTRAIN_SETTING TsubTrainSetting;

	TsubTrainSetting.m_bSubTrainOpen = FALSE;
	TsubTrainSetting.m_SubTrainMode	 = subTrainSetting.m_SubTrainMode;
	TsubTrainSetting.m_SubTrainBeginTime	= 0;				//带练开始时间
	TsubTrainSetting.m_SubTrainMoney		= (UINT)fMoney;	//金钱积累
	TsubTrainSetting.m_SubTrainExper		= (UINT)fExp;
	TsubTrainSetting.m_SubTrainExpRate		=  subTrainSetting.m_SubTrainExpRate;				//带练经验效率
	TsubTrainSetting.m_SubTrainMoneyRate	=  subTrainSetting.m_SubTrainMoneyRate;				//带练金钱效率

	pHuman->SetSubTrainSetting(TsubTrainSetting);

	GCRetSubTrainSetting RetSubTrain;
	RetSubTrain.SetSetting(&TsubTrainSetting);
	pGamePlayer->SendPacket( &RetSubTrain );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskEndSubTrainHandler  ok" ) ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}