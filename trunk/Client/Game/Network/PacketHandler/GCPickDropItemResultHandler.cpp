#include "StdAfx.h"
#include "GCPickDropItemResult.h"
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
uint GCPickDropItemResultHandler::Execute(GCPickDropItemResult* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			tUIDataPool* pDataPool = CGameProcedure::s_pUIDataPool;
			
			BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;   
			 
			if( pPacket->getResult() == PICK_SUCCESS )	//�Ƿ�ɹ� 
			{


  

				//const tObject_Item* pItem = CDataPool::GetMe()->ItemDrop_GetItem(pPacket->getDropItemId());

				//if(!pItem) 
				//{
				//	if(bLog) ADDNEWDEBUGMSG(STRING("�ڲ����󣬷Ƿ�����ƷGUID!"));
				//	return PACKET_EXE_CONTINUE;
				//}

				////���������Ʒת�ƣ�(�����������Ƶ�������),��ı�ͻ���id���Ա�ʾ�ڿͻ����ǲ�ͬ����
				//((CObject_Item*)pItem)->ChangeClientID();
				//((CObject_Item*)pItem)->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);

				//tObject_Item* pItemBag = CDataPool::GetMe()->UserBag_GetItem(pPacket->getBagIndex());

				//if(pItemBag != NULL)
				//{
				//	//�ж���,Ӧ��һ����һ����Ʒ
				//	if(pItem->GetIdTable() == pItemBag->GetIdTable())
				//	{//ͬһ����Ʒ
				//		CDataPool::GetMe()->ItemDrop_SetItem(pPacket->getDropItemId(), NULL, FALSE);
				//	}
				//	else
				//	{
				//		KLAssert(0);
				//		return PACKET_EXE_CONTINUE;
				//	}
				//}
				//else
				//{//û����
				//	//�������ݳ�
				//	CDataPool::GetMe()->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItem);
				//	CDataPool::GetMe()->ItemDrop_SetItem(pPacket->getDropItemId(), NULL, FALSE);
				//}


				////֪ͨActionButton
				//CActionSystem::GetMe()->UserBag_Update();
			 //

				////����ui�¼�
				////			char szMsg[MAX_PATH];
				////			_snprintf(szMsg, MAX_PATH, "ʰȡ����Ʒ:%s", ((CObject_Item*)pItem)->GetName());
				////			CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "PACKET", szMsg, 4 );
				//if(bLog)
				//{
				//	STRING strTemp = COLORMSGFUNC("battle_get_myself", ((CObject_Item*)pItem)->GetName());
				//	ADDTALKMSG(strTemp);
				//}
				//
				//CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());


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