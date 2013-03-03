#include "stdafx.h"
#include "WLRetCharLogin.h"
#include "WLBillingStart.h"
#include "WLBillingEnd.h"
#include "WLBillingKeep.h"
#include "WLBillingKick.h"
#include "WLAskPrize.h"
#include "WLNotifyUser.h"
#include "WLChangeName.h"
#include "LWAskCharLogin.h"


UINT WLRetCharLoginHandler::Execute(WLRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT	WLBillingStartHandler::Execute(WLBillingStart* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT	WLBillingEndHandler::Execute(WLBillingEnd* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT WLBillingKeepHandler::Execute(WLBillingKeep* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

UINT WLBillingKickHandler::Execute(WLBillingKick* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}

UINT WLAskPrizeHandler::Execute(WLAskPrize* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}

UINT WLNotifyUserHandler::Execute(WLNotifyUser* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}

UINT WLChangeNameHandler::Execute( WLChangeName* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR;
}
