#include "stdafx.h" 
#include "LWAskCharLogin.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT LWAskCharLoginHandler::Execute(LWAskCharLogin* pPacket, Player* pPlayer )
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
			AssertEx(FALSE,"LWAskCharLoginHandler �߳���Դִ�д���!");
		}
		g_pLog->FastSaveLog(LOG_FILE_0,"LWAskCharLoginHandler::Execute() ....OK");

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}