#include "stdafx.h" 
#include "CLAskLoginCode.h"
#include "LCRetLoginCode.h"

#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetConnect.h"
#include "Log.h"
#include "TimeManager.h"

UINT CLAskLoginCodeHandler::Execute(CLAskLoginCode* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//检查线程执行资源是否正确
	TID CurrentThreadID = MyGetCurrentThreadID();
	if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID)
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);
		Assert(pPacket);

		switch(pPacket->GetAskType())
		{
		case LOGINCODE_ASK:
		case LOGINCODE_REFRESH:
			{
				//给客户端发送密码图
				pLoginPlayer->SendLoginCode();
			}
			break;
		case LOGINCODE_SEND:
			{
				//验证客户端发送来的答案
				CreateCode* pAnswer = pPacket->GetAnswer();
				Assert(pAnswer);

				BOOL bOk = pLoginPlayer->m_LoginCode.IsSame(pAnswer);
				if(TRUE == bOk)
				{
					LCRetLoginCode	rmsg;
					rmsg.SetRetType(RETLOGINCODE_SUCCESS);
					pLoginPlayer->SendPacket(&rmsg);

					pLoginPlayer->m_bAlreadyCheckLoginCode = TRUE;
				}
				else
				{
					LCRetLoginCode	rmsg;
					rmsg.SetRetType(RETLOGINCODE_FAIL);
					pLoginPlayer->SendPacket(&rmsg);

					//验证失败后再给玩家发送新的密码图....
					pLoginPlayer->SendLoginCode();
				}
			}
			break;
		default:
			Assert(FALSE);
		}

		g_pLog->FastSaveLog(LOG_FILE_0, "CLAskLoginCodeHandler::Execute(...)...OK!!! acc=%s",pLoginPlayer->GetAccount()) ;
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR::CLAskLoginCodeHandler::Execute()...Fail, Invalid Thread Handler,acc = %s",
			pLoginPlayer->GetAccount()) ;
		return PACKET_EXE_CONTINUE;
	}
	

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
