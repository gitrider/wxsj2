#include "StdAfx.h"
#include "GCDetailEquipAtt.h"
#include "Procedure\GameProcedure.h"
#include "GameCommand.h"
#include "DataPool\DataPoolCommandDef.h"
#include "DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "Global.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Event\GMEventSystem.h"
#include "Action\GMActionSystem.h"

using namespace Packets;

UINT	GCDetailEquipAttHandler::Execute( GCDetailEquipAtt* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{

			return PACKET_EXE_CONTINUE ;
		}

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}