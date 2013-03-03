
#include "stdafx.h"
#include "CGAskGetSubTrainExp.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCSubTrainResult.h"
#include "GCRetSubTrainSetting.h"

uint	CGAskGetSubTrainExpHandler::Execute(CGAskGetSubTrainExp* pPacket, Player* pPlayer )
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

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	GCSubTrainResult Result;
	const _SUBTRAIN_SETTING& subTrainSetting = pHuman->GetSettingList()->m_SubTrainSetting;
	if (subTrainSetting.m_bSubTrainOpen)
	{
		Result.SetResult(SUBTRAIN_MODEERROR_OPEN_GETEXPFAIL);
		pGamePlayer->SendPacket( &Result );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskGetSubTrainExpHandler error open mode" ) ;
		return PACKET_EXE_CONTINUE;
	}
	
	if ((subTrainSetting.m_SubTrainExper == 0)&&(subTrainSetting.m_SubTrainMoney == 0))
	{
		return PACKET_EXE_CONTINUE;
	}
	if (pHuman->GetMoney() < subTrainSetting.m_SubTrainMoney)
	{
		Result.SetResult(SUBTRAIN_MODEERROR_MONEY_GETEXPFAIL);
		pGamePlayer->SendPacket( &Result );
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskGetSubTrainExpHandler money is not enough" ) ;
		return PACKET_EXE_CONTINUE;
	}
	
	_SUBTRAIN_SETTING TsubTrainSetting;

	TsubTrainSetting.m_bSubTrainOpen = FALSE;
	TsubTrainSetting.m_SubTrainMode	 = subTrainSetting.m_SubTrainMode;
	TsubTrainSetting.m_SubTrainBeginTime	= 0;				//������ʼʱ��
	TsubTrainSetting.m_SubTrainMoney		= 0;	//��Ǯ����
	TsubTrainSetting.m_SubTrainExper        = 0;	//�������
	TsubTrainSetting.m_SubTrainExpRate		= subTrainSetting.m_SubTrainExpRate;				//��������Ч��
	TsubTrainSetting.m_SubTrainMoneyRate	= subTrainSetting.m_SubTrainMoneyRate;				//������ǮЧ��
	
	pHuman->SetAlterExp(subTrainSetting.m_SubTrainExper);
	pHuman->SetMoney(pHuman->GetMoney() - subTrainSetting.m_SubTrainMoney);
	pHuman->SetSubTrainSetting(TsubTrainSetting);

	GCRetSubTrainSetting RetSubTrain;
	RetSubTrain.SetSetting(&TsubTrainSetting);
	pGamePlayer->SendPacket( &RetSubTrain );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskGetSubTrainExpHandler ok" ) ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}