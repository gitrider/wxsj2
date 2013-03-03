#include "StdAfx.h"
#include "GCLevelUp.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\ObjectCommandDef.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "DataPool\GMDataPool.h"
#include "event\GMEventSystem.h"

#include "GITimeSystem.h"


using namespace Packets;

uint GCLevelUpHandler::Execute(GCLevelUp* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		//AxTrace(0, 2, "GCLevelUp::Execute");

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjId() ));
			if ( pObj == NULL )
				return PACKET_EXE_CONTINUE;

			pObj->PushDebugString("LevelUp!");
			pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

			if(pObj ==(CObject*) CObjectManager::GetMe()->GetMySelf())
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("LeveUP");
				CSoundSystemFMod::_PlayUISoundFunc(66);
			}

			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_LEVEL_UP;
			pObj->PushCommand(&cmdTemp );

			CDataPool::GetMe()->SystemTip_AddEventInfo( STT_LEVELUP, "升级了" );
			// 更新界面显示
			CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}