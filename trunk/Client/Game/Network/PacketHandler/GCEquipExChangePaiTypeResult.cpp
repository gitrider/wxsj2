
#include "StdAfx.h"
#include "GCEquipExChangePaiTypeResult.h"
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
 


uint GCEquipExChangePaiTypeResultHandler::Execute(GCEquipExChangePaiTypeResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			EQUIPEXCHANGEPAITTYPE_RESULT ret = (EQUIPEXCHANGEPAITTYPE_RESULT)pPacket->GetResult();

			switch(ret)
			{
			case EQUIPEXCHANGEPAITTYPE_SUCCESS:
				{

					CEventSystem::GetMe()->PushEvent(GE_EQUIP_SWAPCARD_SUCESS,1);// 1��ʾ�ɹ� 0ʧ��
					CGameProcedure::s_pGfxSystem->PushDebugString("����װ�����ͳɹ�");
					STRING strTemp = "����װ�����ͳɹ�";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

				
			case EQUIPEXCHANGEPAITTYPE_EQUIPBIND_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("�ǰ�װ����������ʧ��");
					STRING strTemp = "�ǰ�װ����������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;


			case EQUIPEXCHANGEPAITTYPE_EQUIP_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("����װ������ʧ��");
					STRING strTemp = "����װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case EQUIPEXCHANGEPAITTYPE_EQUIPPAI_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("����װ������ʧ��");
					STRING strTemp = "����װ������ʧ��";
					CGameProcedure::s_pEventSystem ->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case EQUIPEXCHANGEPAITTYPE_ITEM_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("���߲�����Ҫ��");
					STRING strTemp = "���߲�����Ҫ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}

				break;

			case EQUIPEXCHANGEPAITTYPE_FAIL:
				{

					CGameProcedure::s_pGfxSystem->PushDebugString("����װ������ʧ��");
					STRING strTemp = "����װ������ʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}

				break;
			}

			INT nTypeOwner	=	-1;
			INT	nItemPos	=	-1;

			for (INT i = 0 ; i < 2 ;++i)
			{
				if ( 0 == i)
				{
					nTypeOwner	=	CDataPool::GetMe()->SwapCard_GetItemConta1();
					nItemPos	    =	CDataPool::GetMe()->SwapCard_GetItemPos1();
				}else if (  1 == i)
				{
					nTypeOwner	=	CDataPool::GetMe()->SwapCard_GetItemConta2();
					nItemPos	    =	CDataPool::GetMe()->SwapCard_GetItemPos2();
				}
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
						}
					}
					break;
				default:
					break;
				}

			}

			INT  nITEM =  CDataPool::GetMe()->SwapCard_GetNeedItemPos();

			tObject_Item* pItemNeed = CDataPool::GetMe()->UserBag_GetItem(nITEM);

			if(pItemNeed)
			{
				pItemNeed->SetLock(FALSE);
			}

			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->SwapCard_CleanUP(); 


		}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}