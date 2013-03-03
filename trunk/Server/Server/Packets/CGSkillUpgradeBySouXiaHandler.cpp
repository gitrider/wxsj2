/********************************************************************
	created:	2010/5/10
	filename: 	MapServer\Server\Packets\CGSkillUpgradeBySouXiaHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSkillUpgradeBySouXiaHandler
	file ext:	cpp
	author:		genghao
	
	purpose:	消息句柄处理
	
*********************************************************************/

#include "stdafx.h"
#include "CGSkillUpgradeBySouXia.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "HumanSkillUpgrad.h"
#include "GCSkillUpgradeBySouXia.h"
#include "SouXia.h"
#include "GCDiscardItemResult.h"
#include "GCDetailEquipList.h"

UINT CGSkillUpgradeBySouXiaHandler::Execute( CGSkillUpgradeBySouXia* pPacket, Player* pPlayer )
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

	// 获取学习的技能类型
	BYTE iStudySkillType = pPacket->GetStudySkillType();
	Assert(iStudySkillType >= ITEM_SOUXIASKILL );
	Assert(iStudySkillType <= ITEM_ZUOJI_ZHAOHUAN);

	GCAddSouXiaSkill  msg;
	// 获取背包索引信息,获取并验证客户端记忆棒数据
	BYTE iBagIndex = pPacket->GetBagIndex();
	Assert(iBagIndex >= 0 );

	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iBagIndex);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia BagIndex is invalid, BagIndex = %d", iBagIndex) ;
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_ERROR;
	}

	Item* pUseItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iBagIndex));
	if(pUseItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pUseItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia type is invalid, BagIndex = %d", iBagIndex) ;
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_ERROR;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pUseItem->GetItemTableIndex());
	if (NULL != pGet && ITEM_MEMORYCLUB == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // 使用USB没有达到限制等级
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia level is error, BagIndex = %d", iBagIndex) ;

			// 发送增加技能成功/失败的消息,更新捜侠录
			msg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);//设置结果

			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}

		Assert(pGet->m_nMemoryType == iStudySkillType);

		if(pGet->m_nMemoryType != iStudySkillType) // 如果请求学的技能类型不是当前道具包含的技能类型则校验失败
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillUpgradeBySouXia level is error, BagIndex = %d", iBagIndex) ;
			// 发送增加技能成功/失败的消息,更新捜侠录
			msg.SetResult(SOUXIA_SKILL_STUDY_TYPE_ERROR);//设置结果
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}

			return PACKET_EXE_CONTINUE;
		}
	}
	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	SkillID_t	iParamSkillID = pUseItem->GetItemParam(8,ipv); // 搜侠记忆棒记忆的内容
	if(iParamSkillID <= 0 )
	{
		// 发送增加技能成功/失败的消息,更新捜侠录		
		msg.SetResult(SOUXIA_SKILL_STUDY_TYPE_ERROR);//设置结果
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &msg );
		}
		return PACKET_EXE_CONTINUE;
	}

	// 验证捜侠录信息
	Item* pEquipSouXiaItem = pHuman->GetEquipContain()->GetItem(HEQUIP_SOUXIA);
	Assert(pEquipSouXiaItem != NULL);
	if (NULL == pEquipSouXiaItem)
	{
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	if(pEquipSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pEquipSouXiaItem->GetItemType())
	{
		msg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	
	SOUXIA_INFO_TB* pGetEquip = g_ItemTable.GetSouXiaTB(pEquipSouXiaItem->GetItemTableIndex());
	if (NULL != pGetEquip && ITEM_SOUXIALU == pGetEquip->m_nUseType)//对于类型为0的，该子类有作用
	{
		// 该列填写0，该搜侠录为入门搜侠录，填写1表示为初级搜侠录，填写2表示为中级搜侠录，填写3表示为高级搜侠录，填写4表示特殊类型搜侠录。填写-1表示该列无效
		if(0 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_SOUXIASKILL)
		{
			return PACKET_EXE_CONTINUE;
		} 
		else if (1 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_PRODUCT)
		{
			return PACKET_EXE_CONTINUE;
		} 
		else if (2 == pGetEquip->m_nSouXiaLevel && iStudySkillType > ITEM_PET_ZHAOHUAN)
		{
			return PACKET_EXE_CONTINUE;
		} 
	}
	// 读取已经装备的捜侠录的参数属性,决定是否新捜侠
	
	INT	iParamSouXiaID = pEquipSouXiaItem->GetItemParam(0,ipv);
	INT	iSouXiaPos = pEquipSouXiaItem->GetItemParam(4,ipv);

	SouXia curSouXia;
	// 学习技能
	if (ITEM_PRODUCT != pGet->m_nMemoryType)
	{
		HumanSkillUpgrade oSkillUpgrade;
		if(!oSkillUpgrade.HumanSkillLevelUp( pHuman, iParamSkillID, 1 ))
		{
			msg.SetResult(SOUXIA_SKILL_UPGRADE_FAIL);//学习失败
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}
	}
	else 
	{
		//pHuman->__SetPrescrLearnedFlag(iParamSkillID, TRUE);
	}

	if(iSouXiaPos <= 0 && iParamSouXiaID == 0)
	{
		UINT iSouXiaIndexParam	= pEquipSouXiaItem->GetItemTableIndex();
		iSouXiaPos = pHuman->AddSouXia(iSouXiaIndexParam); // 增加搜侠到人身上的容器
		Assert(iSouXiaPos >= 0);
		if(iSouXiaPos < 0)
		{
			msg.SetResult(HUMAN_SOUXIA_ADD_FAIL);
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &msg );
			}
			return PACKET_EXE_CONTINUE;
		}

		curSouXia = pHuman->GetSouXia(iSouXiaPos);//当前装备的捜侠录
		_SOUXIA_DB_LOAD		SouXia_db_Load;
		SouXia_db_Load.m_SouXiaData[iSouXiaPos] = curSouXia.GetSouXiaData();
		pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iSouXiaPos);	

		BOOL bRet = g_ItemOperator.SetItemParam( pHuman->GetEquipContain(),
			pHuman->GetEquipContain()->BagIndex2ConIndex(HEQUIP_SOUXIA),0, ipv, iSouXiaIndexParam);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		bRet = g_ItemOperator.SetItemParam( pHuman->GetEquipContain(),
			pHuman->GetEquipContain()->BagIndex2ConIndex(HEQUIP_SOUXIA),4, ipv, iSouXiaPos);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}
		// 以上设置ITEM param注意保存到文件,并且要通知ITEM 的param 改变
		UINT RetPart = 0;
		GCDetailEquipList	SelfEquipMsg;
		SelfEquipMsg.setObjID( pHuman->GetID() );
		pEquipSouXiaItem->SaveValueTo(SelfEquipMsg.GetEquipData((HUMAN_EQUIP)HEQUIP_SOUXIA));
		RetPart|=(1<<((UINT)HEQUIP_SOUXIA));
		SelfEquipMsg.SetAskPart(RetPart);
		pHuman->GetPlayer()->SendPacket(&SelfEquipMsg);
	}

	Assert(iSouXiaPos >= 0 && iSouXiaPos < MAX_SOUXIA_CONTAINER);
	curSouXia = pHuman->GetSouXia(iSouXiaPos);
	// 人身上搜侠容器的第几个位置
	BYTE result = curSouXia.AddSkillToSouXia(iStudySkillType, iParamSkillID, pGet->m_nUseTime, pGetEquip->m_nMaxPages);

	if( result != SOUXIA_SKILL_UPGRADE_SUCCESS)//增加技能到相应的类型 
	{
		msg.SetResult((BYTE)result);//设置结果
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &msg );
		}
		return PACKET_EXE_CONTINUE;
	}
	// 发送增加技能成功/失败的消息,更新捜侠录
	msg.SetCurSouXiaPos( (SHORT) iSouXiaPos);
	msg.SetSkillType( (BYTE)pGet->m_nMemoryType );
	msg.SetSkillID( (SkillID_t)iParamSkillID );
	msg.SetCurUseTime( (SHORT)(pGet->m_nUseTime < 0 ? 0 : pGet->m_nUseTime) );
	msg.SetResult((BYTE)result);//设置结果

	Player* pPlayer = pHuman->GetPlayer();
	if( pPlayer != NULL )
	{
		pPlayer->SendPacket( &msg );
	}
	// 更新与保存
	pHuman->SetSouXia(curSouXia.GetSouXiaData(), iSouXiaPos);
	_SOUXIA_DB_LOAD		SouXia_db_Load;
	SouXia_db_Load.m_SouXiaData[iSouXiaPos] = curSouXia.GetSouXiaData();
	pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iSouXiaPos);	

	// 成功则删除搜侠记忆棒,返回该玩家销毁物品消息
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_STUDY_SOUXIA;
	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iBagIndex));
	if(bRet)
	{
		SaveItemLog(&ItemLogParam);
	}
	else
	{
		msg.SetResult(ERASE_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		Assert(FALSE);//删除失败
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

