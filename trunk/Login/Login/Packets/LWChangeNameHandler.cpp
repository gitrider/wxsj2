#include "stdafx.h" 
#include "LWChangeName.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT LWChangeNameHandler::Execute(LWChangeName* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{

		Assert(pPacket);
		
		ServerPlayer* pServerPlayer  = (ServerPlayer*)pPlayer;
		
		Assert(pServerPlayer);

		pServerPlayer->SendPacket(pPacket);
	}
	else
	{
		AssertEx(FALSE,"LWChangeNameHandler 线程资源执行错误!");
	}
	g_pLog->FastSaveLog(LOG_FILE_0,"LWChangeNameHandler::Execute() ....OK");

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}