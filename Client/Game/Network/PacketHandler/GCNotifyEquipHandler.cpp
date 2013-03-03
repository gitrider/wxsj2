#include "StdAfx.h"
#include "GCNotifyEquip.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\Interface\GMGameInterface_Script_SouXia.h"
uint GCNotifyEquipHandler::Execute( GCNotifyEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNotifyEquipHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;   

		if( pPacket->GetOpType() ==  Packets::GCNotifyEquip::OPERATOR_BAG )
		{
			WORD wBagIndex = pPacket->GetBagIndex();

			_ITEM* pItem = pPacket->GetItem();  

			CObject_Item* pItemObj = CObject_Item::NewItem( pItem->m_ItemIndex);

			if(!pItemObj) return PACKET_EXE_CONTINUE;

			pItemObj->SetGUID(
				pItem->m_ItemGUID.m_World, 
				pItem->m_ItemGUID.m_Server,
				pItem->m_ItemGUID.m_Serial);

			//pItemObj->SetNumber(pItem->GetItemCount());

			pItemObj->SetExtraInfo(pItem);

			CDataPool::GetMe()->UserBag_SetItem(wBagIndex, pItemObj );
			CActionSystem::GetMe()->UserBag_Update();

			//UI
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


			//产生ui事件
		 	char szMsg[MAX_PATH];
		 	_snprintf(szMsg, MAX_PATH, "你获得了物品:%s", ((CObject_Item*)pItemObj)->GetName());
//			CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "PACKET", szMsg, 4 );
			CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, szMsg );
 
			// 显示到聊天框
			 if(bLog)
			 {
			 	STRING strTemp = COLORMSGFUNC("battle_get_myself", ((CObject_Item*)pItemObj)->GetName());
			 	ADDTALKMSG(strTemp);
			 }

			if(pItemObj->GetItemClass() == ICLASS_COMITEM )
			{
				if ( pItemObj->GetItemTableType() == COMITEM_SOUXIA )
				{
					CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_TAB );
				}
			}


		}
		else if( pPacket->GetOpType() ==  Packets::GCNotifyEquip::OPERATOR_BUY_BACK )
		{
			// 回购栏操作 后面有消息更新
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;



}

 
