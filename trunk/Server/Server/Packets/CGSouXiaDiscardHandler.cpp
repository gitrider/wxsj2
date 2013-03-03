#include "stdafx.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "ItemRuler.h"
#include "CGSouXiaDiscard.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCSouXiaDiscard.h"

uint CGSouXiaDiscardHandler::Execute(CGSouXiaDiscard* pPacket, Player* pPlayer )
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

	GCSouXiaDiscard	Msg;

	if(Opt == CGSouXiaDiscard::FromBag)
	{
		Item*	pBagItem		=	HumanItemLogic::GetBagItem(pHuman,BagIndex); 
		if(pBagItem == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  occur null type ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}

		if(pBagItem->IsEmpty())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}
		Msg.setItemTableIndex(pBagItem->GetItemTableIndex());

		// add by gh for souxia 2010/05/20
		INT	iParamSouXiaID = 0 , iParamSouXiaPos = -1;
		if ( (ICLASS_EQUIP == pBagItem->GetItemClass() && EQUIP_SOUXIA == pBagItem->GetItemType())
			|| (ICLASS_COMITEM == pBagItem->GetItemClass() && COMITEM_SOUXIA_RELIC == pBagItem->GetItemType()) )
		{
			ItemParamValue ipv = (ItemParamValue)IPV_INT;
			iParamSouXiaID = pBagItem->GetItemParam(0,ipv);
			iParamSouXiaPos = pBagItem->GetItemParam(4,ipv);

			if (iParamSouXiaID > 0 && iParamSouXiaPos >= 0)
			{
				//判断指定NPC的距离
				ObjID_t		NpcObjID = pPacket->GetUniqueNpc();
				//距离判定
				Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
				if(pNpcObj == NULL)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;
					return PACKET_EXE_CONTINUE ; 
				}

				FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

				if(fDist>MAX_NPC_DISTANCE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;

					Msg.setBagIndex(BagIndex);
					Msg.setCurPos(iParamSouXiaPos);
					Msg.setResult(DISCARDITEM_FAIL);
					pGamePlayer->SendPacket(&Msg);
					return PACKET_EXE_CONTINUE ; 
				}
			}
		}
		// end of add
		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType	=	ITEM_USEGEM_LOST;
		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,BagIndex);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler: ok BagIndex=%d  occur null type ",	BagIndex ) ;
			
			Msg.setBagIndex(BagIndex);
			Msg.setCurPos(iParamSouXiaPos);
			Msg.setResult(DISCARDITEM_FAIL);
			pGamePlayer->SendPacket(&Msg);

			return PACKET_EXE_CONTINUE;
		}

		if (iParamSouXiaID>0 && iParamSouXiaPos>=0)
		{
			pHuman->EraseSouXia(iParamSouXiaPos);// 删除拆解的捜侠录,清空原来捜侠录的数据
		}
		Msg.setBagIndex(BagIndex);
		Msg.setCurPos(iParamSouXiaPos);
		Msg.setResult(DISCARDITEM_SUCCESS);
		
		pGamePlayer->SendPacket(&Msg);
	}
	else if(Opt == CGSouXiaDiscard::FromBank)
	{

		Item*	pBagItem		=	HumanItemLogic::GetBankItem(pHuman,BagIndex); 
		Assert(pBagItem);

		if(pBagItem->IsEmpty())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler: ok BagIndex=%d  occur null type ",	BagIndex ) ;
			return PACKET_EXE_CONTINUE;

		}

		Msg.setItemTableIndex(pBagItem->GetItemTableIndex());

		// add by gh for souxia 2010/05.20
		INT	iParamSouXiaID = 0 , iParamSouXiaPos =-1;
		if ( (ICLASS_EQUIP == pBagItem->GetItemClass() && EQUIP_SOUXIA == pBagItem->GetItemType())
			|| (ICLASS_COMITEM == pBagItem->GetItemClass() && COMITEM_SOUXIA_RELIC == pBagItem->GetItemType() ))
		{	
			ItemParamValue ipv = (ItemParamValue)IPV_INT;
			iParamSouXiaID = pBagItem->GetItemParam(0,ipv);
			iParamSouXiaPos = pBagItem->GetItemParam(4,ipv);

			if (iParamSouXiaID > 0 && iParamSouXiaPos >= 0)
			{
				//判断指定NPC的距离
				ObjID_t		NpcObjID = pPacket->GetUniqueNpc();
				//距离判定
				Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
				if(pNpcObj == NULL)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;
					return PACKET_EXE_CONTINUE ; 
				}

				FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

				if(fDist>MAX_NPC_DISTANCE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler:  BagIndex=%d  NO item ",	BagIndex ) ;
					
					Msg.setBagIndex(BagIndex);
					Msg.setCurPos(iParamSouXiaPos);
					Msg.setOpt(GCSouXiaDiscard::FromBank);
					Msg.setResult(DISCARDITEM_FAIL);
					return PACKET_EXE_CONTINUE ; 
				}
			}
		}
		// end of add
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
			g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler: ok BagIndex=%d  occur null type ",	BagIndex ) ;

			Msg.setBagIndex(BagIndex);
			Msg.setCurPos(iParamSouXiaPos);
			Msg.setOpt(GCSouXiaDiscard::FromBank);
			Msg.setResult(DISCARDITEM_FAIL);
			pGamePlayer->SendPacket(&Msg);

			return PACKET_EXE_CONTINUE;
		}
		
		if (iParamSouXiaID>0 && iParamSouXiaPos>=0)
		{
			pHuman->EraseSouXia(iParamSouXiaPos);// 删除拆解的捜侠录,清空原来捜侠录的数据
		}
		Msg.setBagIndex(BagIndex);
		Msg.setCurPos(iParamSouXiaPos);
		Msg.setOpt(GCSouXiaDiscard::FromBank);
		Msg.setResult(DISCARDITEM_SUCCESS);

		pGamePlayer->SendPacket(&Msg);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardSouXiaItemHandler: ok BagIndex=%d", BagIndex ) ;



	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}