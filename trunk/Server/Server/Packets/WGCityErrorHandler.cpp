
#include "stdafx.h"
#include "WGCityError.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

uint WGCityErrorHandler::Execute( WGCityError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if( pPlayer->IsServerPlayer() )
	{//�������յ��������������������

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
