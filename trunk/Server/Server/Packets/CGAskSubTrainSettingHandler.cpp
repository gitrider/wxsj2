
#include "stdafx.h"
#include "CGAskSubTrainSetting.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCRetSubTrainSetting.h"
#include "ScriptDef.h"

uint	CGAskSubTrainSettingHandler::Execute(CGAskSubTrainSetting* pPacket, Player* pPlayer )
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
	
	const _SUBTRAIN_SETTING& HuamnsubTrainSetting = pHuman->GetSettingList()->m_SubTrainSetting;
	_SUBTRAIN_SETTING subTrainSetting;
	
	memcpy(&subTrainSetting, &HuamnsubTrainSetting, sizeof(_SUBTRAIN_SETTING));

	if (subTrainSetting.m_bSubTrainOpen)
	{
		double fTime  = (double)g_pTimeManager->GetANSITime()/60.0 - subTrainSetting.m_SubTrainBeginTime;
		double dHours = fTime/60.0;
		if (fTime>30)
		{
			dHours = 0.5;
			subTrainSetting.m_bSubTrainOpen = FALSE;
		}
		double fExp   = dHours*(double)subTrainSetting.m_SubTrainExpRate;
		double fMoney = dHours*(double)subTrainSetting.m_SubTrainMoneyRate;

		subTrainSetting.m_SubTrainMoney		= (UINT)fMoney;	//金钱积累
		subTrainSetting.m_SubTrainExper		= (UINT)fExp;
		
		pHuman->SetSubTrainSetting(subTrainSetting);

		subTrainSetting.m_SubTrainBeginTime = (UINT)(dHours*60.0);
	}
	else
	{
		INT nLevel = pHuman->GetLevel();
		INT nAttack = pHuman->GetAttack();
		Assert(nLevel>0);
		Assert(nAttack>0);

		switch(subTrainSetting.m_SubTrainMode)
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

				subTrainSetting.m_SubTrainExpRate  = (UINT)expRate;
				subTrainSetting.m_SubTrainMoneyRate = (UINT)moneyRate;
			}
			break;
		default:
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSubTrainSettingHandler:  Mode=%d error", subTrainSetting.m_SubTrainMode ) ;
				return PACKET_EXE_ERROR;
			}
		}
	}

	GCRetSubTrainSetting Result;
	Result.SetSetting(&subTrainSetting);
	pGamePlayer->SendPacket(&Result);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSubTrainSettingHandler:  ok") ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}