#include "StdAfx.h"
#include "GCPickResult.h"
#include "GIUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "GIException.h"

using namespace Packets;
uint GCPickResultHandler::Execute(GCPickResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		tUIDataPool* pDataPool = CGameProcedure::s_pUIDataPool;
		//if( pPacket->getItemBoxId() != pDataPool->GetItemBoxID() )
		
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		// 放置每一个物品
		if( pPacket->getResult() == PICK_SUCCESS )
		{
			INT nIndex = 0;
			const tObject_Item* pItem = CDataPool::GetMe()->ItemBox_GetItem(
											pPacket->getItemID().m_World,
											pPacket->getItemID().m_Server,
											pPacket->getItemID().m_Serial,
											nIndex);

			if(!pItem) 
			{
				if(bLog) ADDNEWDEBUGMSG(STRING("内部错误，非法的物品GUID!"));
				return PACKET_EXE_CONTINUE;
			}

			//如果发生物品转移，(从箱子里面移到包裹里),则改变客户端id，以表示在客户端是不同物体
			((CObject_Item*)pItem)->ChangeClientID();
			((CObject_Item*)pItem)->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);

			tObject_Item* pItemBag = CDataPool::GetMe()->UserBag_GetItem(pPacket->getBagIndex());
			if(pItemBag != NULL)
			{//有东西,应该一定是一类物品
				if(pItem->GetIdTable() == pItemBag->GetIdTable())
				{//同一类物品
					CDataPool::GetMe()->ItemBox_SetItem(nIndex, NULL, FALSE);
				}
				else
				{
					KLAssert(0);
					return PACKET_EXE_CONTINUE;
				}
			}
			else
			{//没东西
				//存入数据池
				CDataPool::GetMe()->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItem);
				CDataPool::GetMe()->ItemBox_SetItem(nIndex, NULL, FALSE);
			}
			
			//通知ActionButton
			CActionSystem::GetMe()->UserBag_Update();
			CActionSystem::GetMe()->ItemBox_Update();

			//产生ui事件
//			char szMsg[MAX_PATH];
//			_snprintf(szMsg, MAX_PATH, "拾取了物品:%s", ((CObject_Item*)pItem)->GetName());
//			CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "PACKET", szMsg, 4 );
			if(bLog)
			{
				STRING strTemp = COLORMSGFUNC("battle_get_myself", ((CObject_Item*)pItem)->GetName());
				ADDTALKMSG(strTemp);
			}

			CEventSystem::GetMe()->PushEvent(GE_LOOT_SLOT_CLEARED);
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());

			//如果掉落箱空，关闭
			if(CDataPool::GetMe()->ItemBox_GetNumber() == 0)
			{
				CEventSystem::GetMe()->PushEvent(GE_LOOT_CLOSED);
			}
		}
		else
		{
			switch( pPacket->getResult() )
			{
			case PICK_BAG_FULL:
				{
					if(bLog) 
					{
						STRING strTemp = "";
						strTemp = NOCOLORMSGFUNC("GCPickResultHandler_Info_package_Full");
						ADDNEWDEBUGMSG(strTemp);
						//ADDNEWDEBUGMSG(STRING("背包已满!"));
					}
					CSoundSystemFMod::_PlayUISoundFunc(96);
				}
				break;
			case PICK_INVALID_OWNER:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("该物品不能拾取!"));
				}
				break;
			case PICK_INVALID_ITEM:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("非法物品!"));
				}
				break;
			case PICK_TOO_FAR:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("距离太远!"));
				}
				break;
			default:
				break;
			}
		}
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}