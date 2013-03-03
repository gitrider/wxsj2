#include "StdAfx.h"
#include "GCPlayerShopUpdateFavorite.h"

UINT GCPlayerShopUpdateFavoriteHandler::Execute( GCPlayerShopUpdateFavorite* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
