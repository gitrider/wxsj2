
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

//装备合成结果
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
					CGameProcedure::s_pGfxSystem->PushDebugString("合成装备成功");
					STRING strTemp = "合成装备成功" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
					CGameProcedure::s_pEventSystem->PushEvent( GE_UPDATE_SOUXIA_TAB );
					
				}
				break;

			case ITEMCREATE_ITEM_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("物品不存在");

					STRING strTemp = "物品不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;


			case ITEMCREATE_MATERIAL_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("材料不存在");

					STRING strTemp = "材料不存在" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_MATERIAL_LEVEL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("材料等级不对");

					STRING strTemp = "材料等级不对" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_MATERIAL_COUNT:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("材料数量不足");

					STRING strTemp = "材料数量不足" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;
			case ITEMCREATE_BAG_FULL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("玩家背包空间不足");

					STRING strTemp = "玩家背包空间不足" ;
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

				}
				break;

			case ITEMCREATE_FAIL:
				{
					CGameProcedure::s_pGfxSystem->PushDebugString("生成材料失败");

					STRING strTemp = "生成材料失败" ;
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