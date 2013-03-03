
#include "StdAfx.h"
#include "GCEquipRemouldResult.h"
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

 
uint GCEquipRemouldResultHandler::Execute(GCEquipRemouldResult * pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPREMOULD_RESULT ret = (EQUIPREMOULD_RESULT)pPacket->GetResult();
			switch(ret)
			{

			case EQUIPREMOULD_SUCCESS:
				{		 
					CGameProcedure::s_pGfxSystem->PushDebugString("װ������ɹ�");

					STRING strTemp = "װ������ɹ�" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
                    
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );
				}
				break;

			case EQUIPREMOULD_EQUIP_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("װ��������");

					STRING strTemp = "װ��������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;


			case EQUIPREMOULD_MAX_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("װ������ﵽ�������");

					STRING strTemp = "װ������ﵽ�������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );


				}
				break;

			case EQUIPREMOULD_MATERIAL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("������ϲ�����");

					STRING strTemp = "������ϲ�����" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			case EQUIPREMOULD_MATERIAL_LEVEL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("������ϵȼ�����");

					STRING strTemp = "������ϵȼ�����" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			case EQUIPREMOULD_MATERIAL_COUNT:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���������������");

					STRING strTemp = "���������������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
 
			case EQUIPREMOULD_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("װ������ʧ��");

					STRING strTemp = "װ������ʧ��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_REWORK );

				}
				break;
			 

			default:
				{


				}


			}

			INT nTypeOwner	=	CDataPool::GetMe()->Rework_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->Rework_GetItemPos();

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

			case tObject_Item::IO_MYSELF_EQUIP:					// ����������ϵ�װ��
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

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
			CDataPool::GetMe()->Rework_CleanUP(); 



		}
	

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}