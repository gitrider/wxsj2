#include "stdafx.h"
#include "CGEquipLevelup.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "GCEquipLevelupResult.h"
#include "GCCharEquipment.h"
#include "GCDetailEquipList.h"
#include "GCItemInfo.h"
using namespace Packets;

uint CGEquipLevelupHandler::Execute(CGEquipLevelup* pPacket,Player* pPlayer)
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

	Item*		pEquipItem		= NULL;
	UINT		EquipPoint		= INVALID_ITEM_POS;
	UINT		BagIndex		= INVALID_ITEM_POS;  
	EQUIPLEVEUP_RESULT nResult	= EQUIPLEVEUP_FAIL;
	GCEquipLevelupResult Msg;
	switch(pPacket->GetEquipPosType())
	{
	case ENUM_EQUIP_POINT:
		{
			EquipPoint = pPacket->GetPos();
			if(EquipPoint>HEQUIP_ADORN2)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: EquipPoint error, EquipPoint=%d", EquipPoint) ;
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
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler pEquipItem is Null at  EquipPoint= %d", EquipPoint) ;
				return PACKET_EXE_CONTINUE;
			}
			
			Assert (pEquipItem->GetItemClass() == ICLASS_EQUIP);

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}


		}
		break;
	case ENUM_BAG:
		{
			BagIndex = pPacket->GetPos();
			ItemContainer* pItemContainer = HumanItemLogic::GetBagContainer(pHuman,BagIndex);
			if( pItemContainer == NULL )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: BagIndex error, BagIndex=%d", BagIndex) ;
				return PACKET_EXE_ERROR;
			}
			if (!pItemContainer->IsCanUse())
			{
				nResult = EQUIPLEVEUP_BAG_INVALID;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: ItemContainer is invalid, ContainerType=%d, BagIndex=%d", 
					pItemContainer->GetContainerType(), BagIndex) ;
					return PACKET_EXE_ERROR;
			}

			pEquipItem		=	pItemContainer->GetItem(pItemContainer->BagIndex2ConIndex(BagIndex));
			if(pEquipItem->IsEmpty())
			{
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup pEquipItem is Null at  BagIndex= %d", BagIndex) ;
				return PACKET_EXE_CONTINUE;
			}
			
			if (pEquipItem->GetItemClass() != ICLASS_EQUIP)
			{
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "EquipLevelup GetItemClass is not ICLASS_EQUIP ") ;
				return PACKET_EXE_CONTINUE;
			}

			if(!(pPacket->getItemID() == pEquipItem->GetGUID()))
			{
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}

			if (pEquipItem->GetItemType() > EQUIP_ADORN)
			{
				nResult = EQUIPLEVEUP_EQUIP_FAIL;
				Msg.SetResult(nResult);
				pGamePlayer->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup GUID is different ") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	default:
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: EquipFromType error, EquipFromType=%d", pPacket->GetEquipPosType()) ;
			return PACKET_EXE_ERROR;
		}
		break;
	}

	INT nCurLevel = pEquipItem->GetCurLevel();
	if(nCurLevel >=MAX_EQUIP_LEVELUP_NUM)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler LEVEL full ") ;
		nResult = EQUIPLEVEUP_FULL_FAIL;
		Msg.SetResult(nResult);
		pGamePlayer->SendPacket( &Msg ) ;
		return PACKET_EXE_CONTINUE;
	}


	nResult = (EQUIPLEVEUP_RESULT)pHuman->EquipLevelup(pEquipItem, pPacket->getGemID(), pPacket->GetGemNum());

	if(nResult == EQUIPLEVEUP_FAIL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: pHuman->AddHole Fail") ;
		return PACKET_EXE_ERROR;
	}

	Msg.SetResult(nResult);
	pGamePlayer->SendPacket( &Msg ) ;
	//////////////////////////////////////////////////////////////////////////

	if((nResult == EQUIPLEVEUP_SUCCESS)||(nResult == EQUIPLEVEUP_FAIL_SUCCESS))
	{
		if(pPacket->GetEquipPosType() == ENUM_EQUIP_POINT)
		{
			pHuman->ItemEffectFlush();

			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			INT Equip_Point = pPacket->GetPos();
			UINT RetPart = 0;
			GCDetailEquipList	SelfMsg;
			SelfMsg.setObjID( pHuman->GetID() );
			Item* pEquipItem = HumanItemLogic::GetEquip(pHuman, (HUMAN_EQUIP)Equip_Point);
			if(!pEquipItem)
			{
				Assert(FALSE);
			}
			if(pEquipItem->IsEmpty()) 
			{
				Assert(FALSE);
			}
			pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)Equip_Point));
			RetPart|=(1<<((UINT)Equip_Point));
			SelfMsg.SetAskPart(RetPart);
			pGamePlayer->SendPacket( &SelfMsg ) ;
		}
		else
		{
			GCItemInfo	EquipMsg;
			EquipMsg.setIsNull(FALSE);
			EquipMsg.setID(BagIndex);
			pEquipItem->SaveValueTo(EquipMsg.getItem());
			pHuman->GetPlayer()->SendPacket(&EquipMsg);
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGEquipLevelupHandler: success nResult=%d", nResult ) ;
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}