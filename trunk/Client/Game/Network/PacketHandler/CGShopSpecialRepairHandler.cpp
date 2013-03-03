#include "StdAfx.h"
#include "CGShopSpecialRepair.h"

using namespace Packets;

uint CGShopSpecialRepairHandler::Execute( CGShopSpecialRepair* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}