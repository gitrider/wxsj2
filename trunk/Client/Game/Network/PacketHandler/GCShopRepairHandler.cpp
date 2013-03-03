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
			//�Ѿ������˾�����
			if(CDataPool::GetMe()->Booth_IsClose())
				return PACKET_EXE_CONTINUE ;

			//��Ʒ����
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

						//ˢ�±���
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, ((CObject_Item_Equip*)pItem)->GetPosIndex());
						//֪ͨtooltip����
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::EQUIP:
						pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);
						//֪ͨtooltip����
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::BOX:
						break;
					default:
						break;
					}
				}
				//֪ͨtooltip����
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
			{//�ô��ӵ��һ��һ���ޣ����������ϵ�Ҳ�����Ǳ������
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

						//ˢ�±���
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, ((CObject_Item_Equip*)pItem)->GetPosIndex());
						//֪ͨtooltip����
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
						bMsg = TRUE;
						break;
					case GCShopRepair::EQUIP:
						pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)itemindex);
						if(!pItem)
							continue;
						((CObject_Item_Equip*)pItem)->SetDur(pPacket->GetReList()[i].nCurDur);
						((CObject_Item_Equip*)pItem)->SetMaxDur(pPacket->GetReList()[i].nCurMaxDur);
						//֪ͨtooltip����
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
