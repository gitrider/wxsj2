// [7/8/2010 陈军龙]
#include "stdafx.h"
#include "CGManipulateMount.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCManipulateMountResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

//extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGManipulateMountHandler::Execute(CGManipulateMount* pPacket, Player* pPlayer )
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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGManipulateMountHandler: change scene") ;
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

	Assert( NULL != pPacket ) ;

	BYTE bagIndex = pPacket->getBagIndex();
	BYTE mountIndex = pPacket->getMountIndex();
	INT  mountID = pPacket->getMountID();

	ItemContainer* pMountContainer = HumanItemLogic::GetMountContainer(pHuman);

	if (pMountContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGManipulateMountHandler BagIndex is invalid, BagIndex = %d", mountIndex) ;

		return PACKET_EXE_ERROR;
	}

    GCManipulateMountResult Msg;

	switch(pPacket->getMountOperator())
	{
		case CGManipulateMount::MANIPULATE_EQUIP_MOUNT:
			{
				ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, bagIndex);
				_ITEM* pSourceItem = pBagContainer->GetItem(bagIndex)->GetItemData();
				INT result = g_ItemOperator.MoveItem(pBagContainer, bagIndex, pMountContainer);

				if (ITEMOE_SUCCESS <= result)
				{
					MOUNT_INFO* mountInfo = pSourceItem->GetMountData();
                    INT elapseMinute = g_pTimeManager->DiffDWORDTime(g_pTimeManager->GetANSITime(), mountInfo->m_nObtainDate);
					const INT MINUTE_IN_ONE_DAY = 1488;

					mountInfo->m_nObtainDate = (UINT)g_pTimeManager->GetANSITime();
					mountInfo->m_nRemainTime = mountInfo->m_nLife * MINUTE_IN_ONE_DAY - elapseMinute;

					if(CGManipulateMount::EQUIP_BIND == (CGManipulateMount::MOUNT_BIND_TYPE) mountInfo->m_nBindType)
					    mountInfo->m_bIsBinded = TRUE;

					Msg.setResult(TRUE);
					Msg.setMountObtainDate(mountInfo->m_nObtainDate);
					Msg.setMountIsBinded(mountInfo->m_bIsBinded);
					Msg.setMountRemainTime(mountInfo->m_nRemainTime);
					Msg.setMountGUID(pSourceItem->m_ItemGUID);
				}
				else
				{
                    Msg.setResult(FALSE);  
				}

                Msg.setBagIndex(bagIndex);
				Msg.setMountIndex(result);
				Msg.setMountOperator(GCManipulateMountResult::MANIPULATE_EQUIP_MOUNT);
				pGamePlayer->SendPacket( &Msg ) ;
			}
			break;

		case CGManipulateMount::MANIPULATE_CALL_MOUNT:
			{
				INT addtionSpeed = pHuman->GetDB()->GetMountDB()->m_pItem[mountIndex].GetMountData()->m_nAddtionSpeed;
				pHuman->GetDB()->SetMountID(mountID);
				pHuman->GetDB()->SetMountIndex(mountIndex);
                pHuman->SetBaseMoveSpeed(pHuman->GetBaseMoveSpeed() + addtionSpeed);

				Msg.setMountIndex(mountIndex);
				Msg.setMountOperator(GCManipulateMountResult::MANIPULATE_CALL_MOUNT);
				Msg.setResult(TRUE);

				pGamePlayer->SendPacket( &Msg ) ;
			}
			break;

		case CGManipulateMount::MANIPULATE_RECALL_MOUNT:
			{
				INT addtionSpeed = pHuman->GetDB()->GetMountDB()->m_pItem[mountIndex].GetMountData()->m_nAddtionSpeed;
				pHuman->GetDB()->SetMountID(INVALID_ID);
				pHuman->GetDB()->SetMountIndex(INVALID_INDEX);
                pHuman->SetBaseMoveSpeed(pHuman->GetBaseMoveSpeed() - addtionSpeed);

				Msg.setMountIndex(mountIndex);
				Msg.setMountOperator(GCManipulateMountResult::MANIPULATE_RECALL_MOUNT);
				Msg.setResult(TRUE);

				pGamePlayer->SendPacket( &Msg ) ;
			}
			break;

		case CGManipulateMount::MANIPULATE_DISCARD_MOUNT:
			{
				pHuman->GetDB()->EraseMountItem(mountIndex);

				Msg.setMountIndex(mountIndex);
				Msg.setMountOperator(GCManipulateMountResult::MANIPULATE_DISCARD_MOUNT);
				Msg.setResult(TRUE);

				pGamePlayer->SendPacket( &Msg ) ;
			}
			break;

		case CGManipulateMount::MANIPULATE_DESTROY_MOUNT:
			{
				if (INVALID_ID != pHuman->GetDB()->GetMountID())
				{
					INT addtionSpeed = pHuman->GetDB()->GetMountDB()->m_pItem[mountIndex].GetMountData()->m_nAddtionSpeed;
					pHuman->GetDB()->SetMountID(INVALID_ID);
					pHuman->GetDB()->SetMountIndex(INVALID_INDEX);
					pHuman->SetBaseMoveSpeed(pHuman->GetBaseMoveSpeed() - addtionSpeed);
				}

				pHuman->GetDB()->EraseMountItem(mountIndex);

				Msg.setMountIndex(mountIndex);
				Msg.setMountOperator(GCManipulateMountResult::MANIPULATE_DESTROY_MOUNT);
				Msg.setResult(TRUE);

				pGamePlayer->SendPacket( &Msg ) ;
			}
			break;

		default:break;
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}