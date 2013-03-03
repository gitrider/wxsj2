
#include "StdAfx.h"
#include "GCItemCreateResult.h"
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

//װ���ϳɽ��
uint GCItemCreateResultHandler::Execute(GCItemCreateResult * pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			ITEMCREATE_RESULT ret = (ITEMCREATE_RESULT)pPacket->GetResult();
			switch(ret)
			{

			case ITEMCREATE_SUCCESS:
				{		 
					CGameProcedure::s_pGfxSystem->PushDebugString("�ϳ�װ���ɹ�");
					STRING strTemp = "�ϳ�װ���ɹ�" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_UPDATE_SOUXIA_TAB );
					
				}
				break;

			case ITEMCREATE_ITEM_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("��Ʒ������");

					STRING strTemp = "��Ʒ������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;


			case ITEMCREATE_MATERIAL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���ϲ�����");

					STRING strTemp = "���ϲ�����" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_MATERIAL_LEVEL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���ϵȼ�����");

					STRING strTemp = "���ϵȼ�����" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_MATERIAL_COUNT:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("������������");

					STRING strTemp = "������������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case ITEMCREATE_BAG_FULL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("��ұ����ռ䲻��");

					STRING strTemp = "��ұ����ռ䲻��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���ɲ���ʧ��");

					STRING strTemp = "���ɲ���ʧ��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			default:
				{
 

				}


			}
			
		}



    return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}