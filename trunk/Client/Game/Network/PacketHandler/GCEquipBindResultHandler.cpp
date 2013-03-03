
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

					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,1);// 1��ʾ�ɹ� 0ʧ��

				}
				break;

			case EQUIPBIND_EQUIP_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("��װ��ʧ��");
					STRING strTemp = "��װ��ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}

			break;
			case EQUIPBIND_BALIVE:
				{
				
					CGameProcedure::s_pGfxSystem->PushDebugString("װ���Ѿ���");
					STRING strTemp = "װ���Ѿ���";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
			break;
			case EQUIPBIND_ITEM_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("�󶨵��߲�����");

					STRING strTemp = "�󶨵��߲�����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
				break;


			case EQUIPBIND_FULL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("װ���ﵽ������");

					STRING strTemp = "װ���ﵽ������";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
				break;


			case EQUIPBIND_MONEY_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("��ҽ�Ǯ����");

					STRING strTemp = "��ҽ�Ǯ����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
				break;

			case EQUIPBIND_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("��ʧ��");

					STRING strTemp = "��ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
				break;

			default:
				{

					STRING strTemp = "��δ֪����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CEventSystem::GetMe()->PushEvent(GE_ADD_BANG_DING_SUCCESS,0);// 1��ʾ�ɹ� 0ʧ��
				}
				break;
			}
 


			//���������Ʒ������
			INT nTypeOwner	= CDataPool::GetMe()->Bind_GetItemConta();
			INT nItemPos	= CDataPool::GetMe()->Bind_GetItemPos();

			INT  nITEM =  CDataPool::GetMe()->Bind_GetNeedItemPos();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);
 
					if(pItem)
					{
						pItem->SetLock(FALSE);
					}


					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:		// ��ҵ�װ��
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