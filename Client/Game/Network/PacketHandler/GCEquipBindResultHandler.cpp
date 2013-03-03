
#include "StdAfx.h"
#include "GCEquipBindResult.h"
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
 

 

uint GCEquipBindResultHandler::Execute(GCEquipBindResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPBIND_RESULT ret = (EQUIPBIND_RESULT)pPacket->GetResult();

			switch(ret)
			{
			case EQUIPBIND_SUCCESS:
				{

					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,1);// 1表示成功 0失败

				}
				break;

			case EQUIPBIND_EQUIP_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("绑定装备失败");
					STRING strTemp = "绑定装备失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}

			break;
			case EQUIPBIND_BALIVE:
				{
				
					CGameProcedure::s_pGfxSystem->PushDebugString("装备已经绑定");
					STRING strTemp = "装备已经绑定";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
			break;
			case EQUIPBIND_ITEM_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("绑定道具不存在");

					STRING strTemp = "绑定道具不存在";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
				break;


			case EQUIPBIND_FULL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("装备达到绑定上限");

					STRING strTemp = "装备达到绑定上限";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
				break;


			case EQUIPBIND_MONEY_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("玩家金钱不足");

					STRING strTemp = "玩家金钱不足";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
				break;

			case EQUIPBIND_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("绑定失败");

					STRING strTemp = "绑定失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
				break;

			default:
				{

					STRING strTemp = "绑定未知错误";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1表示成功 0失败
				}
				break;
			}
 


			//解除被绑定物品的锁定
			INT nTypeOwner	= CDataPool::GetMe()->Bind_GetItemConta();
			INT nItemPos	= CDataPool::GetMe()->Bind_GetItemPos();

			INT  nITEM =  CDataPool::GetMe()->Bind_GetNeedItemPos();
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
			case tObject_Item::IO_MYSELF_EQUIP:		// 玩家的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nItemPos));

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
	

			tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

			if(pItemNeed)
			{
				pItemNeed->SetLock(FALSE);
			}


			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


			CDataPool::GetMe()->Bind_CleanUP();
	 

		}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}