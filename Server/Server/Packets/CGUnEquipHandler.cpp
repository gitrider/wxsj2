#include "stdafx.h"
#include "CGUnEquip.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCUnEquipResult.h"
#include "GCCharEquipment.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGUnEquipHandler::Execute( CGUnEquip* pPacket, Player* pPlayer )
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
	//摆摊不可操作
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}

	BYTE nPos = pPacket->getEquipPoint();
	if (nPos>=HEQUIP_NUMBER)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUnEquip pos invalid");
		return	PACKET_EXE_ERROR;
	}

	HUMAN_EQUIP	EquipPoint = (HUMAN_EQUIP)pPacket->getEquipPoint();
	Item*	pEquipItem = HumanItemLogic::GetEquip(pHuman,EquipPoint);
	
	if(!pEquipItem)
	{
		Assert(pEquipItem);
		return	PACKET_EXE_CONTINUE;
	}
	if(pEquipItem->IsEmpty())
	{
		return	PACKET_EXE_CONTINUE;
	}

	UCHAR	itemClass = pEquipItem->GetItemClass();
	if(itemClass ==	ICLASS_EQUIP)
	{
		INT		BagIndex = -1;
		BYTE	nDestIndex = -1;
		ItemContainer*	pEquipContainer = pHuman->GetEquipContain();
		ItemContainer*	pBagContainer	= NULL;
		if(!pEquipContainer)
		{
			Assert(pEquipItem);
			return	PACKET_EXE_CONTINUE;
		}
		
		BYTE	DeliverBagIndex = pPacket->getBagIndex();
		
		GCUnEquipResult	 Msg;
		if(DeliverBagIndex != nDestIndex)
		{
			if (DeliverBagIndex>=EXTRA_CONTAINER_OFFSET)
			{
				return PACKET_EXE_CONTINUE;
			}
			pBagContainer	= HumanItemLogic::GetBagContainer(pHuman, DeliverBagIndex);
			if(pBagContainer == NULL)
			{
				return PACKET_EXE_ERROR;
			}
			if (!pBagContainer->IsCanUse())
			{
				Msg.setResult(UNEQUIP_BAG_INVALIDTIME);
				pGamePlayer->SendPacket( &Msg );
				return PACKET_EXE_CONTINUE;
			}
			BagIndex	=	g_ItemOperator.MoveItem(pEquipContainer,
				EquipPoint,
				pBagContainer,DeliverBagIndex);
		}
		else
		{
			ITEM_LOG_PARAM ItemLogParam;
			BOOL ret = HumanItemLogic::MoveItem(&ItemLogParam, pHuman, pEquipContainer, EquipPoint, BagIndex);
			if (ret)
			{
				SaveItemLog(&ItemLogParam);
			}
		}
		
		if(BagIndex>=0)
		{
			Item* pUnEquipItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);
			//Add by Jinchao.
			Msg.setEquipPoint(EquipPoint);
			Msg.setBagIndex(BagIndex);
			Msg.setItemID(pUnEquipItem->GetGUID());
			Msg.setItemTableIndex(pUnEquipItem->GetItemTableIndex());
			Msg.setResult(UNEQUIP_SUCCESS);
			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			pHuman->ItemEffectFlush();

			pGamePlayer->SendPacket( &Msg ) ;

			//如果可见
			if(pHuman->IsVisualPart(EquipPoint))
			{	
				GCCharEquipment OtherMsg;
				OtherMsg.setObjID(pHuman->GetID());
				OtherMsg.setID(EquipPoint,-1);
				
				pScene->BroadCast(&OtherMsg,pHuman,TRUE);
			}
		}
		else
		{
			if(ITEMOE_DESTOPERATOR_HASITEM == BagIndex)
			{
				Msg.setResult(UNEQUIP_HAS_ITEM);
				pGamePlayer->SendPacket( &Msg ); 
			}
			else if(ITEMOE_DESTOPERATOR_FULL == BagIndex)
			{
				Msg.setResult(UNEQUIP_BAG_FULL);
				pGamePlayer->SendPacket( &Msg ); 
			}
		}
		
	}
	else
	{
		Assert(FALSE);
		//装备为什么不是 ICLASS_EQUIP
	}
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGUnEquipHandler EquipPoint=%d itemClass=%d",
		EquipPoint, itemClass  ) ;

	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}