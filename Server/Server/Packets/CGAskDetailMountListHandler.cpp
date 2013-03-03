// [7/8/2010 陈军龙]
#include "stdafx.h"
#include "CGAskDetailMountList.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCDetailMountList.h"
#include "HumanItemLogic.h"

uint	CGAskDetailMountListHandler::Execute(CGAskDetailMountList* pPacket, Player* pPlayer )
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

	ObjID_t objID = pPacket->getObjID();
	//请求者一定是人
	Obj* pOther = pScene->GetObjManager()->GetObj(objID);
	if (pOther == NULL)
	{
		return PACKET_EXE_ERROR;
	}
	else
	{
		if( pOther->GetObjType() != Obj::OBJ_TYPE_HUMAN )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailMountListHandler: TargetObjType is not Obj::OBJ_TYPE_HUMAN, TargetObjType=%d", 
				pOther->GetObjType()) ;
			return PACKET_EXE_ERROR;
		}
	}
	
	Obj_Human *pTarget = (Obj_Human*)pOther;

	if ( !(fabsf( pHuman->getWorldPos()->m_fX - pTarget->getWorldPos()->m_fX ) <= 10.f
		&& fabsf( pHuman->getWorldPos()->m_fZ - pTarget->getWorldPos()->m_fZ ) <= 10.f) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailMountListHandler: TargetObj is too Far") ;
		return PACKET_EXE_CONTINUE;
	}
		
	GCDetailMountList	Msg;
	Msg.setObjID( pTarget->GetID() );

	UINT indexFlag = 0;
	for(INT i=0; i<MAX_MOUNT_SIZE; i++)
	{

		Item* pMountItem = HumanItemLogic::GetMount(pTarget,i);
		//Assert(pMountItem);

		if((NULL == pMountItem) || (pMountItem->IsEmpty()))
			continue;

		pMountItem->SaveValueTo(Msg.getMountData(i));
		indexFlag|=(1<<i);

	}

	Msg.setMountIndexFlag(indexFlag);
	pGamePlayer->SendPacket( &Msg ) ;
	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailMountListHandler: ok ") ;

	return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}