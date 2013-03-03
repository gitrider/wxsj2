#include "stdafx.h" 
#include "LoginMain.h"
#include "TimeManager.h"
#include "Log.h"
#include "Config.h"
#include "Login.h"

#if defined(__WINDOWS__)
#include "Minidump.h"
char * g_dump_exe_name = "Login";
#endif

int	g_SingleDBConnection = 0;	//是否只建立3个DB连接

INT main(INT argc, CHAR* argv[])
{
#if defined(__WINDOWS__)
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	__ENTER_FUNCTION
	
		if( argc>1 )
		{
			for( int i=1; i<argc; i++ )
			{
				if( strcmp(argv[i],"-ignoreassert")==0 )
				{
					g_Command_Assert=1 ;
				}
				else if( strcmp(argv[i],"-retryassert")==0 )
				{
					g_Command_Assert=2 ;
				}

				if( strcmp(argv[i],"-ignoremessagebox")==0 )
				{
					g_Command_IgnoreMessageBox=TRUE ;
				}

				if( strcmp(argv[i],"-singledb")==0 )
				{
					g_SingleDBConnection=1;
				}
			}
		}	
		
		
	//时间管理器
	g_pTimeManager	=	new TimeManager;
	g_pTimeManager->Init();
	g_pLog = new Log ;
	Assert( g_pLog ) ;
	BOOL ret = g_pLog->Init( ) ;
	Assert(ret) ;

	g_pLog->SaveLog(LOGIN_LOGFILE, "\r\n(###) main..." ) ;
	g_pTimeManager->SetTime( ) ;
		
	g_pLog->SaveLog(LOGIN_LOGFILE, "Login Starting... (%.10d)(%d)",
	g_pTimeManager->Time2DWORD(),
	g_pTimeManager->StartTime() ) ;
	srand(g_pTimeManager->CurrentTime());
	BOOL	bRet  =		g_Login.Init();
	Assert(bRet);
		
	bRet	=			g_Login.Loop();	
	Assert(bRet);

	bRet	=			g_Login.Exit();
	Assert(bRet);

	return	0;

	__LEAVE_FUNCTION

	return -1;
}





