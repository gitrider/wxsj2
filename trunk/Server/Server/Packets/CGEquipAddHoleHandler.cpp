#include "stdafx.h"
#include "CGEquipAddHole.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCEquipAddHoleResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"

using namespace Packets;

uint CGEquipAddHoleHandler::Execute(CGEquipAddHole* pPacket, Player* pPlayer )
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

	//切场景时丢弃换装消息
	if(pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL ||
		!pHuman->IsActiveObj() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: change scene") ;
		return PACKET_EXE_CONTINUE;
	}

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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*		pEquipItem		= NULL;
	UINT		EquipPoint		= INVALID_ITEM_POS; 
	UINT		BagIndex		= INVALID_ITEM_POS;
	INT			nResult			= ADDHOLE_FAIL;
	GCEquipAddHoleResult Msg;
	switch(pPacket->getEquipFromType())
	{
	case ENUM_EQUIP_POINT:		//来自装备点数据
		{
			EquipPoint = pPacket->getEquipFromPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
				return PACKET_EXE_ERROR;
			}
			ItemContainer* pEquipContainer = pHuman->GetEquipContain();
			if(NULL==pEquipContainer)
			{
				AssertEx(FALSE,"[CGUseGem]: NULL EquipContainer pointer found!");
			}
			pEquipItem = pEquipContainer->GetItem((UINT)EquipPoint);
			Assert(pEquipItem);

			if(pEquipItem->IsEmpty())
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler pEquipItem is Null at  EquipPoint= %d", EquipPoint) ;
				return PACKET_EXE_CONTINUE;
			}
			
			Assert (pEquipItem->GetItemClass() == ICLASS_EQUIP);

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler GUID is different ") ;
				return PACKET_EXE_ERROR;
			}
		}
		break;
	case ENUM_BAG:			//来自背包数据
		{
			BagIndex = pPacket->getEquipFromPos();
			ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
			if( pItemContainer == NULL )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: BagIndex error, BagIndex=%d", BagIndex) ;
				return PACKET_EXE_ERROR;
			}
			if (!pItemContainer->IsCanUse())
			{
				nResult = ADDHOLE_BAG_INVALID;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d",
					pItemContainer->GetContainerType(), BagIndex) ;
				return PACKET_EXE_CONTINUE;
			}

			pEquipItem = pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			if(pEquipItem->IsEmpty())
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler pEquipItem is Null at  BagIndex= %d", BagIndex) ;
				return PACKET_EXE_ERROR;
			}

			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler pEquipItem is NOT EquipType") ;
				return PACKET_EXE_ERROR;
			}

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler GUID is different ") ;
				return PACKET_EXE_ERROR;
			}

			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				nResult = ADDHOLE_EQUIP_FAIL;
				Msg.setResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler GUID is different ") ;
				return PACKET_EXE_ERROR;
			}
		}
	    break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: EquipFromType error, EquipFromType=%d", pPacket->getEquipFromType()) ;
			return PACKET_EXE_ERROR;
		}
	    break;
	}

	INT nCurHoleNum = pEquipItem->GetCurGemHole();
	if(nCurHoleNum >=MAX_GEM_HOLE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::AddHole Hole full ") ;
		nResult = ADDHOLE_FULL_FAIL;
	}
	
	nResult = pHuman->AddHole(pEquipItem);

	if(nResult == ADDHOLE_FAIL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHoleHandler: pHuman->AddHole Fail") ;
		return PACKET_EXE_ERROR;
	}

	
	Msg.setResult(nResult);
	pGamePlayer->SendPacket( &Msg ) ;
	//////////////////////////////////////////////////////////////////////////
	
	if(nResult == ADDHOLE_SUCCESS)
	{
		if(pPacket->getEquipFromType() == ENUM_EQUIP_POINT)
		{
			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			UINT RetPart = 0;
			GCDetailEquipList	SelfMsg;
			SelfMsg.setObjID( pHuman->GetID() );
			pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)EquipPoint));
			RetPart|=(1<<((UINT)EquipPoint));
			SelfMsg.SetAskPart(RetPart);
			pGamePlayer->SendPacket( &SelfMsg ) ;
		}
		else	//仅仅通知背包内物品改变
		{
			GCItemInfo  EquipMsg;
			EquipMsg.setIsNull(FALSE);
			EquipMsg.setID(BagIndex);
			pEquipItem->SaveValueTo(EquipMsg.getItem());
			pHuman->GetPlayer()->SendPacket(&EquipMsg);
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipAddHole: success result=%d", nResult ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}