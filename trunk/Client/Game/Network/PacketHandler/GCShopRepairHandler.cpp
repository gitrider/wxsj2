// GCShopRepairHandler.cpp
#include "stdafx.h"
#include "GCShopRepair.h"
#include "GIUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopRepairHandler::Execute( GCShopRepair* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
			//已经关上了就算了
			if(CDataPool::GetMe()->Booth_IsClose())
				return PACKET_EXE_CONTINUE ;

			//商品个数
			BOOL	RepairAll = (BOOL)pPacket->GetAll();
			if(RepairAll)
			{
				INT nNum = pPacket->GetReListNum();
				tObject_Item* pItem;

				for(INT i=0; i<nNum; i++)
				{
					INT itemindex = pPacket->GetReList()[i].BagIndex;
					BOOL bMsg = FALSE;

					switch(pPacket->GetReList()[i].IsIn)
					{
					case GCShopRepair::BAG:
						pItem = CDataPool::GetMe()->UserBag_GetItem(itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);

						//刷新背包
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, ((CObject_Item_Equip*)pItem)->GetPosIndex());
						//通知tooltip更新
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::EQUIP:
						pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);
						//通知tooltip更新
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::BOX:
						break;
					default:
						break;
					}
				}
				//通知tooltip更新
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);

				if(bLog)
				{
					STRING strTemp = COLORMSGFUNC("repair_all_ok");
					ADDTALKMSG(strTemp);
					strTemp = NOCOLORMSGFUNC("repair_ok");
					ADDNEWDEBUGMSG(strTemp);
				}
			}
			else
			{//用锤子点的一个一个修，可以是身上的也可以是背包里的
				INT nNum = pPacket->GetReListNum();
				tObject_Item* pItem;
				
				for(INT i=0; i<nNum; i++)
				{
					INT itemindex = pPacket->GetReList()[i].BagIndex;
					BOOL bMsg = FALSE;

					switch(pPacket->GetReList()[i].IsIn)
					{
					case GCShopRepair::BAG:
						pItem = CDataPool::GetMe()->UserBag_GetItem(itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);

						//刷新背包
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, ((CObject_Item_Equip*)pItem)->GetPosIndex());
						//通知tooltip更新
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::EQUIP:
						pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);
						//通知tooltip更新
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::BOX:
						break;
					default:
						break;
					}

					if(bLog && pItem)
					{
						STRING strTemp = COLORMSGFUNC("repair_item", pItem->GetName());
						ADDTALKMSG(strTemp);
						strTemp = NOCOLORMSGFUNC("repair_ok");
						ADDNEWDEBUGMSG(strTemp);
					}
				}
			}
			
			CSoundSystemFMod::_PlayUISoundFunc(64);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
