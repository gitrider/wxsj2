#include "StdAfx.h"

#include "GCRetSetting.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Interface\GMInterface_Script_SystemSetup.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
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

uint GCRetSettingHandler::Execute( GCRetSetting* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	_OWN_SETTING* pSetting = pPacket->GetSetting();

	SCRIPT_SANDBOX::SystemSetup::GetMe()->Set_GameData(pSetting[SETTING_TYPE_GAME].m_SettingData);
	
	for(INT i=SETTING_TYPE_K0;i < SETTING_TYPE_K29+1; i++)
	{
		CActionSystem::GetMe()->MainMenuBar_SetID(i-SETTING_TYPE_K0,pSetting[i].m_SettingType,pSetting[i].m_SettingData);
	}

	SCRIPT_SANDBOX::Talk::s_Talk.LoadTabSetting(pPacket);


	CCharacterData* pCharacterData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();

	if( pCharacterData )
	{
		_SUIT_SETTING* pSuits = pPacket->GetSuitSetting();
		for( int i = 0; i < MAX_SUIT_SETTING; i++ )
		{
			pCharacterData->Set_EquipSuit(i, pSuits[i]);
		}
		CEventSystem::GetMe()->PushEvent( GE_UPDATE_QUICK_CHANGE_EQUIPNAME );
	}
	CDataPool::GetMe()->SetAttriAutoPoint(pPacket->GetSetting()[SETTING_TYPE_ATTRI_AUTO_POINT].m_SettingData);
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
