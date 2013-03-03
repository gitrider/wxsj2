#include "StdAfx.h"
#include "GCUseGemResult.h"

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

uint GCUseGemResultHandler::Execute(GCUseGemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			USEGEM_RESULT ret = pPacket->GetResult();

			switch(ret)
			{
			case USEGEM_SUCCESS:
				{
					CEventSystem::GetMe()->PushEvent(GE_COMPOSE_GEM_SUCCESS);
				}
				break;

			case USEGEM_CANNT_USE:
				{
					//CGameProcedure::s_pGfxSystem->PushDebugString("不能镶嵌");

					STRING strTemp = "不能镶嵌" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_LEVEL_FAIL:
				{
					STRING strTemp = "等级不够" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}

			case USEGEM_EQUIP_GEM_FULL:
				{
					STRING strTemp = "";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_CANNT_COMBINE_GEM:
				{
					STRING strTemp = "宝石不能镶嵌" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_EQUIP_FAIL:
				{
					STRING strTemp = "该物品不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_GEMREQ_FAIL:
				{
					STRING strTemp = "需求的宝石不正确" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_CANNT_FINDGEM:
				{
					STRING strTemp = "该宝石不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_GEM_FAIL:
				{
					STRING strTemp = "找不到宝石" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				}
				break;

			case USEGEM_FAIL:
			default:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("Equip UNKNOWN ERROR");
				}
				break;
			}
		}


		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}