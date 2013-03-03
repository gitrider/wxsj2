/********************************************************************
created:	2010/6/25
filename: 	MapServer\Server\Packets\CGEquipRefiningHandler.cpp
file path:	MapServer\Server\Packets
file base:	CGSouXiaSewHandler
file ext:	cpp
author:		genghao

purpose:	消息句柄处理

*********************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "GCEquipRefining.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCEquipRefining.h"
#include "CGEquipRefining.h"
#include "GCNotifyEquip.h"

UINT CGEquipRefiningHandler::Execute( CGEquipRefining* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	//切场景时丢弃换装消息
	//交易状态不可操作
	//摆摊状态不可操作

	// 获取炼化的装备
	BYTE iEquipItemPos = pPacket->GetEquipItemPos();
	Assert(iEquipItemPos >= 0 );
	Assert(iEquipItemPos <= MAX_BAG_SIZE);
	
	GCEquipRefining	msg;
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iEquipItemPos);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler equipment BagIndex is invalid, BagIndex = %d", iEquipItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	Item* pEquipment = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iEquipItemPos));
	Assert(NULL != pEquipment);
	//一些验证：星级，等级...
	if( NULL == pEquipment || ICLASS_EQUIP != pEquipment->GetItemClass() || !(pEquipment->IsRuler(IRL_DISCARD) )
		|| pEquipment->IsAdsorbMagic()	|| pEquipment->GetRequireLevel() < EQUIP_REFINING_MIN_LEVEL	//已经附魔或者需求等级小于40
		|| 0 == pEquipment->GetEquipGemCount())	//没有镶嵌宝石
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler equipment type is invalid, BagIndex = %d", iEquipItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}
	//获取炼化神器
	BYTE iRefiningItemPos = pPacket->GetRefiningItemPos();
	Assert(iRefiningItemPos >= 0 );
	Assert(iRefiningItemPos <= MAX_BAG_SIZE);

	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningItemPos);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler refining BagIndex is invalid, BagIndex = %d", iRefiningItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	Item* pRefiningItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningItemPos));
	Assert(NULL != pRefiningItem);
	if(NULL == pRefiningItem || pRefiningItem->GetItemClass() != ICLASS_COMITEM || pRefiningItem->GetItemType() != COMITEM_EQUIP_REFINING)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipRefiningHandler refining type is invalid, BagIndex = %d", iRefiningItemPos) ;
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}
	//逻辑处理 
	COMMITEM_INFO_TB* pGet = g_ItemTable.GetCommItemInfoTB(pRefiningItem->GetItemTableIndex());
	Assert(NULL != pGet);
	if ((pEquipment->GetRequireLevel()-1)/10+1 != pGet->m_nLevel)//验证等级
	{
		msg.SetResult(EQUIPREFINING_NOT_SUCH_ITEM);
		pGamePlayer->SendPacket(&msg);

		return PACKET_EXE_CONTINUE;
	}

	if ( 0 == pEquipment->GetAttrCount())// && pEquipment->GetLevel() 
	{
		msg.SetResult(EQUIPREFINING_NOT_HAVE_ATTR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	//EQUIP_TB* pGet = g_ItemTable.GetEquipTB(pEquipment->GetItemTableIndex());
	//Assert(NULL != pGet);
	//删除装备和炼化神符道具
	UINT iPrice = pEquipment->GetSellPrice();//保留将要删除的装备属性给后面用
	BYTE iRequireLevel = pEquipment->GetRequireLevel();
	BYTE curEquipPoint = pEquipment->GetEquipPoint();
	BYTE attrCount = 0;

	_ITEM_ATTR tmpItemAttr[MAX_ITEM_ATTR];
	for (BYTE index=0; index<pEquipment->GetAttrCount(); ++index)
	{
		tmpItemAttr[index] = pEquipment->GetEquipAttr(index);
		if (tmpItemAttr[index].m_AttrType > 0)
		{
			attrCount++;
		}
	}
	Assert(attrCount == pEquipment->GetAttrCount());
	//开始删除装备和扣除金钱
	BOOL bRet = FALSE;
	ITEM_LOG_PARAM		ItemLogParam;
	ItemLogParam.OpType	= ITEM_EQUIP_REFINING;

	bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iEquipItemPos));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	//删除炼化道具
	bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningItemPos));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}

	UINT iSoulBeadBagIndex = 0;
	Item*	pSoulBeadItem = NULL;
	//武魂珠
	if (HEQUIP_MAINHAND == curEquipPoint)
	{
		bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,ITEM_WU_SOUL_BEAD, iSoulBeadBagIndex);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		} 
		else 
		{
			msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE;
		}
		
		pSoulBeadItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSoulBeadBagIndex));
		Assert(NULL != pSoulBeadItem);
		Assert(ICLASS_SOUL_BEAD == pSoulBeadItem->GetItemClass());	
		
		pSoulBeadItem->SetSoulBeadPrice(iPrice);//由基本价格计算得出
		pSoulBeadItem->SetSoulBeadNeedLevel(iRequireLevel);
		pSoulBeadItem->SetWuSoulBeadAttr(attrCount, tmpItemAttr);
		
	}//器魂珠
	else if (EQUIP_CAP == pEquipment->GetItemType()//EQUIP_PIFENG == pEquipment->GetEquipPoint() || 
			|| EQUIP_SHOULDER == pEquipment->GetItemType() || EQUIP_HAND == pEquipment->GetItemType()
			|| EQUIP_ARMOR == pEquipment->GetItemType() || EQUIP_BOOT == pEquipment->GetItemType())
	{
		bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,ITEM_WU_SOUL_BEAD, iSoulBeadBagIndex);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		} 
		else 
		{
			msg.SetResult(EQUIPREFINING_ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE;
		}
		pSoulBeadItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSoulBeadBagIndex));
		Assert(NULL != pSoulBeadItem);
		Assert(ICLASS_SOUL_BEAD == pSoulBeadItem->GetItemClass());	
		
		pSoulBeadItem->SetSoulBeadPrice(iPrice);//由基本价格计算得出
		pSoulBeadItem->SetSoulBeadNeedLevel(iRequireLevel);
		pSoulBeadItem->SetQiSoulBeadAttr(attrCount, tmpItemAttr);
	}
	// 通知增加道具到背包
	GCNotifyEquip ItemMsg;
	Assert(pSoulBeadItem);
	ItemMsg.SetBagIndex(iSoulBeadBagIndex);
	pSoulBeadItem->SaveValueTo(ItemMsg.GetItem());
	pGamePlayer->SendPacket(&ItemMsg);
	
	msg.SetResult(EQUIPREFINING_SUCCESS);
	//pGamePlayer->SendPacket(&msg);
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}

