
#include "StdAfx.h"
#include "GCEquipAddHoleResult.h"
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


uint GCEquipAddHoleResultHandler::Execute(GCEquipAddHoleResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		ADDHOLE_RESULT ret = (ADDHOLE_RESULT)pPacket->getResult();
		switch(ret)
		{
		case ADDHOLE_SUCCESS:
			{

				 
				 CEventSystem::GetMe()->PushEvent(GE_ADD_HOLE_SUCCESS);

				 CGameProcedure::s_pGfxSystem->PushDebugString("װ����׳ɹ�");
				 STRING strTemp = "װ����׳ɹ�" ;
				 CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				 return PACKET_EXE_CONTINUE ;

			}
			break;

		case ADDHOLE_ITEM_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("��׵ĵ��߲�����");


				STRING strTemp = "��׵ĵ��߲�����" ;
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		
			}
			break;

		case ADDHOLE_EQUIP_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("���ʧ��");


				STRING strTemp = "���ʧ��" ;
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

			}
			break;

		case ADDHOLE_FULL_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString(" ����Ŀ�ﵽ����");

			}

		case ADDHOLE_MONEY_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("��Ǯ����");
				
				STRING strTemp = "��Ǯ����";
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
			break;

		default:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Equip UNKNOWN ERROR");
			}
			break;
		}
 

		////������������Ʒ������ ֻ��ȡ���ڽ���� �Ժ�߻����� �ٴ�
		//INT nTypeOwner	= CDataPool::GetMe()->AddHole_GetItemConta();
		//INT nItemPos	= CDataPool::GetMe()->AddHole_GetItemPos();
		//switch( nTypeOwner ) 
		//{
		//case tObject_Item::IO_MYSELF_PACKET:				// ������ҵı���
		//	{
		//		tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

		//		if(pItem)
		//		{
		//			pItem->SetLock(FALSE);
		//		}
		//		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);		//	}
		//	break;
		//case tObject_Item::IO_MYSELF_EQUIP:			//
		//	{
		//		tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (nItemPos));

		//		if(pItem)
		//		{
		//			pItem->SetLock(FALSE);
		 
				//֪ͨװ������״̬
		//			CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
		//		}
		//		 
		//	}
		//	break;
		//default:
		//	break;
		//}
		//
		//CDataPool::GetMe()->AddHole_CleanUP();
		 

	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}