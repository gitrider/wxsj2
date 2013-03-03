
#include "StdAfx.h"
#include "GCEquipChangePaiTypeResult.h"
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
 
 
uint GCEquipChangePaiTypeResultHandler::Execute(GCEquipChangePaiTypeResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			 
			EQUIPCHANGEPAITTYPE_RESULT ret = (EQUIPCHANGEPAITTYPE_RESULT)pPacket->GetResult();

			switch(ret)
			{
			case EQUIPCHANGEPAITTYPE_SUCCESS:
				{

					CEventSystem::GetMe()->PushEvent(GE_EQUIP_CHANGECARD_SUCESS,1);// 1��ʾ�ɹ� 0ʧ��
					CGameProcedure::s_pGfxSystem->PushDebugString("�ı�װ�����ͳɹ�");
					STRING strTemp = "�ı�װ�����ͳɹ�";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case EQUIPCHANGEPAITTYPE_EQUIP_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("�ı�װ������ʧ��");
					STRING strTemp = "�ı�װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				 
				}
			break;
			case EQUIPCHANGEPAITTYPE_ITEM_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("���߲�����Ҫ��");
					STRING strTemp = "���߲�����Ҫ��";
					CGameProcedure::s_pEventSystem ->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					 
				}
			break;
			case EQUIPCHANGEPAITTYPE_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("�ı�װ������ʧ��");
					STRING strTemp = "�ı�װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					 
				}

			break;

			}

			INT nTypeOwner	=	CDataPool::GetMe()->ChangeCard_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->ChangeCard_GetItemPos();

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

			INT  nITEM =  CDataPool::GetMe()->ChangeCard_GetNeedItemPos();

			tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

			if(pItemNeed)
			{
				pItemNeed->SetLock(FALSE);
			}


			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->ChangeCard_CleanUP(); 


		}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}