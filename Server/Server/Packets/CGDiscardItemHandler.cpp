#include "stdafx.h"
#include "CGDiscardItem.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "ItemRuler.h"
#include "GCDiscardItemResult.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"

uint CGDiscardItemHandler::Execute(CGDiscardItem* pPacket, Player* pPlayer )
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

	BYTE	Opt = pPacket->GetOpt();
	BYTE	BagIndex = pPacket->GetBagIndex();

	if (BagIndex>=EXTRA_CONTAINER_OFFSET)
	{
		return PACKET_EXE_ERROR;
	}

	if(Opt == CGDiscardItem::FromBag)
	{
		Item*	pBagItem		=	HumanItemLogic::GetBagItem(pHuman,BagIndex); 
		if(pBagItem == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardItemHandler:  BagIndex=%d  occur null type ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}

		if(pBagItem->IsEmpty())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}

		GCDiscardItemResult	Msg;

		Msg.setItemTableIndex(pBagItem->GetItemTableIndex());
		
		if(pBagItem->IsRuler(IRL_DISCARD))
		{
			ITEM_LOG_PARAM		ItemLogParam;
			ItemLogParam.OpType	=	ITEM_USEGEM_LOST;
			BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,BagIndex);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Msg.setResult(DISCARDITEM_SUCCESS);
			Msg.setBagIndex(BagIndex);
		}
		else
		{
			Msg.setResult(DISCARDITEM_FAIL);
		}
		pGamePlayer->SendPacket(&Msg);
	}
	else if(Opt == CGDiscardItem::FromBank)
	{

		Item*	pBagItem		=	HumanItemLogic::GetBankItem(pHuman,BagIndex); 
		Assert(pBagItem);

		if(pBagItem->IsEmpty())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardItemHandler: ok BagIndex=%d  occur null type ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}
		GCDiscardItemResult	Msg;
		Msg.setItemTableIndex(pBagItem->GetItemTableIndex());
		
		if(pBagItem->IsRuler(IRL_DISCARD))
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		=	ITEM_DISCARD_EQUIP;
			ItemLogParam.CharGUID	=	pHuman->GetGUID();
			ItemLogParam.ContainerPos		=	BagIndex;
			ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();
			ItemLogParam.ItemGuid   =   pBagItem->GetGUID();
			ItemLogParam.ItemType   =	pBagItem->GetItemTableIndex();

			ItemContainer* pBankContainer = pHuman->GetBankContain();
			if(g_ItemOperator.EraseItem(pBankContainer, BagIndex) == FALSE)
			{
				SaveItemLog(&ItemLogParam);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardItemHandler: ok BagIndex=%d  occur null type ",	BagIndex ) ;
				return PACKET_EXE_CONTINUE;
			}
			Msg.setOpt(GCDiscardItemResult::FromBank);
			Msg.setResult(DISCARDITEM_SUCCESS);
			Msg.setBagIndex(BagIndex);
		}
		else
		{
			Msg.setResult(DISCARDITEM_FAIL);
		}
		pGamePlayer->SendPacket(&Msg);
	}



	g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardItemHandler: ok BagIndex=%d", 
		BagIndex ) ;



	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}