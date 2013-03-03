#include "stdafx.h"
#include "CGUseExtraBag.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCUseExtraBagResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

uint CGUseExtraBagHandler::Execute(CGUseExtraBag* pPacket, Player* pPlayer )
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

	//切场景时丢弃换装消息
	if(pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL ||
		!pHuman->IsActiveObj() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseExtraBagHandler: change scene") ;
		return PACKET_EXE_CONTINUE;
	}

	INT  BagIndex = pPacket->getBagIndex();

	if( BagIndex == INVALID_BAG_INDEX )
	{
		return PACKET_EXE_ERROR ;
	}
	
	if ((BagIndex>EXTRA_CONTAINER_OFFSET)||(BagIndex<0))
	{
		return PACKET_EXE_ERROR ;
	}
	
	GCUseExtraBagResult Msg;
	Item* pUseItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);

	//////////////////////////////////////////////////////////////////////////
	//装配条件判断
	UCHAR	itemClass = pUseItem->GetItemClass();

	if(itemClass == ICLASS_EXTRABAG)
	{
		ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, BagIndex);
		if(pBagContainer == NULL)
		{
			return PACKET_EXE_ERROR;
		}
		ItemContainer* pExtraContainer = pHuman->GetExtraContain();

		INT nConIndex = g_ItemOperator.MoveItem(pBagContainer, BagIndex, pExtraContainer, -1);
		if (nConIndex < 0)
		{
			return PACKET_EXE_CONTINUE ;
		}
		UINT nBagIndex = pExtraContainer->ConIndex2BagIndex((UINT)nConIndex);
		Item* pDestItem = HumanItemLogic::GetExtraContainerItem(pHuman,nBagIndex);
		Assert(pDestItem);
		Assert(!pDestItem->IsEmpty());
		if(pDestItem->IsCanEquipBind())
		{
			g_ItemOperator.SetItemBind(pExtraContainer, nConIndex);	
		}
		if (pDestItem->GetExtraBagBeginTime() == INVALID_TIME)
		{
			pDestItem->SetExtraBagBeginTime();
		}
		pHuman->ReInitExtraBag();

		Msg.setSourIndex(BagIndex);
		Msg.setDestBagIndex(nBagIndex);
		Msg.setResult(USEEXTRABAG_SUCCESS);

		pGamePlayer->SendPacket( &Msg ) ;
		
	}
	else
	{
		Msg.setResult(USEEXTRABAG_TYPE_FAIL);
		Msg.setSourIndex(BagIndex);
		Msg.setDestBagIndex(INVALID_INDEX);
		pGamePlayer->SendPacket( &Msg ) ;

	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseExtraBagHandler: BagIndex=%d", BagIndex ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}