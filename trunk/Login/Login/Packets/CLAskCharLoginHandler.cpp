#include "stdafx.h" 
#include "CLAskCharLogin.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "LCRetCharLogin.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "LWAskCharLogin.h"
#include "ServerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"
#include "DBThreadManager.h"
#include "CharConfig.h"



UINT CLAskCharLoginHandler::Execute(CLAskCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	
	if(CurrentThreadID== g_pProcessPlayerManager->m_ThreadID)
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			return PACKET_EXE_CONTINUE;
		}
		
		UINT uTime = g_pTimeManager->CurrentTime();
		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			//用户操作过于频繁
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_OP_TIMES);
			pLoginPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....Fails!,code = ASKCHARLOGIN_OP_TIMES,acc=%s,guid=%X",
				pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;
			return PACKET_EXE_CONTINUE;
		}
		GUID_t	TryLoginGuid = pPacket->GetCharGuid();
		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//用户操作非法,用户不是请求角色的所有者
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_NOT_OWNER);
			pLoginPlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....Fails!,code = ASKCHARLOGIN_NOT_OWNER,acc=%s,guid=%X",
				pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;
			return PACKET_EXE_CONTINUE;
		}

		//// 检查是否已经通过登陆图形验证....
		//if ( pLoginPlayer->m_bAlreadyCheckLoginCode == FALSE )
		//{
		//	LCRetCharLogin Msg;
		//	Msg.SetResult(ASKCHARLOGIN_VALIDATE_ERROR);
		//	pLoginPlayer->SendPacket(&Msg);
		//	g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....Fails!, code = ASKCHARLOGIN_VALIDATE_ERROR,acc=%s,guid=%X",
		//		pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;

		//	return PACKET_EXE_CONTINUE;
		//}

		//检查是否是合法SID
		
		if(g_DBSceneTable.isValidSceneID(pPacket->GetSceneID(),CT_NORMAL))
		{
			pLoginPlayer->SetChooseSceneID(pPacket->GetSceneID());
		}
		pLoginPlayer->SetUserKey(rand());
		pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		{   //加入成功，将消息发送到DB处理
			//将修改数据库操作时间的步骤移动到实际数据库操作进行的部分
			return PACKET_EXE_NOTREMOVE;	
		}
		else
		{//DB 压力过大，让用户重新尝试
			LCRetCharLogin Msg;
			Msg.SetResult(ASKCHARLOGIN_SERVER_BUSY);
			pLoginPlayer->SendPacket(&Msg);
			pLoginPlayer->m_LastDBOpTime = uTime;
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....Fails!, code = ASKCHARLOGIN_SERVER_BUSY,acc=%s,guid=%X",
				pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;

			return PACKET_EXE_CONTINUE;
		}
	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{//将玩家加入到World 队列中
		
		PlayerID_t	PlayerID			= pPacket->GetPlayerID();
		GUID_t	    PlayerCharGUID		= pPacket->GetCharGuid();
		SceneID_t	PlayerSceneID			= pPacket->GetSceneID();

		UINT        QueuePos;

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		GUID_t	TryLoginGuid = pPacket->GetCharGuid();
		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//一次错误的数据操作
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler Wrong operation,acc=%s,guid=%X",
				pLoginPlayer->GetAccount(),pPacket->GetCharGuid()) ;
			return PACKET_EXE_CONTINUE;
		}
	
		/*[12/19/2006]
		 *	注意，这里LoginPlayer加入到WorldQueue中处理，但还在TurnPlayerQueue中，
		 *	并且归ProcessPlayerManager管理。
		 */
		if(g_pWorldPlayerQueue->AddInPlayer(PlayerID,
			pLoginPlayer->GetAccount(),
			pPacket->GetCharGuid(),
			QueuePos))
		{ //加入成功
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_WORLD_PROCESS_TURN);
			pLoginPlayer->SetLastSendTurnTime(g_pTimeManager->CurrentTime());
			//pLoginPlayer->SetQueuePos(0);
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....OK!, acc= %s guid = %X ",
				pLoginPlayer->GetAccount(),
				PlayerCharGUID ) ;

			return PACKET_EXE_CONTINUE;
			
		}
		else //加入失败
		{
			//queue 都满了,World爆满 ,但是不应该出现，因为World状态已经通知过Login
			//并且不是所有玩家都在排队
			g_pLog->FastSaveLog(LOG_FILE_0, "CLAskCharLoginHandler::Execute()....Fail,World is Full,acc=%s,GUID = %X ",
				pLoginPlayer->GetAccount(),
				PlayerCharGUID ) ;
		}
		
	}
	else
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR::CLAskCharLoginHandler::Execute()...Fail, Invalid Thread Handler,acc = %s",
					pLoginPlayer->GetAccount()) ;
		return PACKET_EXE_CONTINUE;
	}
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
