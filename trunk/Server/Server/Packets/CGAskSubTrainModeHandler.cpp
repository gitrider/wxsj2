
#include "stdafx.h"
#include "CGAskSubTrainMode.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCSubTrainModeResult.h"
#include "GCSubTrainResult.h"
#include "ScriptDef.h"

uint	CGAskSubTrainModeHandler::Execute(CGAskSubTrainMode* pPacket, Player* pPlayer )
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
	if (subTrainSetting.m_bSubTrainOpen)
	{
		Result.SetResult(SUBTRAIN_MODEERROR_OPEN_ASKMODEFAIL);
		pGamePlayer->SendPacket( &Result );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSubTrainModeHandler error open mode" ) ;
		return PACKET_EXE_CONTINUE;
	}
	INT nLevel  = pHuman->GetLevel();
	INT nAttack = pHuman->GetAttack();
	
	INT moneyRate = 0;
	INT expRate   = 0;
	switch(pPacket->getMode())
	{
	case SUBTRAIN_MODE1:
		{
			INT modeRate = 10;
			expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);
		}
		break;
	case SUBTRAIN_MODE1POINT5:
		{
			INT modeRate = 15;
			expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);
		}
		break;
	case SUBTRAIN_MODE2:
		{
			INT modeRate = 20;
			expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);
		}
		break;
	case SUBTRAIN_MODE3:
		{
			INT modeRate = 30;
			expRate   = pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_EXPRATE_SCRIPT, DEF_SUBTRAIN_EXPRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainExp);

			moneyRate =  pScene->GetLuaInterface()->ExeScript_DDDDD(	SUBTRAIN_MONEYRATE_SCRIPT, DEF_SUBTRAIN_MONEYRATE,
				(INT)pHuman->getScene()->SceneID(),
				(INT)nLevel,
				(INT)nAttack,
				modeRate,
				g_Config.m_ConfigInfo.m_nSubTrainMoney);
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSubTrainModeHandler:  Mode=%d error", pPacket->getMode() ) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}


	GCSubTrainModeResult modeResult;
	modeResult.SetMoneyRate((UINT)moneyRate);
	modeResult.SetExpRate((UINT)expRate);
	pGamePlayer->SendPacket( &modeResult );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSubTrainModeHandler:  ok" ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}