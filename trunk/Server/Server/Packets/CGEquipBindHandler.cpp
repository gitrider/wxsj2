#include "stdafx.h"
#include "CGEquipBind.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCEquipBindResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGEquipBindHandler::Execute(CGEquipBind* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

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

	ObjID_t		NpcObjID=	pPacket->GetUniqueID();
	Obj* pNpcObj = pScene->GetObjManager()->GetObj(NpcObjID);
	if (pNpcObj == NULL)
	{
		return PACKET_EXE_ERROR;
	}
	else
	{
		if( pNpcObj->GetObjType() != Obj::OBJ_TYPE_MONSTER )
		{
			return PACKET_EXE_ERROR;
		}
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}


	EQUIPBIND_RESULT	Result = EQUIPBIND_FAIL;
	GCEquipBindResult  ResultPacket;

	Item*		pEquipItem		= NULL;
	INT			EquipPoint		= INVALID_ITEM_POS;
	if(pPacket->GetEquipPosType()>=INVALID_ITEM_POS_TYPE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler Equip posType error, posType=%d", pPacket->GetEquipPosType()) ;
		return PACKET_EXE_ERROR;
	}

	if(pPacket->GetEquipPosType() == ENUM_BAG)
	{
		UINT BagIndex = pPacket->GetPos();
		ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
		if( pItemContainer == NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler: BagIndex error, BagIndex=%d", BagIndex) ;
			return PACKET_EXE_ERROR;
		}
		if (!pItemContainer->IsCanUse())
		{
			Result = EQUIPBIND_BAG_INVALID;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d", 
				pItemContainer->GetContainerType(), BagIndex) ;
				return PACKET_EXE_ERROR;
		}

		pEquipItem		=	pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
		if(pEquipItem->IsEmpty())
		{
			Result = EQUIPBIND_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler Equip empyty, bagpos=%d", pPacket->GetPos()) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pEquipItem->GetItemClass()!=ICLASS_EQUIP)
		{
			Result = EQUIPBIND_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}

		if (pEquipItem->GetItemType() > EQUIP_ADORN)
		{
			Result = EQUIPBIND_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}
	}

	if(pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)
	{
		if(pPacket->GetPos()>HEQUIP_ADORN2)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler Item pos error") ;
			return PACKET_EXE_ERROR;
		}
		EquipPoint = pPacket->GetPos();
		ItemContainer* pEquipContainer = pHuman->GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[CGEquipBindHandler]: NULL EquipContainer pointer found!");
		}
		pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
		Assert(pEquipItem != NULL);
		if(pEquipItem->IsEmpty())
		{
			Result = EQUIPBIND_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler Equip empyty, bagpos=%d", pPacket->GetPos()) ;
			return PACKET_EXE_CONTINUE;
		}

		Assert (pEquipItem->GetItemClass() == ICLASS_EQUIP);
	}

	if(!(pEquipItem->GetGUID() == pPacket->getItemID()))
	{
		Result = EQUIPBIND_EQUIP_FAIL;
		ResultPacket.SetResult(Result);
		pGamePlayer->SendPacket(&ResultPacket);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler guid error") ;
		return PACKET_EXE_CONTINUE;
	}

	
	Result	= (EQUIPBIND_RESULT)pHuman->EquipBind(pEquipItem, pPacket->getGemBagPos());
	ResultPacket.SetResult(Result);
	pGamePlayer->SendPacket(&ResultPacket);

	//如果装备在身上
	if((pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)&&(EquipPoint != INVALID_ITEM_POS))
	{
		//设置装备版本
		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);

		//刷新装备对人物的属性影响
		pHuman->ItemEffectFlush();

		//更新装备
		UINT RetPart = 0;
		GCDetailEquipList	SelfMsg;
		SelfMsg.setObjID( pHuman->GetID() );
		pEquipItem = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)EquipPoint);
		if(!pEquipItem)
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		if(pEquipItem->IsEmpty()) 
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipBindHandler: Equip_Point =%d Fail", EquipPoint ) ;
			return PACKET_EXE_CONTINUE;
		}

		pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)EquipPoint));
		RetPart|=(1<<((UINT)EquipPoint));
		SelfMsg.SetAskPart(RetPart);
		pGamePlayer->SendPacket( &SelfMsg ) ;
	}
	else	//仅仅通知背包内物品改变
	{
		GCItemInfo  EquipMsg;
		EquipMsg.setIsNull(FALSE);
		EquipMsg.setID(pPacket->GetPos());
		pEquipItem->SaveValueTo(EquipMsg.getItem());
		pHuman->GetPlayer()->SendPacket(&EquipMsg);
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}