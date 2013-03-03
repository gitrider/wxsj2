#include"stdafx.h"

#include "GCBankMoney.h"
#include "CGBankMoney.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "GIException.h"
uint GCBankMoneyHandler::Execute( GCBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE IsSave = pPacket->GetIsSave();
			INT	 Amount = pPacket->GetAmount();
			INT CurMoney = CDataPool::GetMe()->UserBank_GetBankMoney();
			CHAR szMsg[256];
			if(IsSave == CGBankMoney::SAVE_MONEY)
			{
				CurMoney += Amount;
				//通知玩家交易成功

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankMoneyHandler_Info_Save_Money");
				_snprintf(szMsg, 256, strTemp.c_str());
				//_snprintf(szMsg, 256, "存入金钱成功");

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave == CGBankMoney::PUTOUT_MONEY)
			{
				CurMoney -= Amount;
				//通知玩家交易成功		

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCBankMoneyHandler_Info_Load_Money");
				_snprintf(szMsg, 256, strTemp.c_str());
				//_snprintf(szMsg, 256, "取出金钱成功");

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave == CGBankMoney::UPDATE_MONEY)
			{//进入场景时由服务器刷过来，银行中的金钱数
				CurMoney = Amount;

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
