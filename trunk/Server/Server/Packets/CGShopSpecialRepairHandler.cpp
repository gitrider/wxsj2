#include "stdafx.h"
#include "CGShopSpecialRepair.h"
#include "GCShopSell.h"
#include "GCShopBuy.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "ShopManager.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCShopRepair.h"
#include "GCShopSpecialRepairResult.h"

uint CGShopSpecialRepairHandler::Execute( CGShopSpecialRepair* pPacket, Player* pPlayer )
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

	//交易状态不可操作
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//摆摊状态不可操作
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	BYTE	Opt         =	pPacket->GetOpt();
	BYTE	BagIndex	=	pPacket->GetBagIndex();		//包中的位置

	
	Item*	pCurItem = NULL;
	INT				MoneySpent = 0;
	INT				MoneyLast = 0;
	INT				MoneyHave  = pHuman->GetMoney();
	INT				RepairedIndex = 0;
	GCShopSpecialRepairResult MsgResult;
	GCShopSpecialRepairResult::REPAIRED_ITEM	ItemList;

	if(Opt == CGShopSpecialRepair::FromBag)
	{
		//一定是修理单个
		ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman, BagIndex);
		if (pItemContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler BagIndex is invalid, BagIndex = %d", BagIndex) ;

			return PACKET_EXE_ERROR;
		}
		if (!pItemContainer->IsCanUse())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler extrabag is invalid time BagIndex = %d", BagIndex) ;
			return PACKET_EXE_CONTINUE ;
		}

		pCurItem = pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
		if(!pCurItem->IsEmpty())
		{
			if(pCurItem->GetItemClass() != ICLASS_EQUIP)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler pCurItem->GetItemClass() != ICLASS_EQUIP ") ;
				return PACKET_EXE_ERROR ;
			}
			EQUIP_SPECIALREPAIR_TB* pSpecialRepair = g_ItemTable.GetSpecialRepairTB(); 
			Assert (pSpecialRepair != NULL);
			if (!pSpecialRepair)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[CGShopSpecialRepairHandler::Execute] GetSpecialRepairTB index is error");
				return PACKET_EXE_ERROR;
			}
		
			//获得Human背包中对应ItemIndex的物品位置
			UINT	nItemCount	= 	HumanItemLogic::CalcBagItemCount(pHuman, pSpecialRepair->m_ItemID);
			if((nItemCount == 0)||(nItemCount < pPacket->GetItemNum()))
			{
				MsgResult.SetResult(EQUIPSPECIALREPAIR_ITEM);
				pGamePlayer->SendPacket(&MsgResult);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler nItemIndex = %d", pSpecialRepair->m_ItemID) ;
				return PACKET_EXE_CONTINUE ;
			}

			FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
			FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
			FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();
			if(fMax != fCurMax)
			{
				UINT nUsedItem = 0;
				for (UINT i=0; i<pPacket->GetItemNum(); ++i)
				{
					++nUsedItem;
					fCurMax = pSpecialRepair->m_RecoverPoint + fCurMax;
					if (fCurMax>=fMax)
					{
						fCurMax = fMax;
						break;
					}
					
				}
				
				HumanItemLogic::SetBagItemCurMaxDur(pHuman,(UINT)BagIndex, (INT)fCurMax);
				HumanItemLogic::SetBagItemDur(pHuman,(UINT)BagIndex, (INT)fCurMax);
				
				
				ITEM_LOG_PARAM LogParam;
				LogParam.OpType     =   ITEM_SPECIALREPAIR_CONSUME;
				LogParam.CharGUID	=	pHuman->GetGUID();
				LogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				LogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				LogParam.SceneID	=	pHuman->getScene()->SceneID();
				BOOL bOK = HumanItemLogic::EraseBagItem(&LogParam,pHuman, pSpecialRepair->m_ItemID, nUsedItem);
				Assert(bOK);
				SaveItemLog(&LogParam);

				ItemList.IsIn		=	GCShopSpecialRepairResult::BAG;	
				ItemList.nCurDur		=  pCurItem->GetDurPoints(); 
				ItemList.nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
				ItemList.BagIndex	=  BagIndex;
				MsgResult.SetResult(EQUIPSPECIALREPAIR_SUCCESS);
				MsgResult.SetReList(&ItemList);
				pHuman->GetPlayer()->SendPacket(&MsgResult);
				return PACKET_EXE_CONTINUE ;
			}
			else
			{
				MsgResult.SetResult(EQUIPSPECIALREPAIR_NOREAPIR);
				pGamePlayer->SendPacket(&MsgResult);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler needn't repair") ;
				return PACKET_EXE_CONTINUE ;
			}
		}
		else
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler BagIndex = %d", BagIndex) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if(Opt == CGShopSpecialRepair::FromEquip)
	{
		pCurItem = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)BagIndex);

		if(!pCurItem->IsEmpty())
		{//有东西
			if(pCurItem->GetItemClass() != ICLASS_EQUIP)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler pCurItem->GetItemClass() != ICLASS_EQUIP ") ;
				return PACKET_EXE_ERROR ;
			}
			EQUIP_SPECIALREPAIR_TB* pSpecialRepair = g_ItemTable.GetSpecialRepairTB(); 
			Assert (pSpecialRepair != NULL);
			if (!pSpecialRepair)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[CGShopSpecialRepairHandler::Execute] GetSpecialRepairTB index is error");
				return PACKET_EXE_ERROR;
			}
			//获得Human背包中对应ItemIndex的物品位置
			UINT	nItemCount	= 	HumanItemLogic::CalcBagItemCount(pHuman, pSpecialRepair->m_ItemID);
			if((nItemCount == 0)||(nItemCount < pPacket->GetItemNum()))
			{
				MsgResult.SetResult(EQUIPSPECIALREPAIR_ITEM);
				pGamePlayer->SendPacket(&MsgResult);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler nItemIndex = %d", pSpecialRepair->m_ItemID) ;
				return PACKET_EXE_CONTINUE ;
			}

			FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
			FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
			FLOAT fCurMax	= (FLOAT)pCurItem->GetCurMaxDurPoint();

			if(fMax != fCurMax)
			{
				UINT nUsedItem = 0;
				for (UINT i=0; i<pPacket->GetItemNum(); ++i)
				{
					++nUsedItem;
					fCurMax = pSpecialRepair->m_RecoverPoint + fCurMax;
					if (fCurMax>=fMax)
					{
						fCurMax = fMax;
						break;
					}
				}
				
				HumanItemLogic::SetCurEquipMaxDur(pHuman,(HUMAN_EQUIP)BagIndex, (INT)fCurMax);
				HumanItemLogic::SetEquipDur(pHuman,(HUMAN_EQUIP)BagIndex, (INT)fCurMax);

				ITEM_LOG_PARAM LogParam;
				LogParam.OpType     =   ITEM_SPECIALREPAIR_CONSUME;
				LogParam.CharGUID	=	pHuman->GetGUID();
				LogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				LogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				LogParam.SceneID	=	pHuman->getScene()->SceneID();
				BOOL bOK = HumanItemLogic::EraseBagItem(&LogParam,pHuman, pSpecialRepair->m_ItemID,nUsedItem);
				Assert(bOK);
				SaveItemLog(&LogParam);

				ItemList.IsIn		=	GCShopSpecialRepairResult::EQUIP;	
				ItemList.nCurDur		=  pCurItem->GetDurPoints(); 
				ItemList.nCurMaxDur	=  pCurItem->GetCurMaxDurPoint(); 
				ItemList.BagIndex	=  BagIndex;
				MsgResult.SetResult(EQUIPSPECIALREPAIR_SUCCESS);
				MsgResult.SetReList(&ItemList);
				pHuman->GetPlayer()->SendPacket(&MsgResult);

				if ((INT)fCur == 0)
				{
					pHuman->ItemEffectFlush();
				}
				return PACKET_EXE_CONTINUE ;
			}
			else
			{
				MsgResult.SetResult(EQUIPSPECIALREPAIR_NOREAPIR);
				pGamePlayer->SendPacket(&MsgResult);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSpecialRepairHandler needn't repair") ;
				return PACKET_EXE_CONTINUE ;
			}
		}

	}
	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSpecialRepairHandler error Opt=", Opt);
	return PACKET_EXE_ERROR;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
