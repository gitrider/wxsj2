
#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCMissionList.h"
#include "Scene.h"
#include "CGAskMissionList.h"
#include "GCMissionModify.h"

#include "DayTips.h"

using namespace Packets;


uint CGAskMissionListHandler::Execute( CGAskMissionList* pPacket, Player* pPlayer )
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


	GCMissionList	Msg;
	Msg.SetObjID( pHuman->GetID() );

	UINT i;
	const _OWN_MISSION *pMission;

	// 如果每日活动今日可接，且最后一次完成日期不是今天，则置次数为0，重新开始计数	
	INT nCount = DayTips::GetMe()->GetMisCount();
	for ( INT i = 0; i < nCount; i++ )// 循环活动表的每一个活动
	{
		INT nID = DayTips::GetMe()->GetMisIDByIndex( i );

		if ( DayTips::GetMe()->IsTodayMission( nID ) == TRUE )
		{
			INT nLastDoneIndex = DayTips::GetMe()->GetMisDataDateIndex( nID );

			INT nLastDoneDate = pHuman->GetMissionData( nLastDoneIndex );

			if ( DayTips::GetMe()->LastIsTodayRecord( nLastDoneDate ) == 0 )
			{
				pHuman->SetMissionData( DayTips::GetMe()->GetMisDataCountIndex( nID ), 0 );
			}	
		}	
	}


	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		pMission = pHuman->GetMission( i );


		if ( pMission->m_idMission != INVALID_ID )
		{
			Msg.AddMission( (BYTE)i, pMission );
		}
	}
	
	Msg.SetMissionList((_MISSION_LIST*)pHuman->GetConstMissionList());

	Msg.SetMissionHaveDone(pHuman->GetConstMissionList()->m_aMissionHaveDoneFlags);

	pGamePlayer->SendPacket( &Msg ) ;

	GCMissionModify msgMissionModify;
	msgMissionModify.SetFlag(1) ;
	msgMissionModify.SetMission(pHuman->GetConstMissionList()->m_aMission);
	msgMissionModify.SetMissionData( pHuman->GetConstMissionList()->m_aMissionData );
	pGamePlayer->SendPacket( &msgMissionModify );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionListHandler: ok GUID=%X scene=%d", 
		pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
