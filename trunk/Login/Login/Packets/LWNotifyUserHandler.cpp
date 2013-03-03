#include "stdafx.h" 
#include "LWNotifyUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT	LWNotifyUserHandler::Execute(LWNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		Assert(pPacket);
		ServerPlayer* pServerPlayer  = (ServerPlayer*)pPlayer;
		Assert(pServerPlayer);
		pServerPlayer->SendPacket(pPacket);

		g_pLog->FastSaveLog(LOG_FILE_0, "LWNotifyUserHandler ServerManager Send LWNotifyUser(%s) to world! Success!",
			pPacket->GetAccount());
	}
	else
	{
		AssertEx(FALSE,"LWNotifyUserHandler 线程资源执行错误!");
	}
	g_pLog->FastSaveLog(LOG_FILE_0,"LWNotifyUserHandler::Execute() ....OK");

	return PACKET_EXE_CONTINUE;
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}


