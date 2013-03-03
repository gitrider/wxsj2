#include "stdafx.h"
#include "CGAskDetailEquipList.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCDetailEquipList.h"
#include "HumanItemLogic.h"

uint	CGAskDetailEquipListHandler::Execute(CGAskDetailEquipList* pPacket, Player* pPlayer )
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
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: TargetObjType is not Obj::OBJ_TYPE_HUMAN, TargetObjType=%d", 
				pOther->GetObjType()) ;
			return PACKET_EXE_ERROR;
		}
	}
	
	Obj_Human *pTarget = (Obj_Human*)pOther;

	if ( !(fabsf( pHuman->getWorldPos()->m_fX - pTarget->getWorldPos()->m_fX ) <= 10.f
		&& fabsf( pHuman->getWorldPos()->m_fZ - pTarget->getWorldPos()->m_fZ ) <= 10.f) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: TargetObj is too Far") ;
		return PACKET_EXE_CONTINUE;
	}
	
	INT mode = pPacket->GetAskMode();
	if ((mode != ASK_EQUIP_ALL)&&(mode != ASK_EQUIP_SET))
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: mode error mode=%d", pPacket->GetAskMode()) ;
		return PACKET_EXE_ERROR ;
	}
	
	GCDetailEquipList	Msg;
	Msg.setObjID( pTarget->GetID() );
	Msg.SetAskMode((ASK_EQUIP_MODE)mode);

	UINT RetPart = 0;
	if(mode ==ASK_EQUIP_ALL)
	{
		for(INT i=0; i<HEQUIP_NUMBER; i++)
		{

			Item* pEquipItem = HumanItemLogic::GetEquip(pTarget,(HUMAN_EQUIP)i);
			Assert(pEquipItem);

			if(pEquipItem->IsEmpty())
				continue;
			
			pEquipItem->SaveValueTo(Msg.GetEquipData((HUMAN_EQUIP)i));
			RetPart|=(1<<i);
		};
	}
	else
	{
		UINT	askPart =	pPacket->GetAskPart();
		if (askPart>=HEQUIP_NUMBER)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: askPart error askPart=%d", askPart) ;
			return PACKET_EXE_ERROR ;
		}

		for(INT i=0; i<HEQUIP_NUMBER; i++)
		{
			if( askPart & (1<<i) ) 
			{
				Item* pEquipItem = HumanItemLogic::GetEquip(pTarget,(HUMAN_EQUIP)i);
				Assert(pEquipItem);

				if(pEquipItem->IsEmpty())
					continue;
				pEquipItem->SaveValueTo(Msg.GetEquipData((HUMAN_EQUIP)i));
				RetPart|=(1<<((UINT)i));

			}
		}	
	}

	Msg.SetAskPart(RetPart);
	pGamePlayer->SendPacket( &Msg ) ;
	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: ok ") ;

	return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}