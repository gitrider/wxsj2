#include "StdAfx.h"
#include "CGEquipExChangePaiType.h"

using namespace Packets;

uint CGEquipExChangePaiTypeHandler::Execute( CGEquipExChangePaiType* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}