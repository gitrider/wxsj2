#include "stdafx.h" 
#include "LCRetDeleteChar.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT LCRetDeleteCharHandler::Execute(LCRetDeleteChar* pPacket, Player* pPlayer )
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
			//����˺��Ƿ���ȷ
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount()) != 0)
			{
				g_pLog->FastSaveLog(LOG_FILE_0,"LCRetDeleteCharHandler::Execute() ....OK acc=%s,packacc=%s",
					pLoginPlayer->GetAccount(),pPacket->GetAccount());
				return PACKET_EXE_CONTINUE;
			}
			pLoginPlayer->SendPacket(pPacket);
			g_pLog->FastSaveLog(LOG_FILE_0,"LCRetDeleteCharHandler::Execute() ....OK acc=%s",
				pLoginPlayer->GetAccount());
		
		}
		else
		{
			AssertEx(FALSE,"LCRetDeleteCharHandler �߳���Դִ�д���!");
		}

		return PACKET_EXE_CONTINUE;

		

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}