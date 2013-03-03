
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
				//	CGameProcedure::s_pGfxSystem->PushDebugString("装备升星成功");
					STRING strTemp = "装备附魔成功";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIP_FAIL:
				{
					STRING strTemp = "装备附魔失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIPBYTE_ERROR:
				{
					STRING strTemp = "这件装备不能进行附魔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_EQUIP_HAVE_MAGIC:
				{
					STRING strTemp = "该装备上存在一个附魔效果，无法进行附魔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_BEAMLEVEL_BIG_HUMANLEVEL:
				{
					STRING strTemp = "你的等级不足";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_BEAMLEVEL_BIG_EQUIPLEVEL:
				{
					STRING strTemp = "该魂珠过于强大，无法附魔在这个等级的装备上";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPADSORMAGIC_MONEY_NOT_ENOUGH:
				{
					STRING strTemp = "您的游戏币不足，无法进行附魔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			default:
				{
					//CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					//CGameProcedure::s_pGfxSystem->PushDebugString("Equip LEVELUP UNKNOWN ERROR");
					STRING strTemp = "装备附魂未知错误";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

	  }

	return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}