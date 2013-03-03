// GCRemovePetHandler.cpp
#include "StdAfx.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "../../Event/GMEventSystem.h"
#include "../../Action/GMActionSystem.h"

#include "GCRemovePet.h"

using namespace Packets;

UINT GCRemovePetHandler::Execute( GCRemovePet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		PET_GUID_t guidPet = pPacket->GetGUID();
		for (INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
		{
			SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(i);
			if (pPetData && pPetData->m_GUID == guidPet)
			{
				pPetData->CleanUp();
				CActionSystem::GetMe()->UpdateToolBarForPetSkill();
				CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);
				break;
			}
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
