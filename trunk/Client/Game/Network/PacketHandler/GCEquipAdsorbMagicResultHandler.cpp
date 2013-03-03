
#include "StdAfx.h"
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
#include "GCEquipAdsorbMagicResult.h"

using namespace Packets;



uint GCEquipAdsorbMagicResultHandler::Execute(GCEquipAdsorbMagicResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPADSORMAGIC_RESULT ret = (EQUIPADSORMAGIC_RESULT)pPacket->getResult();

			switch(ret)
			{
			case EQUIPADSORMAGIC_SUCCESS:
				{

				//	CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,1);
				//	CGameProcedure::s_pGfxSystem->PushDebugString("װ�����ǳɹ�");
					STRING strTemp = "װ����ħ�ɹ�";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIP_FAIL:
				{
					STRING strTemp = "װ����ħʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIPBYTE_ERROR:
				{
					STRING strTemp = "���װ�����ܽ��и�ħ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIP_HAVE_MAGIC:
				{
					STRING strTemp = "��װ���ϴ���һ����ħЧ�����޷����и�ħ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_BEAMLEVEL_BIG_HUMANLEVEL:
				{
					STRING strTemp = "��ĵȼ�����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_BEAMLEVEL_BIG_EQUIPLEVEL:
				{
					STRING strTemp = "�û������ǿ���޷���ħ������ȼ���װ����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_MONEY_NOT_ENOUGH:
				{
					STRING strTemp = "������Ϸ�Ҳ��㣬�޷����и�ħ";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			default:
				{
					//CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					//CGameProcedure::s_pGfxSystem->PushDebugString("Equip LEVELUP UNKNOWN ERROR");
					STRING strTemp = "װ������δ֪����";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

	  }

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}