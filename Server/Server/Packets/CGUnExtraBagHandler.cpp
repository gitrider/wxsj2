#include "stdafx.h"
#include "CGUnExtraBag.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCUnExtraBagResult.h"
#include "GCCharEquipment.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

uint CGUnExtraBagHandler::Execute( CGUnExtraBag* pPacket, Player* pPlayer )
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
	
	INT nBagPoint = pPacket->getExtraContainerPoint();
	if ((nBagPoint<EXTRA_CONTAINER_OFFSET)||(nBagPoint>=MAX_BAG_SIZE))
	{
		return PACKET_EXE_ERROR;
	}
	
	Item* pItem = HumanItemLogic::GetExtraContainerItem(pHuman, nBagPoint);
	Assert(pItem);
	if (pItem->IsEmpty())
	{
		return PACKET_EXE_ERROR;
	}
	UCHAR	itemClass = pItem->GetItemClass();
	if(itemClass ==	ICLASS_EXTRABAG)
	{
		INT		BagIndex = -1;
		ItemContainer* pExtraContainer = pHuman->GetExtraContain();
		ItemContainer* pExtraBagContainer =HumanItemLogic::GetBagByExtraContainPos(pHuman, nBagPoint);
		if (pExtraBagContainer == NULL)
		{
			return PACKET_EXE_ERROR;
		}
		if (!pExtraBagContainer->IsValid())
		{
			return PACKET_EXE_CONTINUE;
		}
		
		if (!pExtraBagContainer->IsEmpty())
		{
			return PACKET_EXE_CONTINUE;
		}

		ITEM_LOG_PARAM LogParam;

		BOOL Ret = HumanItemLogic::MoveItem(&LogParam, pHuman, pExtraContainer, pExtraContainer->BagIndex2ConIndex(nBagPoint), BagIndex);
		if (!Ret)
		{
			return PACKET_EXE_CONTINUE;
		}
		pHuman->ReInitExtraBag();
		GCUnExtraBagResult	 Msg;
		if(BagIndex>=0)
		{
			pExtraBagContainer->ChangeSize(0);
			Msg.setBagIndex(BagIndex);
			Msg.setDestIndex(nBagPoint);
			Msg.setResult(UNEQUIP_SUCCESS);
			pGamePlayer->SendPacket( &Msg ) ;
		}
		else
		{
			if(ITEMOE_DESTOPERATOR_HASITEM == BagIndex)
			{
				Msg.setResult(UNEXTRABAG_HAS_ITEM);
				pGamePlayer->SendPacket( &Msg ); 
			}
			else if(ITEMOE_DESTOPERATOR_FULL == BagIndex)
			{
				Msg.setResult(UNEXTRABAG_BAG_FULL);
				pGamePlayer->SendPacket( &Msg ); 
			}
		}

	}
	else
	{
		Assert(FALSE);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUnExtraBagHandler nBagPoint=%d itemClass=%d",
		nBagPoint, itemClass  ) ;


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}