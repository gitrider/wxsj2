
/**	GCCharEquipmentHandler.cpp
 *	
 *	���ܣ�	����װ����Ϣ
 *	
 *	�޸ļ�¼��
 *			080222	���������жϣ� ��������ǾͲ��������Ϣ�� 
 *					����ᵼ�¼�һ��װ����ʱ���� GCUseEquipResultHandler ����������ʹ�ý��ʱ��һ�Σ� �ڴ��ּ�һ�Σ� �ͻ����ظ�����
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

		// ˢ�����ݳ�
		CCharacterData * pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		if( pObj != (CObject*)CObjectManager::GetMe()->GetMySelf() )
		{
			UINT dwEquipFlag = pPacket->getFlags();

            // ����
			if(dwEquipFlag & (1<<HEQUIP_MAINHAND))
			{
				pCharacterData->Set_Equip( HEQUIP_MAINHAND, pPacket->getMainWeaponID());
			}
			// ����
			if(dwEquipFlag & (1<<HEQUIP_ASSIHAND))
			{
				pCharacterData->Set_Equip( HEQUIP_ASSIHAND, pPacket->getAssiWeaponID());
			}
			// ͷ
			if(dwEquipFlag & (1<<HEQUIP_HEAD))	 
			{
				pCharacterData->Set_Equip(HEQUIP_HEAD, pPacket->getCapID() );
			}
            //����
			if(dwEquipFlag & (1<<HEQUIP_BACK))
			{
				pCharacterData->Set_Equip(HEQUIP_BACK,pPacket->getArmourID());
			}
             //��
			if(dwEquipFlag & (1<<HEQUIP_WRIST)) 
			{
				pCharacterData->Set_Equip(HEQUIP_WRIST, pPacket->getCuffID());
			}
             //��
			if(dwEquipFlag & (1<<HEQUIP_FEET))  
			{
				pCharacterData->Set_Equip(HEQUIP_FEET, pPacket->getFootID());
			}
			//��װ
			if (dwEquipFlag &(1<<HEQUIP_SUIT))
			{
				pCharacterData->Set_Equip(HEQUIP_SUIT,pPacket->getSuitID());
			}
			//�粿
			if (dwEquipFlag &(1<<HEQUIP_SHOULDER))
			{
              	pCharacterData->Set_Equip(HEQUIP_SHOULDER,pPacket->getShoulderID());
			}

			//����
			if (dwEquipFlag &(1<<HEQUIP_RIDER))
			{
				pCharacterData->Set_Equip(HEQUIP_RIDER,pPacket->getRiderID());
			}

			//����¼
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

