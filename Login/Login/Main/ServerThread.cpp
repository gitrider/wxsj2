#include "stdafx.h" 

#include "ServerThread.h"
#include "ServerManager.h"
#include "Log.h"
#include "TimeManager.h"



ServerThread::ServerThread( )
{
__ENTER_FUNCTION

	m_Active = TRUE ;

__LEAVE_FUNCTION
}

ServerThread::~ServerThread( )
{
__ENTER_FUNCTION




__LEAVE_FUNCTION
}

VOID ServerThread::run( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ServerThread::run() Thread ID:%d LoginID:%d...", 
			getTID(), g_pServerManager->GetLoginID() ) ;

		srand(g_pTimeManager->CurrentTime());
		g_pServerManager->m_ThreadID = getTID() ;

		while( IsActive() )
		{
			if( g_pServerManager )
			{
				MySleep(100);
				BOOL ret = g_pServerManager->Tick( ) ;
				Assert( ret ) ;
			}

#ifdef _EXEONECE
			static INT ic=_EXEONECE ;
			ic-- ;
			if( ic<=0 )
			{
				g_pLog->FastSaveLog(LOG_FILE_0, "ServerThread::Exit() Thread ID:%d ServerID:%d...", 
					getTID(), g_pServerManager->GetServerID() ) ;
				return ;
			}
#endif
		}
	}
	_MY_CATCH
	{
	}


__LEAVE_FUNCTION
}
