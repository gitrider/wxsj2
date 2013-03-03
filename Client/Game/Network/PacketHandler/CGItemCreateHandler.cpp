#include "StdAfx.h"
#include "CGItemCreate.h"

using namespace Packets;

uint CGItemCreateHandler::Execute( CGItemCreate* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}