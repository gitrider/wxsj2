#include "stdafx.h"
#include "CGEquipCancelMagic.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
#include "GCEquipCancelMagicResult.h"

using namespace Packets;

uint CGEquipCancelMagicHandler::Execute( CGEquipCancelMagic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if ( pScene == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// 检测线程执行资源是否正确
	Assert( MyGetCurrentThreadID() == pScene->m_ThreadID );

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

	Item*	pEquipItem	= NULL;
	Item*	pGemItem	= NULL;

	UINT	EquipIndex	= pPacket->GetEquipPos();
	UINT	GemIndex	= pPacket->GetGemPos();

	GCEquipCancelMagicResult Msg;
	EQUIPCANCELMAGIC_RESULT	Result = EQUIPCANCELMAGIC_EQUIP_FAIL;

	ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer( pHuman, EquipIndex );
	if( pItemContainer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler: BagIndex error, BagIndex=%d", EquipIndex) ;
		return PACKET_EXE_ERROR;
	}
	if (!pItemContainer->IsCanUse())
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler: ItemContainer is invalid, ContainerType=%d, EquipIndex=%d", 
			pItemContainer->GetContainerType(), EquipIndex) ;
		return PACKET_EXE_ERROR;
	}
	
	pEquipItem = pItemContainer->GetItem( pItemContainer->BagIndex2ConIndex(EquipIndex) );
	Assert(pEquipItem);

	if (pEquipItem->IsEmpty())
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler pEquipItem is Null at  EquipIndex= %d", EquipIndex) ;
		return PACKET_EXE_CONTINUE;
	}

	if (pEquipItem->GetItemClass() != ICLASS_EQUIP )
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler GetItemClass is not ICLASS_EQUIP ") ;
		return PACKET_EXE_CONTINUE;
	}

	if ( pEquipItem->GetItemType() > EQUIP_ADORN )
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler GUID is different ") ;
		return PACKET_EXE_CONTINUE;
	}

	// 是否是正确的装备类型
	switch(pEquipItem->GetItemType())
	{
		// 正确类型
	case EQUIP_CAP:
	case EQUIP_SHOULDER:
	case EQUIP_ARMOR:
	case EQUIP_HAND:
	case EQUIP_BOOT:
		{

		}
		break;
	default:
		{
			// 返回装备类型不正确消息
			Result = EQUIPCANCELMAGIC_EQUIPTYPE_ERROR;
			Msg.setResult( Result );
			pGamePlayer->SendPacket( &Msg );

			return PACKET_EXE_CONTINUE;
		}
	}

	// 判断装备是否已经附魔
	if ( !pEquipItem->IsAdsorbMagic() ) 
	{
		// 返回没有附魔信息
		Result = EQUIPCANCELMAGIC_EQUIP_NO_MAGIC;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler Equip Is't Have Maigc ") ;
		return PACKET_EXE_CONTINUE;
	}


	ItemContainer* pGemContainer = HumanItemLogic::GetBagContainer( pHuman, GemIndex );
	if (pGemContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler: GemIndex error, GemIndex=%d", GemIndex) ;
		return PACKET_EXE_ERROR;
	}

	if (!pGemContainer->IsCanUse())
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler: ItemContainer is invalid, ContainerType=%d, GemIndex=%d", 
			pItemContainer->GetContainerType(), GemIndex) ;
		return PACKET_EXE_ERROR;
	}
	
	pGemItem = pGemContainer->GetItem(pGemContainer->BagIndex2ConIndex(GemIndex));
	Assert(pGemItem);

	if (pGemItem->IsEmpty())
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler pEquipItem is Null at  GemIndex= %d", GemIndex) ;
		return PACKET_EXE_CONTINUE;
	}
	if (pGemItem->GetItemClass() != ICLASS_COMITEM)
	{
		// 返回没有放入退魔石
		Result = EQUIPCANCELMAGIC_NO_GEM;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler GetItemClass is not ICLASS_GEM ") ;
		return PACKET_EXE_CONTINUE;
	}
	if (pGemItem->GetItemType() != COMITEM_EQUIP_CANCEL_MAGIC)
	{
		// 返回没有放入退魔石
		Result = EQUIPCANCELMAGIC_NO_GEM;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler GetItemClass is not COMITEM_EQUIP_CANCEL_MAGIC ") ;
		return PACKET_EXE_CONTINUE;
	}

	///////////////////////////////////////////////////////////////
	//消耗金钱计算方法：级别的1.8次方 除以10 再加2
	// 附魔消耗的金钱计算,暂时按照平方来计算,以后会修改
	INT UseMoney = 0;
	INT nEquipLevel = (pEquipItem->GetRequireLevel())/10;
	nEquipLevel *= nEquipLevel;
	UseMoney = nEquipLevel/10+2;
	///////////////////////////////////////////////////////////////	

	// 判断玩家的游戏币是否够用
	if ( pHuman->GetMoney() < UseMoney )
	{
		//返回金钱不够消息
		Result = EQUIPCANCELMAGIC_MONEY_NOT_ENOUGH;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler Not Enough Money ") ;
		return PACKET_EXE_CONTINUE;
	}

	if ( pHuman->EquipCancelMagic( pEquipItem ) == EQUIPCANCELMAGIC_SUCCESS )
	{
		// 删除退魔石
		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType = ITEM_CANCELMAGIC_CONSUME;
		ItemLogParam.CharGUID = pHuman->GetGUID();

		BOOL bRet = HumanItemLogic::EraseBagItem( &ItemLogParam, pHuman, GemIndex );
		if (bRet)
		{
			SaveItemLog( &ItemLogParam );
		}

		// 删除退魔消耗的金钱
		pHuman->SetMoney( pHuman->GetMoney()-UseMoney );

		MONEY_LOG_PARAM MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_CANCELMAIGC_REDUCE;
		MoneyLogParam.Count		=	UseMoney;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		SaveMoneyLog( &MoneyLogParam );

		Result = EQUIPCANCELMAGIC_SUCCESS;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler::EquipCancelMagic success");

	}
	else
	{
		// 返回失败信息
		Result = EQUIPCANCELMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipCancelMagicHandler: EquipCancelMagic Fail") ;
		return PACKET_EXE_CONTINUE;
	}
	
	GCItemInfo EquipMsg;
	EquipMsg.setIsNull( FALSE );
	EquipMsg.setID( pPacket->GetEquipPos() );
	pEquipItem->SaveValueTo(EquipMsg.getItem());
	pHuman->GetPlayer()->SendPacket(&EquipMsg);

	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}