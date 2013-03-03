// GCPlayerCallOfHandler.cpp

#include "StdAfx.h"

#include "GCPlayerCallOf.h"
#include "..\..\Event\GMEventSystem.h"

using namespace Packets;

uint GCPlayerCallOfHandler::Execute( GCPlayerCallOf* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//显示对话框，
	//确定和取消的时候发送 CGPlayerCallOfResult
	GUID_t PlayerGuid = pPacket->GetCallerGUID();

	// 到好友列表中取找到这个玩家的名字 

	CEventSystem::GetMe()->PushEvent(GE_OPEN_CALLOF_PLAYER);

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
