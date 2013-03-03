
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Event/GMEventSystem.h"

#include "NetWork/NetManager.h"
#include "CGShopBuy.h"
#include "CGShopSell.h"

#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Action/GMActionSystem_MouseCmd.h"
#include "DataPool/GMUIDataPool.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	NpcShop NpcShop::s_NpcShop;
	LuaPlus::LuaObject* NpcShop::s_pMetaTable = NULL;

	IBShop IBShop::s_IBShop;
	LuaPlus::LuaObject* IBShop::s_pMetaTable = NULL;

	//关闭
	INT NpcShop::Close(LuaPlus::LuaState* state)
	{
		//在点击Quest的"再见"的时候，关闭NpcShop
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_BOOTH);

		return 0;
	}
	//获得商店NpcID
	INT NpcShop::GetNpcId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetShopNpcId());

		return 1;
	}

	//获得商店物品价格
	INT NpcShop::GetItemPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:GetItemPrice");
		}

		INT nIndex = args[3].GetInteger();

		tObject_Item* pItem;

		// 20100407 ModifyCodeBegin
		//tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		if ( nIndex < 200 )
		{
			pItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		}
		else if ( nIndex >= 200 )
		{
			pItem = CDataPool::GetMe()->Booth_GetSoldItem(nIndex - 200 );
		}
		// 20100407 ModifyCodeEnd

		if (pItem)
		{
			// 20100407 ModifyCodeBegin
			//INT nPrice = pItem->GetItemPrice();
			INT nPrice;	
			if ( nIndex < 200 )
			{
						
				nPrice = CDataPool::GetMe()->Booth_GetItemPrice( nIndex );		
			}
			else if ( nIndex >= 200 )
			{
				nPrice = pItem->GetItemPrice();
			}	
			// 20100407 ModifyCodeEnd

			state->PushInteger( nPrice );
		}
		else
		{
			state->PushInteger( 100 );
		}

		return 1;
	}

	INT NpcShop::GetItemPriceType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		state->PushInteger( 0 );

		return 1;
	}

	

	INT NpcShop::NpcShopBuy(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:NpcShopBuy Param 1 Wrong");
		}
		INT nIndex = args[2].GetInteger();

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:NpcShopBuy Param 2 Wrong");
		}
		INT nNumber = args[3].GetInteger();

		//tObject_Item* pItem = CDataPool::GetMe()->CDataPool::GetMe()->Booth_GetSoldItem(i);(nIndex);
		//tActionItem* pItem = CActionSystem::GetMe()->EnumAction( nIndex, "boothitem" );


		// 20100406 modifyCodeBegin
		// tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		// if( !pItem ) return 0;
		tObject_Item* pItem;

		if ( nIndex < 200 )
		{
			pItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
			if( !pItem ) return 0;
		}
		else if ( nIndex >= 200 )
		{
			pItem = CDataPool::GetMe()->Booth_GetSoldItem( nIndex - 200 );
			if( !pItem ) return 0;
		}
		else
			return 0;
		// 20100406 modifyCodeEnd


		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		{
			//取消修理
			CGameProcedure::s_pGameInterface->Skill_CancelAction();
		}
	

		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice() )
		{
			CGShopBuy msg;
			msg.SetIndex(nIndex);
			msg.SetCount(nNumber);
			msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
			CNetManager::GetMe()->SendPacket( &msg );
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
		}

		return 0;
	}

	INT NpcShop::NpcShopSell(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:NpcShopSell Param 1 Wrong");
		}
		INT nIndex = args[2].GetInteger();

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:NpcShopSell Param 2 Wrong");
		}
		INT nNumber = args[3].GetInteger();

		//tObject_Item* pItem = CDataPool::GetMe()->ItemBox_GetItem(nIndex);
		//if( pItem == NULL )
		//	return 0;
		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		//{
		//	//是否在修理状态
		//	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		//	{
		//		//取消修理
		//		CGameProcedure::s_pGameInterface->Skill_CancelAction();
		//	}
		//	//执行销售
		//	else
		//	{
		//		////任务物品不可以卖
		//		if( pItem->GetItemClass() != ICLASS_TASKITEM )//pItem->GetPosIndex() < TASK_CONTAINER_OFFSET )
		//		{
		//			if(!pItem->Rule(tObject_Item::RULE_SALE))
		//			{
		//				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_SALE).c_str());
		//				return 0;
		//			}

		//			//判断当前的NPC是不是收购这类物品
		//			if( !CDataPool::GetMe()->Booth_IsCanBuy(pItem) )
		//			{
		//				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "我不收购这个物品");
		//				return 0;
		//			}

		//			CGShopSell msg;
		//			msg.SetCount(nNumber);
		//			msg.SetBagIndex( nIndex );
		//			msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
		//			CNetManager::GetMe()->SendPacket( &msg );
		//		}
		//		//else
		//		//{
		//		//	//任务物品不能卖的信息提示
		//		//	if(bLog)
		//		//	{
		//		//		STRING strTemp = NOCOLORMSGFUNC("stall_sold_failure");
		//		//		ADDNEWDEBUGMSG(strTemp);
		//		//	}
		//		//}
		//	}
		//}

		CGShopSell msg;
		msg.SetCount(nNumber);
		msg.SetBagIndex( nIndex );
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
		CNetManager::GetMe()->SendPacket( &msg );

		return 0;
	}

	//获得商城物品价格
	INT IBShop::Lua_GetIBItemPrice(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetItemPrice");
		}

		INT nIndex = args[2].GetInteger();

		tObject_Item* pItem;

		pItem = CDataPool::GetMe()->IBShop_GetItem(nIndex);

		if (pItem)
		{
			INT nPrice;	
			nPrice = CDataPool::GetMe()->IBShop_GetItemPrice( nIndex );		
			state->PushInteger( nPrice );
		}
		else
		{
			state->PushInteger( 100 );
		}

		return 1;
	}
	//end of add

};


