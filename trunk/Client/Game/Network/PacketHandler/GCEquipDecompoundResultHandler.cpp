
#include "StdAfx.h"
#include "GCEquipDecompoundResult.h"
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
 

uint GCEquipDecompoundResultHandler::Execute(GCEquipDecompoundResult * pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPDECOMPOUND_RESULT ret = (EQUIPDECOMPOUND_RESULT)pPacket->GetResult();
			switch(ret)
			{

			case EQUIPDECOMPOUND_SUCCESS:
				{	

				   CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_EQUIP_DISASS);	
					
				   CGameProcedure::s_pGfxSystem->PushDebugString("װ�����ɹ�");
				   STRING strTemp = "װ�����ɹ�" ;
				   CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				
				}
				break;

			case EQUIPDECOMPOUND_EQUIP_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("�����Ʒ������");

					STRING strTemp = "�����Ʒ������" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;


			case EQUIPDECOMPOUND_BAG_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("��ұ����ռ䲻��");

					STRING strTemp = "��ұ����ռ䲻��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case EQUIPDECOMPOUND_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���װ��ʧ��");

					STRING strTemp = "���װ��ʧ��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			default:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("���װ��ʧ��");

					STRING strTemp = "���װ��ʧ��" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

			//���������Ʒ������
			INT nTypeOwner	= CDataPool::GetMe()->Dismantle_GetItemConta();
			INT nItemPos	= CDataPool::GetMe()->Dismantle_GetItemPos();
 
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

						if (EQUIPDECOMPOUND_SUCCESS == (EQUIPDECOMPOUND_RESULT)pPacket->GetResult())
						{
							// �ı���Ʒ��id, �Ա�����µ�action ��ť
							((CObject_Item*)pItem)->ChangeClientID();

							CDataPool::GetMe()->UserEquip_SetItem(HUMAN_EQUIP (nItemPos), NULL, FALSE);

							//---------------------------------------------------
							//�߼���
							CObject_PlayerMySelf * pMySelf = CObjectManager::GetMe()->GetMySelf();
							//pMySelf->UnEquipItem( (HUMAN_EQUIP)pPacket->getEquipPoint() );

							//---------------------------------------
							//ˢ�½�ɫ����
							CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(HUMAN_EQUIP (nItemPos), -1);

							// �����������ϵ�װ����ActionSystem
							CActionSystem::GetMe()->UserEquip_Update();

							// ֪ͨ�����¼�
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

							CSoundSystemFMod::_PlayUISoundFunc(67);
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
						}
					}
				}
				break;
			default:
				break;
			}
	 

			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


			CDataPool::GetMe()->Dismantle_CleanUP();


		}

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}