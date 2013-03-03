
#include "StdAfx.h"
#include "GCShopSpecialRepairResult.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Action\GMActionSystem.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Sound\GMSoundSystem.h"
 

using namespace Packets;


 

uint GCShopSpecialRepairResultHandler::Execute(GCShopSpecialRepairResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPSPECIALREPAIR_RESULT ret = (EQUIPSPECIALREPAIR_RESULT)pPacket->GetResult();

			switch(ret)
			{
			case EQUIPSPECIALREPAIR_SUCCESS:
				{

					CEventSystem::GetMe()->PushEvent(GE_OPEN_SP_REPAIR_SUCCESS);

					//通知tooltip更新
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);

				}
				break;

			case EQUIPSPECIALREPAIR_NOREAPIR:
				{

					                      
					CGameProcedure::s_pGfxSystem->PushDebugString("该装备不需要特修");
					STRING strTemp = "该装备不需要特修";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case EQUIPSPECIALREPAIR_ITEM:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("修理材料不存在");

					STRING strTemp = "修理材料不存在";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			case EQUIPSPECIALREPAIR_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("特修不成功");

					STRING strTemp = "特修不成功";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			default:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("Equip SPREPAIR UNKNOWN ERROR");
				}
				break;
			}
			 
			//解除特修物品的锁定
			INT nTypeOwner	= CDataPool::GetMe()->SpRepair_GetItemConta();
			INT nItemPos	= CDataPool::GetMe()->SpRepair_GetItemPos();

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
					((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[0].nCurDur);
					((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[0].nCurMaxDur);
					
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:		// 玩家的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nItemPos));

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
					((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[0].nCurDur);
					((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[0].nCurMaxDur);
				}
				break;
			default:
				break;
			}


			CDataPool::GetMe()->SpRepair_CleanUP();

		}

		return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}