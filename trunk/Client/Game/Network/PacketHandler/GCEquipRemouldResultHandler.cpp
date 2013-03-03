
#include "StdAfx.h"
#include "GCEquipRemouldResult.h"
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

 
uint GCEquipRemouldResultHandler::Execute(GCEquipRemouldResult * pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPREMOULD_RESULT ret = (EQUIPREMOULD_RESULT)pPacket->GetResult();
			switch(ret)
			{

			case EQUIPREMOULD_SUCCESS:
				{		 
					CGameProcedure::s_pGfxSystem->PushDebugString("装备改造成功");

					STRING strTemp = "装备改造成功" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
                    
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );
				}
				break;

			case EQUIPREMOULD_EQUIP_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("装备不存在");

					STRING strTemp = "装备不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;


			case EQUIPREMOULD_MAX_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("装备改造达到最大上限");

					STRING strTemp = "装备改造达到最大上限" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );


				}
				break;

			case EQUIPREMOULD_MATERIAL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("改造材料不存在");

					STRING strTemp = "改造材料不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			case EQUIPREMOULD_MATERIAL_LEVEL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("改造材料等级不对");

					STRING strTemp = "改造材料等级不对" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			case EQUIPREMOULD_MATERIAL_COUNT:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("改造材料数量不对");

					STRING strTemp = "改造材料数量不对" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
 
			case EQUIPREMOULD_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("装备改造失败");

					STRING strTemp = "装备改造失败" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			 

			default:
				{


				}


			}

			INT nTypeOwner	=	CDataPool::GetMe()->Rework_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->Rework_GetItemPos();

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;

			case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
					}
				}
				break;
			default:
				break;
			}
			CDataPool::GetMe()->Rework_CleanUP(); 



		}
	

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}