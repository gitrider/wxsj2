#include "stdafx.h"
#include "SSScenePlayerCount.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"

UINT SSScenePlayerCountHandler::Execute(SSScenePlayerCount* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION	
	
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			Assert(pPacket);

			INT count = pPacket->GetChangeCount() ;
			if( count==0 )
			{//��ҳ�ˢ��

				ID_t ServerID = pPacket->GetServerID() ;
				INT PlayerPoolSize = pPacket->GetPlayerPoolSize() ;
				g_pServerManager->SetServerPlayerPoolSize( ServerID, PlayerPoolSize ) ;

				g_pLog->FastSaveLog( LOG_FILE_0, "SSScenePlayerCountHandler ServerID=%d PlayerPoolSize=%d OK ", 
					ServerID, PlayerPoolSize ) ;
			}
			else
			{
				//�޸�g_WorldPlayerCounter ������
				g_WorldPlayerCounter.m_WorldPlayerCount = pPacket->GetTotalPlayerCount();
				g_pWorldPlayerQueue->ResetSendCount();
				
				//�޸ĳ�����������
				for( INT i=0; i<count; i++ )
				{
					SceneID_t sceneid = pPacket->GetChangeSceneID(i) ;
					INT playercount = pPacket->GetChangeScenePlayerCount(i) ;
					g_pServerManager->SetScenePlayerCount(sceneid,playercount) ;
				}

				g_pLog->FastSaveLog(LOG_FILE_0,"SSScenePlayerCountHandler::Execute()....OK,WorldPlayerCount = %d",
					g_WorldPlayerCounter.m_WorldPlayerCount);

				g_pProcessManager->SendPacket(pPacket,INVALID_ID);
				return PACKET_EXE_NOTREMOVE;
			}
		}
		else if (CurrentThreadID == g_pProcessPlayerManager->m_ThreadID )
		{
			Assert(pPacket);
			if(0 != pPacket->GetChangeCount())
			{
				//�޸�g_WorldPlayerCounter1 ������
				g_WorldPlayerCounter1.m_WorldPlayerCount = pPacket->GetTotalPlayerCount();

				g_pLog->FastSaveLog(LOG_FILE_0,"SSScenePlayerCountHandler::Execute()....OK,WorldPlayerCount1 = %d",
					g_WorldPlayerCounter1.m_WorldPlayerCount);
			}
		}
		else
		{
			AssertEx(FALSE,"SSScenePlayerCountHandler �߳���Դִ�д���!");
		}
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION	

		return PACKET_EXE_ERROR;
}