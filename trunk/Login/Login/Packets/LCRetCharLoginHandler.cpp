#include "stdafx.h" 
#include "LCRetCharLogin.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	if(MyGetCurrentThreadID() == g_pProcessPlayerManager->m_ThreadID)
	{	//��DB->ProcessPlayerManager �������ݲ������
		LoginPlayer*	pLoginPlayer = (LoginPlayer*)pPlayer ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
		pLoginPlayer->SendPacket(pPacket);
		g_pLog->FastSaveLog(LOG_FILE_0,"LCRetCharLoginHandler::Execute() ....OK,acc=%s",
			pLoginPlayer->GetAccount());
	}
	else
	{
		AssertEx(FALSE,"LCRetCharLoginHandler �߳���Դִ�д���!");
	}
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}