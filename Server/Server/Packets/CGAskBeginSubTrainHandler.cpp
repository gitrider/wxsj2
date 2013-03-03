
#include "stdafx.h"
#include "CGAskBeginSubTrain.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCRetSubTrainSetting.h"
#include "GCSubTrainResult.h"
#include "ScriptDef.h"

uint	CGAskBeginSubTrainHandler::Execute(CGAskBeginSubTrain* pPacket, Player* pPlayer )
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
	const _SUBTRAIN_SETTING& HuamnsubTrainSetting = pHuman->GetSettingList()->m_SubTrainSetting;
	if (HuamnsubTrainSetting.m_bSubTrainOpen)
	{
		 Result.SetResult(SUBTRAIN_MODEERROR_OPEN_BEGINFAIL);
		 pGamePlayer->SendPacket( &Result );
		 g_pLog->FastSaveLog( LOG_FILE_1, "CGAskBeginSubTrainHandler:  has open, HuamnsubTrainSetting.m_bSubTrainOpen = TRUE") ;
		 return PACKET_EXE_CONTINUE;
	}

	BYTE nMode = pPacket->getMode();
	_SUBTRAIN_SETTING subTrainSetting;

	//每小时获得经验值＝玩家等级×玩家等级×玩家攻击力×代练模式倍数×常数（10）
	//每小时花费游戏币＝玩家等级×玩家等级×代练模式倍数×常数（5）
	INT nLevel = pHuman->GetLevel();
	INT nAttack = pHuman->GetAttack();
	Assert(nLevel>0);
	Assert(nAttack>0);

	switch(nMode)
	{
	case SUBTRAIN_MODE1:
		{
			INT modeRate = 10;
			INT expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			INT moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);

			subTrainSetting.m_bSubTrainOpen = TRUE;
			subTrainSetting.m_SubTrainMode  = SUBTRAIN_MODE1;
			subTrainSetting.m_SubTrainBeginTime = (UINT)((double)g_pTimeManager->GetANSITime()/60.0);
			subTrainSetting.m_SubTrainMoney	= 0;
			subTrainSetting.m_SubTrainExper	= 0;
			subTrainSetting.m_SubTrainExpRate  = (UINT)expRate;
			subTrainSetting.m_SubTrainMoneyRate = (UINT)moneyRate;
		}
		break;
	case SUBTRAIN_MODE1POINT5:
		{
			INT modeRate = 15;
			INT expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			INT moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);

			subTrainSetting.m_bSubTrainOpen = TRUE;
			subTrainSetting.m_SubTrainMode  = SUBTRAIN_MODE1POINT5;
			subTrainSetting.m_SubTrainBeginTime = (UINT)((double)g_pTimeManager->GetANSITime()/60.0);
			subTrainSetting.m_SubTrainMoney	= 0;
			subTrainSetting.m_SubTrainExper	= 0;
			subTrainSetting.m_SubTrainExpRate  = (UINT)expRate;
			subTrainSetting.m_SubTrainMoneyRate = (UINT)moneyRate;
		}
		break;
	case SUBTRAIN_MODE2:
		{
			INT modeRate = 20;
			INT expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			INT moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);

			subTrainSetting.m_bSubTrainOpen = TRUE;
			subTrainSetting.m_SubTrainMode  = SUBTRAIN_MODE2;
			subTrainSetting.m_SubTrainBeginTime = (UINT)((double)g_pTimeManager->GetANSITime()/60.0);
			subTrainSetting.m_SubTrainMoney	= 0;
			subTrainSetting.m_SubTrainExper	= 0;
			subTrainSetting.m_SubTrainExpRate  = (UINT)expRate;
			subTrainSetting.m_SubTrainMoneyRate = (UINT)moneyRate;
		}
	    break;
	case SUBTRAIN_MODE3:
		{
			INT modeRate = 30;
			INT expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			INT moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);

			subTrainSetting.m_bSubTrainOpen = TRUE;
			subTrainSetting.m_SubTrainMode  = SUBTRAIN_MODE2;
			subTrainSetting.m_SubTrainBeginTime = (UINT)((double)g_pTimeManager->GetANSITime()/60.0);
			subTrainSetting.m_SubTrainMoney	= 0;
			subTrainSetting.m_SubTrainExper	= 0;
			subTrainSetting.m_SubTrainExpRate  = (UINT)expRate;
			subTrainSetting.m_SubTrainMoneyRate = (UINT)moneyRate;
		}
	    break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskBeginSubTrainHandler:  Mode=%d error", nMode ) ;
			return PACKET_EXE_ERROR;
		}
	    break;
	}

	pHuman->SetSubTrainSetting(subTrainSetting);

	subTrainSetting.m_SubTrainBeginTime = 0;
	GCRetSubTrainSetting RetSubTrain;
	RetSubTrain.SetSetting(&subTrainSetting);
	pGamePlayer->SendPacket( &RetSubTrain );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskBeginSubTrainHandler:  ok") ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}