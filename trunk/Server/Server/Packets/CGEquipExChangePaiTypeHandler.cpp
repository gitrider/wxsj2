#include "stdafx.h"
#include "CGEquipExChangePaiType.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCEquipExChangePaiTypeResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGEquipExChangePaiTypeHandler::Execute(CGEquipExChangePaiType* pPacket,Player* pPlayer)
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

	//摆摊不可操作
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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}


	EQUIPEXCHANGEPAITTYPE_RESULT	Result = EQUIPEXCHANGEPAITTYPE_FAIL;
	GCEquipExChangePaiTypeResult  ResultPacket;

	Item*		pEquip1Item		= NULL;
	Item*		pEquip2Item		= NULL;
	INT			Equip1Point		= INVALID_ITEM_POS;
	INT			Equip2Point		= INVALID_ITEM_POS;
	if(pPacket->GetEquip1PosType()>=INVALID_ITEM_POS_TYPE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip posType error, posType=%d", pPacket->GetEquip1PosType()) ;
		return PACKET_EXE_ERROR;
	}

	if(pPacket->GetEquip1PosType() == ENUM_BAG)
	{
		ItemContainer* pItemContainer1 = HumanItemLogic::GetBagContainer(pHuman,pPacket->GetPos1());
		if( pItemContainer1 == NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler: BagIndex error, BagIndex=%d", pPacket->GetPos1()) ;
			return PACKET_EXE_ERROR;
		}
		if (!pItemContainer1->IsCanUse())
		{
			Result = EQUIPEXCHANGEPAITTYPE_BAG_INVALID;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d", 
				pItemContainer1->GetContainerType(), pPacket->GetPos1()) ;
				return PACKET_EXE_ERROR;
		}

		pEquip1Item		=	pItemContainer1->GetItem(pItemContainer1->BagIndex2ConIndex(pPacket->GetPos1()));
		if(pEquip1Item->IsEmpty())
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip empyty, bagpos=%d", pPacket->GetPos1()) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pEquip1Item->GetItemClass()!=ICLASS_EQUIP)
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}

		if (pEquip1Item->GetItemType() > EQUIP_ADORN)
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}
	}

	if(pPacket->GetEquip1PosType() == ENUM_EQUIP_POINT)
	{
		if(pPacket->GetPos1()>HEQUIP_ADORN2)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Item pos error") ;
			return PACKET_EXE_ERROR;
		}
		Equip1Point = pPacket->GetPos1();
		ItemContainer* pEquipContainer = pHuman->GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[CGEquipExChangePaiTypeHandler]: NULL EquipContainer pointer found!");
		}
		pEquip1Item = pEquipContainer->GetItem((UINT)Equip1Point);
		Assert(pEquip1Item != NULL);
		if(pEquip1Item->IsEmpty())
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip empyty, bagpos=%d", pPacket->GetPos1()) ;
			return PACKET_EXE_CONTINUE;
		}

		Assert (pEquip1Item->GetItemClass() == ICLASS_EQUIP);
	}

	if(!(pEquip1Item->GetGUID() == pPacket->getItem1ID()))
	{
		Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
		ResultPacket.SetResult(Result);
		pGamePlayer->SendPacket(&ResultPacket);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler guid error") ;
		return PACKET_EXE_CONTINUE;
	}
	
	if(pPacket->GetEquip2PosType()>=INVALID_ITEM_POS_TYPE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip posType error, posType=%d", pPacket->GetEquip1PosType()) ;
		return PACKET_EXE_ERROR;
	}

	if(pPacket->GetEquip2PosType() == ENUM_BAG)
	{
		ItemContainer* pItemContainer2 = HumanItemLogic::GetBagContainer(pHuman,pPacket->GetPos2());
		if( pItemContainer2 == NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: BagIndex error, BagIndex=%d", pPacket->GetPos2()) ;
			return PACKET_EXE_ERROR;
		}
		if (!pItemContainer2->IsCanUse())
		{
			Result = EQUIPEXCHANGEPAITTYPE_BAG_INVALID;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);;
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipDecompoundHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d", 
				pItemContainer2->GetContainerType(), pPacket->GetPos2()) ;
				return PACKET_EXE_ERROR;
		}

		pEquip2Item		=	pItemContainer2->GetItem(pItemContainer2->BagIndex2ConIndex(pPacket->GetPos2()));
		if(pEquip2Item->IsEmpty())
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip empyty, bagpos=%d", pPacket->GetPos2()) ;
			return PACKET_EXE_CONTINUE;
		}

		if(pEquip2Item->GetItemClass()!=ICLASS_EQUIP)
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}

		if (pEquip2Item->GetItemType() > EQUIP_ADORN)
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler is not ICLASS_EQUIP") ;
			return PACKET_EXE_CONTINUE;
		}
	}

	if(pPacket->GetEquip2PosType() == ENUM_EQUIP_POINT)
	{
		if(pPacket->GetPos2()>HEQUIP_ADORN2)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Item pos error") ;
			return PACKET_EXE_ERROR;
		}
		Equip2Point = pPacket->GetPos2();
		ItemContainer* pEquipContainer = pHuman->GetEquipContain();
		if(NULL==pEquipContainer)
		{
			AssertEx(FALSE,"[CGEquipExChangePaiTypeHandler]: NULL EquipContainer pointer found!");
		}
		pEquip2Item = pEquipContainer->GetItem((UINT)Equip2Point);
		Assert(pEquip2Item != NULL);
		if(pEquip2Item->IsEmpty())
		{
			Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
			ResultPacket.SetResult(Result);
			pGamePlayer->SendPacket(&ResultPacket);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler Equip empyty, bagpos=%d", pPacket->GetPos2()) ;
			return PACKET_EXE_CONTINUE;
		}

		Assert (pEquip2Item->GetItemClass() == ICLASS_EQUIP);
	}

	if(!(pEquip2Item->GetGUID() == pPacket->getItem2ID()))
	{
		Result = EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL;
		ResultPacket.SetResult(Result);
		pGamePlayer->SendPacket(&ResultPacket);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler guid error") ;
		return PACKET_EXE_CONTINUE;
	}
	
	if ((!pEquip1Item->GetItemBind())||(!pEquip2Item->GetItemBind()))
	{
		Result = EQUIPEXCHANGEPAITTYPE_EQUIPBIND_FAIL;
		ResultPacket.SetResult(Result);
		pGamePlayer->SendPacket(&ResultPacket);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler no Bind") ;
		return PACKET_EXE_CONTINUE;
	}

	Result	= (EQUIPEXCHANGEPAITTYPE_RESULT)pHuman->EquipChangePai(pEquip1Item, pEquip2Item, pPacket->getGemBagPos());
	ResultPacket.SetResult(Result);
	pGamePlayer->SendPacket(&ResultPacket);

	//如果装备在身上
	if(((pPacket->GetEquip1PosType() == ENUM_EQUIP_POINT)&&(Equip1Point != INVALID_ITEM_POS))
		||((pPacket->GetEquip2PosType() == ENUM_EQUIP_POINT)&&(Equip2Point != INVALID_ITEM_POS)))
	{
		//设置装备版本
		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);

		//刷新装备对人物的属性影响
		pHuman->ItemEffectFlush();

		//更新装备
		UINT RetPart = 0;
		GCDetailEquipList	SelfMsg;
		SelfMsg.setObjID( pHuman->GetID() );
		if (pPacket->GetEquip1PosType() == ENUM_EQUIP_POINT)
		{
			pEquip1Item = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)Equip1Point);
			if(!pEquip1Item)
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}
			if(pEquip1Item->IsEmpty()) 
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler: Equip_Point =%d Fail", Equip1Point ) ;
				return PACKET_EXE_CONTINUE;
			}

			pEquip1Item->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)Equip1Point));
			RetPart|=(1<<((UINT)Equip1Point));
		}
		
		if (pPacket->GetEquip2PosType() == ENUM_EQUIP_POINT)
		{
			pEquip2Item = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)Equip2Point);
			if(!pEquip2Item)
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}
			if(pEquip2Item->IsEmpty()) 
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipExChangePaiTypeHandler: Equip_Point =%d Fail", Equip2Point ) ;
				return PACKET_EXE_CONTINUE;
			}

			pEquip2Item->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)Equip2Point));
			RetPart|=(1<<((UINT)Equip2Point));
		}
		SelfMsg.SetAskPart(RetPart);
		pGamePlayer->SendPacket( &SelfMsg ) ;

		if (pPacket->GetEquip1PosType() != ENUM_EQUIP_POINT)
		{
			GCItemInfo  EquipMsg;
			EquipMsg.setIsNull(FALSE);
			EquipMsg.setID(pPacket->GetPos1());
			pEquip1Item->SaveValueTo(EquipMsg.getItem());
			pHuman->GetPlayer()->SendPacket(&EquipMsg);
		}

		if (pPacket->GetEquip2PosType() != ENUM_EQUIP_POINT)
		{
			GCItemInfo	EquipMsg;
			EquipMsg.setIsNull(FALSE);
			EquipMsg.setID(pPacket->GetPos2());
			pEquip2Item->SaveValueTo(EquipMsg.getItem());
			pHuman->GetPlayer()->SendPacket(&EquipMsg);
		}
	}

	else	//仅仅通知背包内物品改变
	{
		GCItemInfo  EquipMsg;
		EquipMsg.setIsNull(FALSE);
		EquipMsg.setID(pPacket->GetPos1());
		pEquip1Item->SaveValueTo(EquipMsg.getItem());
		pHuman->GetPlayer()->SendPacket(&EquipMsg);

		EquipMsg.setIsNull(FALSE);
		EquipMsg.setID(pPacket->GetPos2());
		pEquip2Item->SaveValueTo(EquipMsg.getItem());
		pHuman->GetPlayer()->SendPacket(&EquipMsg);
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}