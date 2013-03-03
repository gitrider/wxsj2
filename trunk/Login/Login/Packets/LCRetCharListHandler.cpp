#include "stdafx.h" 
#include "LCRetCharList.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT LCRetCharListHandler::Execute(LCRetCharList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(MyGetCurrentThreadID() == g_pProcessPlayerManager->m_ThreadID)
	{
		LoginPlayer*	pLoginPlayer = (LoginPlayer*)pPlayer ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}

		//���GUID �Ƿ���ȷ
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCharListHandler::Execute() ....Fails,packacc=%s,acc=%s",
				pPacket->GetAccount(),pLoginPlayer->GetAccount());
			return PACKET_EXE_CONTINUE;
		}

		pLoginPlayer->SendPacket(pPacket);
	}
	else
	{
		AssertEx(FALSE,"LCRetCharListHandler �߳���Դִ�д���!");
	}
		
	g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCharListHandler::Execute() ....OK");

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}