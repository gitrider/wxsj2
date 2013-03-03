#include "StdAfx.h"
#include "GCPlayerShopType.h"

UINT GCPlayerShopTypeHandler::Execute( GCPlayerShopType* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
