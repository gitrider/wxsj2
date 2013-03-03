
#include "StdAfx.h"
#include "GCSaveEquipSuitResult.h"
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


uint GCSaveEquipSuitResultHandler::Execute(GCSaveEquipSuitResult* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			// 刷入数据池
			CCharacterData * pCharacterData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();

			if( pCharacterData )
			{
				EQUIPSUIT_RESULT ret = (EQUIPSUIT_RESULT)pPacket->getResult();
				int suitNum = (int)pPacket->getSuitNum();

				switch( ret )
				{
				case EQUIPSUIT_SUCCESS:
					{
						_SUIT_SETTING aSuit = pPacket->getSuitSetting();

						pCharacterData->Set_EquipSuit(suitNum, aSuit);

						// 通知UI更新
						CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE, aSuit.m_SuitName, "server", pPacket->getSuitNum() );

						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "保存换装成功" );
					}
					break;

				case EQUIPSUIT_EQUIP_FAIL:
					{
						STRING strTemp = "保存失败";
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					}
					break;
				}
			}
		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

			return PACKET_EXE_ERROR ;
}