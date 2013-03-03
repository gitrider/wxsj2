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
		// ����ÿһ����Ʒ
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
				if(bLog) ADDNEWDEBUGMSG(STRING("�ڲ����󣬷Ƿ�����ƷGUID!"));
				return PACKET_EXE_CONTINUE;
			}

			//���������Ʒת�ƣ�(�����������Ƶ�������),��ı�ͻ���id���Ա�ʾ�ڿͻ����ǲ�ͬ����
			((CObject_Item*)pItem)->ChangeClientID();
			((CObject_Item*)pItem)->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);

			tObject_Item* pItemBag = CDataPool::GetMe()->UserBag_GetItem(pPacket->getBagIndex());
			if(pItemBag != NULL)
			{//�ж���,Ӧ��һ����һ����Ʒ
				if(pItem->GetIdTable() == pItemBag->GetIdTable())
				{//ͬһ����Ʒ
					CDataPool::GetMe()->ItemBox_SetItem(nIndex, NULL, FALSE);
				}
				else
				{
					KLAssert(0);
					return PACKET_EXE_CONTINUE;
				}
			}
			else
			{//û����
				//�������ݳ�
				CDataPool::GetMe()->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItem);
				CDataPool::GetMe()->ItemBox_SetItem(nIndex, NULL, FALSE);
			}
			
			//֪ͨActionButton
			CActionSystem::GetMe()->UserBag_Update();
			CActionSystem::GetMe()->ItemBox_Update();

			//����ui�¼�
//			char szMsg[MAX_PATH];
//			_snprintf(szMsg, MAX_PATH, "ʰȡ����Ʒ:%s", ((CObject_Item*)pItem)->GetName());
//			CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "PACKET", szMsg, 4 );
			if(bLog)
			{
				STRING strTemp = COLORMSGFUNC("battle_get_myself", ((CObject_Item*)pItem)->GetName());
				ADDTALKMSG(strTemp);
			}

			CEventSystem::GetMe()->PushEvent(GE_LOOT_SLOT_CLEARED);
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());

			//���������գ��ر�
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
						//ADDNEWDEBUGMSG(STRING("��������!"));
					}
					CSoundSystemFMod::_PlayUISoundFunc(96);
				}
				break;
			case PICK_INVALID_OWNER:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("����Ʒ����ʰȡ!"));
				}
				break;
			case PICK_INVALID_ITEM:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("�Ƿ���Ʒ!"));
				}
				break;
			case PICK_TOO_FAR:
				{
					if(bLog) ADDNEWDEBUGMSG(STRING("����̫Զ!"));
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