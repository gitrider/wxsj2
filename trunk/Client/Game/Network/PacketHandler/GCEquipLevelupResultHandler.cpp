
#include "StdAfx.h"
#include "GCEquipLevelupResult.h"
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



uint GCEquipLevelupResultHandler::Execute(GCEquipLevelupResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPLEVEUP_RESULT ret = (EQUIPLEVEUP_RESULT)pPacket->GetResult();

			switch(ret)
			{
			case EQUIPLEVEUP_SUCCESS:
				{

					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,1);
					CGameProcedure::s_pGfxSystem->PushDebugString("装备升星成功");
					STRING strTemp = "装备升星成功";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

					return PACKET_EXE_CONTINUE ;

				}
				break;

			case EQUIPLEVEUP_COMMONEQUIP:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("普通装备不能升星");
					STRING strTemp = "普通装备不能升星";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;



			case EQUIPLEVEUP_FAIL_SUCCESS:
				{

					//操作失败音效
					//CSoundSystemFMod::_PlayUISoundFunc(47);
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("装备升星失败");
					STRING strTemp = "装备升星失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case EQUIPLEVEUP_EQUIP_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("装备不存在");

					STRING strTemp = "装备不存在";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			case EQUIPLEVEUP_ITEMTYPE_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("宝石类型不对");

					STRING strTemp = "宝石类型不对";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_ITEMNUM_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("您没有足够的升级宝石");

					STRING strTemp = "您没有足够的升级宝石 ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_FULL_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("装备已经达到最大星级");

					STRING strTemp = "装备已经达到最大星级";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case EQUIPLEVEUP_MONEY_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("升星所需金钱不足");

					STRING strTemp = "升星所需金钱不足";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("装备升星失败");

					STRING strTemp = "装备升星失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
		

			default:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("Equip LEVELUP UNKNOWN ERROR");
					STRING strTemp = "装备升星未知错误";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}
 
		 


			////解除升星物品的锁定
			//INT nTypeOwner	= CDataPool::GetMe()->AddStar_GetItemConta();
			//INT nItemPos	= CDataPool::GetMe()->AddStar_GetItemPos();

			//switch( nTypeOwner ) 
			//{
			//case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
			//	{
			//		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			//		if(pItem)
			//		{
			//			pItem->SetLock(FALSE);
			//		}


			//		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			//	}
			//	break;
			//case tObject_Item::IO_MYSELF_EQUIP:		// 玩家的装备
			//	{
			//		tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nItemPos));

			//		if(pItem)
			//		{
			//			pItem->SetLock(FALSE);
			//			CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
			//		}

			//	}
			//	break;
			//default:
			//	break;
			//}

		 //
			//CDataPool::GetMe()->AddStar_CleanUP();

	  }

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}