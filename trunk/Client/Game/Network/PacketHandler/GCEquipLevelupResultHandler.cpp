
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
					CGameProcedure::s_pGfxSystem->PushDebugString("װ�����ǳɹ�");
					STRING strTemp = "װ�����ǳɹ�";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

					return PACKET_EXE_CONTINUE ;

				}
				break;

			case EQUIPLEVEUP_COMMONEQUIP:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("��ͨװ����������");
					STRING strTemp = "��ͨװ����������";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;



			case EQUIPLEVEUP_FAIL_SUCCESS:
				{

					//����ʧ����Ч
					//CSoundSystemFMod::_PlayUISoundFunc(47);
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("װ������ʧ��");
					STRING strTemp = "װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case EQUIPLEVEUP_EQUIP_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("װ��������");

					STRING strTemp = "װ��������";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			case EQUIPLEVEUP_ITEMTYPE_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("��ʯ���Ͳ���");

					STRING strTemp = "��ʯ���Ͳ���";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_ITEMNUM_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("��û���㹻��������ʯ");

					STRING strTemp = "��û���㹻��������ʯ ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_FULL_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("װ���Ѿ��ﵽ����Ǽ�");

					STRING strTemp = "װ���Ѿ��ﵽ����Ǽ�";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case EQUIPLEVEUP_MONEY_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("���������Ǯ����");

					STRING strTemp = "���������Ǯ����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;


			case EQUIPLEVEUP_FAIL:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("װ������ʧ��");

					STRING strTemp = "װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
		

			default:
				{
					CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					CGameProcedure::s_pGfxSystem->PushDebugString("Equip LEVELUP UNKNOWN ERROR");
					STRING strTemp = "װ������δ֪����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}
 
		 


			////���������Ʒ������
			//INT nTypeOwner	= CDataPool::GetMe()->AddStar_GetItemConta();
			//INT nItemPos	= CDataPool::GetMe()->AddStar_GetItemPos();

			//switch( nTypeOwner ) 
			//{
			//case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
			//	{
			//		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			//		if(pItem)
			//		{
			//			pItem->SetLock(FALSE);
			//		}


			//		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			//	}
			//	break;
			//case tObject_Item::IO_MYSELF_EQUIP:		// ��ҵ�װ��
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