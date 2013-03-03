
/**	GCCharEquipmentHandler.cpp
 *	
 *	功能：	人物装备消息
 *	
 *	修改记录：
 *			080222	增加主角判断， 如果是主角就不处理此消息， 
 *					否则会导致加一件装备的时候在 GCUseEquipResultHandler 服务器返回使用结果时加一次， 在此又加一次， 客户端重复创建
 *			
 */

#include "StdAfx.h"
#include "GITimeSystem.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GCCharEquipment.h"
#include "GIUIDataPool.h"
#include "Global.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "DataPool\GMDP_CharacterData.h"



uint GCCharEquipmentHandler :: Execute( GCCharEquipment* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	//AxTrace(0, 2, "GCCharEquipmentHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		
		if(pObj == NULL || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			return PACKET_EXE_CONTINUE;

		// 刷入数据池
		CCharacterData * pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		if( pObj != (CObject*)CObjectManager::GetMe()->GetMySelf() )
		{
			UINT dwEquipFlag = pPacket->getFlags();

            // 主手
			if(dwEquipFlag & (1<<HEQUIP_MAINHAND))
			{
				pCharacterData->Set_Equip( HEQUIP_MAINHAND, pPacket->getMainWeaponID());
			}
			// 副手
			if(dwEquipFlag & (1<<HEQUIP_ASSIHAND))
			{
				pCharacterData->Set_Equip( HEQUIP_ASSIHAND, pPacket->getAssiWeaponID());
			}
			// 头
			if(dwEquipFlag & (1<<HEQUIP_HEAD))	 
			{
				pCharacterData->Set_Equip(HEQUIP_HEAD, pPacket->getCapID() );
			}
            //身体
			if(dwEquipFlag & (1<<HEQUIP_BACK))
			{
				pCharacterData->Set_Equip(HEQUIP_BACK,pPacket->getArmourID());
			}
             //腕
			if(dwEquipFlag & (1<<HEQUIP_WRIST)) 
			{
				pCharacterData->Set_Equip(HEQUIP_WRIST, pPacket->getCuffID());
			}
             //脚
			if(dwEquipFlag & (1<<HEQUIP_FEET))  
			{
				pCharacterData->Set_Equip(HEQUIP_FEET, pPacket->getFootID());
			}
			//外装
			if (dwEquipFlag &(1<<HEQUIP_SUIT))
			{
				pCharacterData->Set_Equip(HEQUIP_SUIT,pPacket->getSuitID());
			}
			//肩部
			if (dwEquipFlag &(1<<HEQUIP_SHOULDER))
			{
              	pCharacterData->Set_Equip(HEQUIP_SHOULDER,pPacket->getShoulderID());
			}

			//坐骑
			if (dwEquipFlag &(1<<HEQUIP_RIDER))
			{
				pCharacterData->Set_Equip(HEQUIP_RIDER,pPacket->getRiderID());
			}

			//搜侠录
			if (dwEquipFlag &(1<<HEQUIP_SOUXIA))
			{
				pCharacterData->Set_Equip(HEQUIP_SOUXIA,pPacket->getSouxiaID() );
			}
		}

		pObj->PushDebugString("GCCharEquipment");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

