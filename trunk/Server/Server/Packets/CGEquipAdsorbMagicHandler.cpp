#include "stdafx.h"
#include "CGEquipAdsorbMagic.h"

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
#include "GCEquipAdsorbMagicResult.h"

using namespace Packets;

UINT CGEquipAdsorbMagicHandler::Execute( CGEquipAdsorbMagic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer*pGamePlayer = (GamePlayer*)pPlayer;
	Assert(pGamePlayer);

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert(pHuman);

	Scene* pScene = pHuman->getScene();
	if (pScene == NULL)
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

	GCEquipAdsorbMagicResult Msg;
	EQUIPADSORMAGIC_RESULT	 Result = EQUIPADSORMAGIC_EQUIP_FAIL;

	UINT EquipPoint			= INVALID_ITEM_POS;
	UINT BagIndex			= INVALID_ITEM_POS;

	Item*	pEquipItem		= NULL;
	Item*   pBeadItem		= NULL;
	switch(pPacket->GetEquipPosType())
	{
	case ENUM_EQUIP_POINT:
		{
			EquipPoint = pPacket->GetEquipPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
				return PACKET_EXE_ERROR;
			}

			ItemContainer* pEquipContainer = pHuman->GetEquipContain();
			if(NULL==pEquipContainer)
			{
				AssertEx(FALSE,"[CGEquipAdsorbMagic]: NULL EquipContainer pointer found!");
			}
			pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
			Assert( pEquipItem );

			if ( pEquipItem->IsEmpty() )
			{
				//在这返回附魔失败信息
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
				return PACKET_EXE_CONTINUE;
			}
			
			// 穿在身上的一定是装备，如果不是，则报错
			Assert( pEquipItem->GetItemClass() == ICLASS_EQUIP );

		}
		break;
	case ENUM_BAG:
		{
			BagIndex = pPacket->GetEquipPos();
			ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
			if( pItemContainer == NULL )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: BagIndex error") ;
				return PACKET_EXE_ERROR;
			}

			if (!pItemContainer->IsCanUse())
			{
				//在这返回附魔失败信息
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: ItemContainer is invalid") ;
				return PACKET_EXE_ERROR;
			}
			
			pEquipItem	= pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			Assert(pEquipItem);

			if ( pEquipItem->IsEmpty() )
			{
				//在这返回附魔失败信息
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
				return PACKET_EXE_CONTINUE;
			}
			
			// 在背包里的物品有可能不是装备，所以不直接报错，而是向客户端发送失败消息包
			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				//在这返回附魔失败信息
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler GetItemClass is not ICLASS_EQUIP ") ;
				return PACKET_EXE_CONTINUE;
			}
			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				//在这返回附魔失败信息
				Result = EQUIPADSORMAGIC_EQUIP_FAIL;
				Msg.setResult( Result );
				pGamePlayer->SendPacket( &Msg );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler EquipAdsorbmagic GUID is defferent ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipFromType error, EquipFromType=%d", pPacket->GetEquipPosType()) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}

	// 获得要装备的魂珠，魂珠一定是在背包中
	UINT nBeadPos = pPacket->GetBeadPos();

	ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,nBeadPos);
	if( pItemContainer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: BagIndex error") ;
			return PACKET_EXE_ERROR;
	}
	if (!pItemContainer->IsCanUse())
	{
		//在这返回附魔失败信息
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: ItemContainer is invalid") ;
			return PACKET_EXE_ERROR;
	}

	pBeadItem = pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(nBeadPos));
	Assert(pBeadItem);

	if ( pBeadItem->IsEmpty() )
	{
		//在这返回附魔失败信息
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler pEquipItem is Null ") ;
		return PACKET_EXE_CONTINUE;
	}

	// 如果获得的对象不是魂珠，返回失败信息
	if (pBeadItem->GetItemClass() != ICLASS_SOUL_BEAD)
	{
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler GetItemClass is not ICLASS_SOUL_BEAD ") ;
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
			Result = EQUIPADSORMAGIC_EQUIPBYTE_ERROR;
			Msg.setResult( Result );
			pGamePlayer->SendPacket( &Msg );

			return PACKET_EXE_CONTINUE;
		}
	}

	// 判断装备是否已经附魔
	if ( pEquipItem->IsAdsorbMagic() ) 
	{
		Result = EQUIPADSORMAGIC_EQUIP_HAVE_MAGIC;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler Equip Is Have Maigc ") ;
		return PACKET_EXE_CONTINUE;
	}
	
	// 判断珠子的等级是否大于玩家的等级
	if ( pHuman->GetLevel() < pBeadItem->GetSoulBeadNeedLevel() )
	{
		Result = EQUIPADSORMAGIC_BEAMLEVEL_BIG_HUMANLEVEL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler BeadLevel Big Then HumanLevel ") ;
		return PACKET_EXE_CONTINUE;
	}

	// 判断珠子的等级是否大于武器的使用等级
	if ( pEquipItem->GetRequireLevel() < pBeadItem->GetSoulBeadNeedLevel() )
	{
		Result = EQUIPADSORMAGIC_BEAMLEVEL_BIG_EQUIPLEVEL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler BeadLevel Big Then EquipLevel ") ;
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
		Result = EQUIPADSORMAGIC_MONEY_NOT_ENOUGH;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler Not Enough Money ") ;
		return PACKET_EXE_CONTINUE;
	}

	// 附魔过程
	if (pHuman->EquipAdsorbMagic(pEquipItem, pBeadItem) == EQUIPADSORMAGIC_SUCCESS)
	{
		// 删除魂珠
		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType	= ITEM_ADSORBMAGIC_CONSUME;
		ItemLogParam.CharGUID = pHuman->GetGUID();

		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, nBeadPos);
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}

		// 删除附魔消耗的金钱
		pHuman->SetMoney( pHuman->GetMoney()-UseMoney );

		MONEY_LOG_PARAM MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_ADSORBMAIGC_REDUCE;
		MoneyLogParam.Count		=	UseMoney;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		SaveMoneyLog( &MoneyLogParam );

		Result = EQUIPADSORMAGIC_SUCCESS;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );


		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler::EquipAdsorbMagic success") ;
	}
	else
	{
		// 返回失败信息
		Result = EQUIPADSORMAGIC_EQUIP_FAIL;
		Msg.setResult( Result );
		pGamePlayer->SendPacket( &Msg );
		
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAdsorbMagicHandler: EquipAdsorbMagic Fail") ;
		return PACKET_EXE_CONTINUE;
	}

	GCItemInfo EquipMsg;
	EquipMsg.setIsNull(FALSE);
	EquipMsg.setID( pPacket->GetEquipPos() );
	pEquipItem->SaveValueTo(EquipMsg.getItem());
	pHuman->GetPlayer()->SendPacket(&EquipMsg);

return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}