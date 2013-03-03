#include "stdafx.h" 
#include "CLAskCreateCode.h"
#include "LCRetCreateCode.h"

#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetConnect.h"
#include "Log.h"
#include "TimeManager.h"

UINT CLAskCreateCodeHandler::Execute(CLAskCreateCode* pPacket, Player* pPlayer )
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
		case CREATECODE_ASK:
		case CREATECODE_REFRESH:
			{
				//给客户端发送密码图
				pLoginPlayer->SendCreateCode();
			}
			break;
		case CREATECODE_SEND:
			{
				//验证客户端发送来的答案
				CreateCode* pAnswer = pPacket->GetAnswer();
				Assert(pAnswer);

				BOOL bOk = pLoginPlayer->m_CreateCode.IsSame(pAnswer);
				if(TRUE == bOk)
				{
					LCRetCreateCode	rmsg;
					rmsg.SetRetType(RETCREATECODE_SUCCESS);
					pLoginPlayer->SendPacket(&rmsg);

					pLoginPlayer->m_bAlreadyCheckCreateCode = TRUE;
				}
				else
				{
					LCRetCreateCode	rmsg;
					rmsg.SetRetType(RETCREATECODE_FAIL);
					pLoginPlayer->SendPacket(&rmsg);

					//验证失败后再给玩家发送新的密码图....
					pLoginPlayer->SendCreateCode();
				}
			}
			break;
		default:
			Assert(FALSE);
		}

		g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCreateCodeHandler::Execute(...)...OK!!! acc=%s",pLoginPlayer->GetAccount()) ;
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR::CLAskCreateCodeHandler::Execute()...Fail, Invalid Thread Handler,acc = %s",
			pLoginPlayer->GetAccount()) ;
		return PACKET_EXE_CONTINUE;
	}
	

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION
	return PACKET_EXE_ERROR;
}
