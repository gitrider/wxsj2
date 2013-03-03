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
#include "GCEquipCancelMagicResult.h"

using namespace Packets;

uint GCEquipCancelMagicResultHandler::Execute( GCEquipCancelMagicResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			EQUIPCANCELMAGIC_RESULT ret = (EQUIPCANCELMAGIC_RESULT)pPacket->getResult();

			switch(ret)
			{
			case EQUIPCANCELMAGIC_SUCCESS:
				{

					//	CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,1);
					//	CGameProcedure::s_pGfxSystem->PushDebugString("装备升星成功");
					STRING strTemp = "装备退魔成功";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPCANCELMAGIC_EQUIP_FAIL:
				{
					STRING strTemp = "装备退魔失败";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPCANCELMAGIC_EQUIPTYPE_ERROR:
				{
					STRING strTemp = "这件装备不能进行退魔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPCANCELMAGIC_EQUIP_NO_MAGIC:
				{
					STRING strTemp = "该武器/装备未被附魔，怎么能拿来退魔呢!";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPCANCELMAGIC_MONEY_NOT_ENOUGH:
				{
					STRING strTemp = "您的游戏币不足，无法进行退魔";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case EQUIPCANCELMAGIC_NO_GEM:
				{
					STRING strTemp = "退魔需要退魔石才能完成，请准备好退魔石再来!";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
				}
				break;

			default:
				{
					//CEventSystem::GetMe()->PushEvent(GE_OPEN_DA_XING_SUCCESS,0);
					//CGameProcedure::s_pGfxSystem->PushDebugString("Equip LEVELUP UNKNOWN ERROR");
					STRING strTemp = "装备退魂未知错误";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;
			}

		}

		return PACKET_EXE_CONTINUE ;


	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}