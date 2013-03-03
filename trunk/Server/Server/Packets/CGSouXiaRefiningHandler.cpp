/********************************************************************
	created:	2010/5/19
	filename: 	MapServer\Server\Packets\CGSouXiaRefiningHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSouXiaRefiningHandler
	file ext:	cpp
	author:		genghao
	
	purpose:	消息句柄处理
	
*********************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "CGSouXiaRefining.h"
#include "GCSouXiaRefining.h"
#include "SouXiaLogic.h"
#include "GCItemInfo.h"

UINT CGSouXiaRefiningHandler::Execute( CGSouXiaRefining* pPacket, Player* pPlayer )
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

	// 获取缝合的捜侠录和缝合道具
	BYTE iRefiningSouXiaItem = pPacket->GetRefiningSouXiaIndex();
	Assert(iRefiningSouXiaItem >= 0 );
	Assert(iRefiningSouXiaItem <= MAX_BAG_SIZE);

	BYTE iRefiningSouXiaItem2 = pPacket->GetRefiningSouXia2Index();
	Assert(iRefiningSouXiaItem2 >= 0 );
	Assert(iRefiningSouXiaItem2 <= MAX_BAG_SIZE);


	BYTE iRefiningItem = pPacket->GetRefiningItemIndex();
	Assert(iRefiningItem >= 0 );
	Assert(iRefiningItem <= MAX_BAG_SIZE);

	GCSouXiaRefining  refiningMsg;
	// 背包索引获取搜侠道具信息并验证
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningSouXiaItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia BagIndex is invalid, BagIndex = %d", iRefiningSouXiaItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningSouXiaItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem));
	if(pRefiningSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pRefiningSouXiaItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia type is invalid, BagIndex = %d", iRefiningSouXiaItem) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);

		return PACKET_EXE_ERROR;
	}

	// 背包索引获取第二本搜侠录道具信息
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningSouXiaItem2);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia BagIndex is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningSouXiaItem2 = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem2));
	if(pRefiningSouXiaItem2->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pRefiningSouXiaItem2->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia type is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);
		return PACKET_EXE_ERROR;
	}
	
	// 背包索引获取炼化道具信息
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining refiningItem BagIndex is invalid, BagIndex = %d", iRefiningItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningItem));
	if(pRefiningItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pRefiningItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining refiningItem type is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);
		return PACKET_EXE_ERROR;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pRefiningItem->GetItemTableIndex());
	if (NULL != pGet && ITEM_REFINING == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // 使用道具没有达到限制等级
		{
			refiningMsg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &refiningMsg );
			}

			return PACKET_EXE_CONTINUE;
		}
	}
	// 炼化逻辑
	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT	iParamSouXiaID = pRefiningSouXiaItem->GetItemParam(0,ipv);
	INT	iParamSouXiaPos = pRefiningSouXiaItem->GetItemParam(4,ipv);

	INT iParamSouXiaID2 = pRefiningSouXiaItem2->GetItemParam(0,ipv);
	INT iParamSouXiaPos2 = pRefiningSouXiaItem2->GetItemParam(4,ipv);
	//两本捜侠录都为空
	if ( (iParamSouXiaID == 0 && iParamSouXiaPos <= 0 ) && (iParamSouXiaID2 == 0 && iParamSouXiaPos2 <= 0 ))
	{
		refiningMsg.SetResult(SOUXIA_EMPTY);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
		return PACKET_EXE_CONTINUE;
	}

	BOOL  bRefiningResult = FALSE;
	BYTE  refiningType = 0;
	BYTE  refiningRule = 0;
	SHORT refiningRate = -1;
	BYTE randNum = pScene->GetRand100();
	SOUXIA_DATA randSouXiaData;//炼化随机后取得的新数据
	
	if (pGet->m_nRefiningRule[0][0] > 0)//炼化技能道具
	{
		refiningRule = pGet->m_nRefiningRule[0][0];
		refiningRate = pGet->m_nRefiningRule[0][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_SOUXIASKILL);
			refiningType = ITEM_SOUXIASKILL;
		}
	}
	else if (pGet->m_nRefiningRule[1][0] > 0)//炼化配方道具
	{
		refiningRule = pGet->m_nRefiningRule[1][0];
		refiningRate = pGet->m_nRefiningRule[1][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_PRODUCT);
			refiningType = ITEM_PRODUCT;
		}
	}
	else if (pGet->m_nRefiningRule[2][0] > 0)//炼化神兽召唤道具
	{
		refiningRule = pGet->m_nRefiningRule[2][0];
		refiningRate = pGet->m_nRefiningRule[2][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_PET_ZHAOHUAN);
			refiningType = ITEM_PET_ZHAOHUAN;
		}
	}
	else if (pGet->m_nRefiningRule[3][0] > 0)//炼化坐骑召唤道具
	{
		refiningRule = pGet->m_nRefiningRule[3][0];
		refiningRate = pGet->m_nRefiningRule[3][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_ZUOJI_ZHAOHUAN);
			refiningType = ITEM_ZUOJI_ZHAOHUAN;
		}
	} 

	if (!bRefiningResult)
	{
		refiningMsg.SetResult(SOUXIA_SKILL_REFINING_FAIL);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
		return PACKET_EXE_CONTINUE;
	}

	// 手续费用的扣除
	// 成功删除使用的道具和捜侠录
	if (randNum <= refiningRate)
	{
		UINT  newSouXiaIndex = 0;
		BYTE  newSouXiaCurPos = 0;

		// 取玩家身上捜侠录的数据pos 和 skillId
		if (iParamSouXiaID == 0 && iParamSouXiaPos <= 0)//第一本为空
		{
			newSouXiaIndex = iParamSouXiaID2;
			newSouXiaCurPos = iParamSouXiaPos2;
		} 
		if (iParamSouXiaID2 == 0 && iParamSouXiaPos2 <= 0)//第二本为空
		{
			newSouXiaIndex = iParamSouXiaID;
			newSouXiaCurPos = iParamSouXiaPos;
		} 
		
		// 第一本的最大页数
		BYTE maxPage = 0, maxPage2 = 0;
		pGet = g_ItemTable.GetSouXiaTB(pRefiningSouXiaItem->GetItemTableIndex());
		Assert(pGet);
		maxPage = pGet->m_nMaxPages[0];
		// 第二本最大页数
		pGet = g_ItemTable.GetSouXiaTB(pRefiningSouXiaItem2->GetItemTableIndex());
		Assert(pGet);
		maxPage2 = pGet->m_nMaxPages[0];

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_REFINING_SOUXIA;
		UINT  newSouXiaBagIndex = 0;//新的捜侠录item背包索引

		if (maxPage >= maxPage2) // 第一本的最大页大于第二本则保留第一本
		{
			BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem2));
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			else 
			{
				refiningMsg.SetResult(ERASE_ITEM_FAIL);
				pGamePlayer->SendPacket(&refiningMsg);
				return PACKET_EXE_CONTINUE;
			}
			if (iParamSouXiaPos2 >= 0)
			{
				pHuman->EraseSouXia(iParamSouXiaPos2);//人身上销毁第二本
			}
			
			newSouXiaBagIndex = iRefiningSouXiaItem;//保留第一本的数据的bagindex

			if ( (iParamSouXiaID > 0  && iParamSouXiaPos >= 0) && (iParamSouXiaID2 > 0  && iParamSouXiaPos2 >= 0))
			{
				newSouXiaIndex = iParamSouXiaID;
				newSouXiaCurPos = iParamSouXiaPos;
			}
		} 
		else // 销毁第一本
		{
			BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem));
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			else 
			{
				refiningMsg.SetResult(ERASE_ITEM_FAIL);
				pGamePlayer->SendPacket(&refiningMsg);
				return PACKET_EXE_CONTINUE;
			}

			if (iParamSouXiaPos >= 0)
			{
				pHuman->EraseSouXia(iParamSouXiaPos);//人身上销毁第一本
			}
			newSouXiaBagIndex = iRefiningSouXiaItem2;//保留第二本的数据的bagindex

			if ( (iParamSouXiaID > 0  && iParamSouXiaPos >= 0) && (iParamSouXiaID2 > 0  && iParamSouXiaPos2 >= 0))
			{
				newSouXiaIndex = iParamSouXiaID2;
				newSouXiaCurPos = iParamSouXiaPos2;
			}
		}
		Assert (newSouXiaIndex > 0 && newSouXiaCurPos >= 0);
		randSouXiaData.m_CurPos = newSouXiaCurPos;
		randSouXiaData.m_SouXiaID = newSouXiaIndex;
		// 销毁炼化道具
		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else 
		{
			refiningMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&refiningMsg);
			return PACKET_EXE_CONTINUE;
		}
		// 改变保留的捜侠录道具的信息
		Item* pNewSouXiaItem = HumanItemLogic::GetBagItem(pHuman, pBagContainer->BagIndex2ConIndex(newSouXiaBagIndex));
		Assert(pNewSouXiaItem);

		bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex),0, ipv, newSouXiaIndex);
		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex), 4, ipv, newSouXiaCurPos);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		/*bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex),8, ipv, refiningType);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}*/

		// 以上设置ITEM param注意保存到文件,并且要通知ITEM 的param 改变
		GCItemInfo	Msg;
		Msg.setID( newSouXiaBagIndex );
		Msg.setIsNull(FALSE);
		pNewSouXiaItem->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);

		pHuman->SetSouXia(randSouXiaData, newSouXiaCurPos);
		// 保存到文件
		_SOUXIA_DB_LOAD		SouXia_db_Load;
		SouXia_db_Load.m_SouXiaData[newSouXiaCurPos] = randSouXiaData;
		pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, newSouXiaCurPos);	
		
		// 发送新的捜侠录消息
		refiningMsg.SetNewItemBagIndex(newSouXiaBagIndex);
		refiningMsg.SetSouXiaData(&randSouXiaData);//炼化以后的捜侠录数据
		refiningMsg.SetCurRefiningType(refiningType);
		refiningMsg.SetResult(SOUXIA_REFINING_SUCCESS);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
	}	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

