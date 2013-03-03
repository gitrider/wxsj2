#include "stdafx.h"

#include "GCStallOpenPublic.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMyself.h"
#include "DataPool\GMDP_CharacterData.h"


uint GCStallOpenPublicHandler::Execute( GCStallOpenPublic* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_StallOpenPublic( pPacket->GetIsOpen() );
			CEventSystem::GetMe()->PushEvent( GE_STALL_OPEN_PUBLIC, pPacket->GetIsOpen() );			
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
