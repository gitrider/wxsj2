
#include "StdAfx.h"
#include "GCUseEquipResult.h"
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
#include "..\Game\Interface\GMGameInterface_Script_SouXia.h"
 

using namespace Packets;


uint GCUseEquipResultHandler::Execute(GCUseEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCUseEquipResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		UseEquipResultCode ret = (UseEquipResultCode)pPacket->getResult();
		switch(ret)
		{
		case USEEQUIP_SUCCESS:
			{
				//---------------------------------------------------------
				// UI���ݳ�
				CDataPool* pDataPool = CDataPool::GetMe();

				// �������װ��
				tObject_Item* pItemAtBag = pDataPool->UserBag_GetItem(pPacket->getBagIndex());
				if(!pItemAtBag) return PACKET_EXE_CONTINUE;

				// ���ϵ�װ��
				tObject_Item* pItemAtUser= pDataPool->UserEquip_GetItem( (HUMAN_EQUIP)pPacket->getEquipPoint());


				//���������ڵ���������
				if( SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.IsSouXiaItem(pPacket->getBagIndex(), 0 ) )
				{
					CEventSystem::GetMe()->PushEvent(GE_CLEAR_SOUXIA_BAR);
					CEventSystem::GetMe()->PushEvent(GE_CLOSE_SOUXIA);
				}

				// װ����Ϣ����
				if(pItemAtBag->GetItemClass() != ICLASS_EQUIP || 
					((CObject_Item_Equip*)pItemAtBag)->GetItemType() != pPacket->getEquipPoint()) return PACKET_EXE_CONTINUE;

		    


				// ���������Ʒת�ƣ���ı�ͻ���id���Ա�ʾ�ڿͻ����ǲ�ͬ����
				((CObject_Item*)pItemAtBag)->ChangeClientID();
				if(pItemAtUser)	((CObject_Item*)pItemAtUser)->ChangeClientID();



				//---------------------------------------
				// ˢ�½�ɫ����
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(
					HUMAN_EQUIP( pPacket->getEquipPoint()), pItemAtBag->GetIdTable());		

			
				//---------------------------------------
				// ˢ�°�������
				if(pItemAtUser)
					pItemAtUser->SetGUID(pPacket->getItemId().m_World, 
					pPacket->getItemId().m_Server,
					pPacket->getItemId().m_Serial);

				pDataPool->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItemAtUser, FALSE);

				// ˢ�����ݳ�
				//pDataPool->UserEquip_SetItem(((CObject_Item_Equip*)pItemAtBag)->GetItemType(), (tObject_Item*)pItemAtBag, FALSE);
			
				pDataPool->UserEquip_SetItem(  HUMAN_EQUIP(pPacket->getEquipPoint()), (tObject_Item*)pItemAtBag, FALSE);

				CActionSystem::GetMe()->UserBag_Update();

				// �����¼�
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());

				CSoundSystemFMod::_PlayUISoundFunc(67);
			}
			break;

		case USEEQUIP_LEVEL_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Need Level failed!");

				STRING strTemp = NOCOLORMSGFUNC("Player_Use_Equip_Level_Not_Enough");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		
			}
			break;

		case USEEQUIP_JOB_FAIL:
			{

				CGameProcedure::s_pGfxSystem->PushDebugString("Equip Profession failed!");

				STRING strTemp = NOCOLORMSGFUNC("Player_Use_Equip_Profession_Erro");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
			break;

		case USEEQUIP_TYPE_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Equip Type failed!");

			}

		case USEEQUIP_IDENT_FAIL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Equip need ident_fail!");
				
				STRING strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Need_Identity");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
			break;

		default:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Equip UNKNOWN ERROR");
			}
			break;
		}


		
		// �����������ϵ�װ����ActionSystem
		CActionSystem::GetMe()->UserEquip_Update();

		// ֪ͨ�����¼�
	 	CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}