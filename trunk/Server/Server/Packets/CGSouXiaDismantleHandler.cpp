/********************************************************************
	created:	2010/5/18
	filename: 	MapServer\Server\Packets\CGSouXiaDiamantleHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSouXiaDismantleHandler
	file ext:	cpp
	author:		genghao
	
	purpose:	消息句柄处理
	
*********************************************************************/

#include "stdafx.h"
#include "CGSouXiaDismantle.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCSouXiaDismantle.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
#include "GCNotifyEquip.h"
#include "SouXiaLogic.h"

UINT CGSouXiaDismantleHandler::Execute( CGSouXiaDismantle* pPacket, Player* pPlayer )
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

	// 获取拆解的捜侠录和拆解道具
	BYTE iSouXiaItem = pPacket->GetSouXiaItemIndex();
	Assert(iSouXiaItem >= 0 );
	Assert(iSouXiaItem <= MAX_BAG_SIZE);

	BYTE iDismantleItem = pPacket->GetDismantleItemIndex();
	Assert(iDismantleItem >= 0 );
	Assert(iDismantleItem <= MAX_BAG_SIZE);

	GCSouXiaDismantle  dismantleMsg;
	// 背包索引获取搜侠道具信息并验证合法性
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iSouXiaItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaDismantle souxia BagIndex is invalid, BagIndex = %d", iSouXiaItem) ;
		dismantleMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&dismantleMsg);

		return PACKET_EXE_CONTINUE;
	}

	Item* pSouXiaItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSouXiaItem));
	if(NULL == pSouXiaItem || pSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pSouXiaItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaDismantle souxia type is invalid, BagIndex = %d", iSouXiaItem) ;
		dismantleMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&dismantleMsg);

		return PACKET_EXE_CONTINUE;
	}

	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT	iParamSouXiaID = pSouXiaItem->GetItemParam(0,ipv);
	INT	iParamSouXiaPos = pSouXiaItem->GetItemParam(4,ipv);
	// 从人身上通过道具参数获取具体搜侠录信息
	SouXia dismantleSouxia;
	if (iParamSouXiaID > 0 && iParamSouXiaPos >= 0)
	{
		dismantleSouxia = pHuman->GetSouXia(iParamSouXiaPos);
		BYTE curTypeCount =	dismantleSouxia.GetSouXiaData().GetCurTypeCount();
		Assert(curTypeCount > 0);
		if (curTypeCount == 0 )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaDismantle souxia  is NULL, BagIndex = %d", iSouXiaItem) ;
			dismantleMsg.SetResult(GET_ITEM_ERROR);
			pGamePlayer->SendPacket(&dismantleMsg);
			return PACKET_EXE_CONTINUE;
		}
	}
	// 背包索引获取拆解道具信息并验证合法性
	ItemContainer* pDismantleBagContainer = HumanItemLogic::GetBagContainer(pHuman, iDismantleItem);
	if (pDismantleBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaDismantle dismantle BagIndex is invalid, BagIndex = %d", iDismantleItem) ;
		dismantleMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&dismantleMsg);
		return PACKET_EXE_CONTINUE;
	}

	Item* pDismantleItem = pDismantleBagContainer->GetItem(pDismantleBagContainer->BagIndex2ConIndex(iDismantleItem));
	if(pDismantleItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pDismantleItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaDismantle dismantle type is invalid, BagIndex = %d", iDismantleItem) ;
		dismantleMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&dismantleMsg);
		return PACKET_EXE_CONTINUE;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pDismantleItem->GetItemTableIndex());
	if (NULL != pGet && ITEM_DISMANTLE == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // 使用道具没有达到限制等级
		{
			dismantleMsg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);
			pGamePlayer->SendPacket(&dismantleMsg);
			return PACKET_EXE_CONTINUE;
		}
	}


	// 拆解逻辑
	UINT  dismantleResult = 0;
	BYTE newPages = 0;//新生成的总页数
	BYTE dismantleRule = 0, dismantleRate = 0;
	SOUXIA_DATA newSouXiaDataArray[MAX_ZUOJI_ZHAOHUAN_COUNT];
	BYTE randNum = pScene->GetRand100();
	BYTE relicType = 0;
	BYTE curSouXiaCount = pHuman->GetCurSouXiaCount();
	
	//拆技能道具
	if (pGet->m_nDismantleRule[0][0] > 0)
	{
		dismantleRule = pGet->m_nDismantleRule[0][0];
		dismantleRate = pGet->m_nDismantleRule[0][1];
		if (randNum <= dismantleRate)
		{
			dismantleResult = SouXiaLogic::DismantleSouXiaSkill(dismantleRule, dismantleSouxia.GetSouXiaData(), newSouXiaDataArray, newPages, curSouXiaCount);
			relicType = ITEM_SOUXIASKILL;
		}
	} 
	else if (pGet->m_nDismantleRule[1][0] > 0)//拆配方道具
	{
		dismantleRule = pGet->m_nDismantleRule[1][0];
		dismantleRate = pGet->m_nDismantleRule[1][1];
		if (randNum <= dismantleRate)
		{
			dismantleResult = SouXiaLogic::DismantleSouXiaProduct(dismantleRule, dismantleSouxia.GetSouXiaData(), newSouXiaDataArray, newPages, curSouXiaCount);
			relicType = ITEM_PRODUCT;
		}
	}
	else if (pGet->m_nDismantleRule[2][0] > 0)//拆神兽召唤道具
	{
		dismantleRule = pGet->m_nDismantleRule[2][0];
		dismantleRate = pGet->m_nDismantleRule[2][1];
		if (randNum <= dismantleRate)
		{
			dismantleResult = SouXiaLogic::DismantleSouXiaPet(dismantleRule, dismantleSouxia.GetSouXiaData(), newSouXiaDataArray, newPages, curSouXiaCount);
			relicType = ITEM_PET_ZHAOHUAN;
		}
	}
	else if (pGet->m_nDismantleRule[3][0] > 0 )//拆坐骑召唤道具
	{
		dismantleRule = pGet->m_nDismantleRule[3][0];
		dismantleRate = pGet->m_nDismantleRule[3][1];
		if (randNum <= dismantleRate)
		{
			dismantleResult = SouXiaLogic::DismantleSouXiaZuoJi(dismantleRule, dismantleSouxia.GetSouXiaData(), newSouXiaDataArray, newPages, curSouXiaCount);
			relicType = ITEM_ZUOJI_ZHAOHUAN;
		}
	} 
	
	if (dismantleResult != SOUXIA_DISMANTLE_SUCCESS)
	{
		dismantleMsg.SetResult(dismantleResult);
		pGamePlayer->SendPacket(&dismantleMsg);
		return PACKET_EXE_CONTINUE;
	}
	// 手续费用的扣除

	// 成功删除使用的道具和捜侠录
	if (randNum <= dismantleRate)
	{
		//背包的验证
		if (HumanItemLogic::CalcBagSpace(pHuman)+2 < newPages)
		{
			dismantleMsg.SetResult(BAG_OUT_OF_SPACE);
			pGamePlayer->SendPacket(&dismantleMsg);
			return PACKET_EXE_CONTINUE;
		}

		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType	= ITEM_DISMANTLE_SOUXIA;
		//
		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iSouXiaItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else
		{
			dismantleMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&dismantleMsg);
			return PACKET_EXE_CONTINUE;
		}
		pHuman->EraseSouXia(iParamSouXiaPos);// 删除拆解的捜侠录,清空原来捜侠录的数据

		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pDismantleBagContainer->BagIndex2ConIndex(iDismantleItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else
		{
			dismantleMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&dismantleMsg);
			return PACKET_EXE_CONTINUE;
		}
		
		BYTE  bagIndexArray[MAX_ZUOJI_ZHAOHUAN_COUNT] = {0};
		// 生成捜侠录残页（多个）残页
		ItemLogParam.ItemType = ITEM_RELIC_SOUXIA; // 固定的索引
		for (int i=0; i<newPages; ++i)
		{
			UINT iBagIndex = 0;
			bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,ITEM_RELIC_SOUXIA, iBagIndex);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Item* pRelicItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iBagIndex));

			INT	iParamSouXiaID = pRelicItem->GetItemParam(0,ipv);
			INT	iParamSouXiaPos = pRelicItem->GetItemParam(4,ipv);

			SouXia curSouXiaRelic;
			if(iParamSouXiaPos <= 0 && iParamSouXiaID == 0)
			{
				UINT iSouXiaIndexParam	= pRelicItem->GetItemTableIndex();
				iParamSouXiaPos = pHuman->AddSouXia(iSouXiaIndexParam); // 增加搜侠到人身上的容器
				if(iParamSouXiaPos < 0)
				{
					dismantleMsg.SetResult(HUMAN_SOUXIA_ADD_FAIL);
					pGamePlayer->SendPacket(&dismantleMsg);

					return PACKET_EXE_CONTINUE;
				}
				// 重新设置捜侠录残页的属性以供发送使用,一页一页的设置
				newSouXiaDataArray[i].m_CurPos = iParamSouXiaPos;
				newSouXiaDataArray[i].m_SouXiaID = iSouXiaIndexParam;
				pHuman->SetSouXia(newSouXiaDataArray[i], iParamSouXiaPos);
				curSouXiaRelic = pHuman->GetSouXia(iParamSouXiaPos);//当前生成捜侠录残页

				Assert (iSouXiaIndexParam >0 && iParamSouXiaPos >=0);
				//保存信息到文件
				_SOUXIA_DB_LOAD		SouXia_db_Load;
				SouXia_db_Load.m_SouXiaData[iParamSouXiaPos] = curSouXiaRelic.GetSouXiaData();
				pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iParamSouXiaPos);	

				bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),
					pHuman->GetBaseContain()->BagIndex2ConIndex(iBagIndex),0, ipv, iSouXiaIndexParam);

				if (!bRet)
				{
					return PACKET_EXE_CONTINUE;
				}

				bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),
					pHuman->GetBaseContain()->BagIndex2ConIndex(iBagIndex),4, ipv, iParamSouXiaPos);

				if (!bRet)
				{
					return PACKET_EXE_CONTINUE;
				}
				//记录残页的类型,与记忆棒内容同位
				bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),
					pHuman->GetBaseContain()->BagIndex2ConIndex(iBagIndex),8, ipv, relicType);

				if (!bRet)
				{
					return PACKET_EXE_CONTINUE;
				}

				// 以上设置ITEM param注意保存到文件,并且要通知ITEM 的param 改变
				GCItemInfo	Msg;
				Msg.setID( iBagIndex );
				Msg.setIsNull(FALSE);
				Assert(pRelicItem);
				pRelicItem->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
				// 通知增加道具到背包
				GCNotifyEquip ItemMsg;
				ItemMsg.SetBagIndex(iBagIndex);
				pRelicItem->SaveValueTo(ItemMsg.GetItem());
				pGamePlayer->SendPacket(&ItemMsg);

				bagIndexArray[i] = iBagIndex;//残页所在格子
			}
		}

		// 发送残页生成消息
		BYTE  iCount = 0;
		for (int i=0; i<newPages; ++i)
		{
			UINT   souXiaID = newSouXiaDataArray[i].m_SouXiaID;
			if (souXiaID > 0)
			{
				SHORT curPos = newSouXiaDataArray[i].GetCurPos();
				Assert(curPos >= 0 );
				iCount++;
				dismantleMsg.SetCurBagIndex(i, bagIndexArray[i]);
				dismantleMsg.SetSouXiaData(&newSouXiaDataArray[i], i);
			}
		}
		Assert(newPages == iCount);
		dismantleMsg.SetNewSouXiaRelicCount(iCount);
		dismantleMsg.SetCurRelicType(relicType);
		dismantleMsg.SetResult(SOUXIA_DISMANTLE_SUCCESS);
		pGamePlayer->SendPacket(&dismantleMsg);

		/*msg.SetCurSouXia( (BYTE) iParamSouXiaPos);
		msg.SetSkillType( (BYTE)pGet->m_nMemoryType );
		msg.SetSkillID( (SkillID_t)iParamSkillID );
		msg.SetCurUseTime( (BYTE)(pGet->m_nUseTime < 0 ? 0 : pGet->m_nUseTime) );
	 
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &msg );
		}*/
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

