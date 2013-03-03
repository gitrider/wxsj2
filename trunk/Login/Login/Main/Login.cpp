#include "stdafx.h" 
#include "Login.h"
#include "TimeManager.h"
#include "Log.h"
#include "LoginDBManager.h"
#include "Config.h"
#include "ConnectManager.h"
#include "ThreadManager.h"
#include "ProcessManager.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "TurnPlayerQueue.h"
#include "DBThreadManager.h"
#include "LoginTable.h"
//#include "captcha_generator.h"
//#include "captcha_font_file.h"
#include "IpTable.h"

//using namespace CAPTCHA;

#if __LINUX__
#include <signal.h>
#endif

Login	g_Login;
//Generator*					g_pGenerator=NULL ;

Login::Login() : m_bExited(FALSE)
{
#if defined(__WINDOWS__)
	//Windows初始化Socket
	WORD wVersionRequested;
	WSADATA wsaData;
	INT err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData ); 
#endif
}

Login::~Login()
{
#if defined(__WINDOWS__)
	//Windows清理Socket
	WSACleanup();
#endif
}

BOOL	Login::Init()
{
	__ENTER_FUNCTION

	
	BOOL	bRet = FALSE;

	bRet = g_Config.Init( ) ;
	if(!bRet)
	{
		Assert(bRet);
		return FALSE;
	}

	bRet  = NewLogin();
	if(!bRet)
	{
		Assert(bRet);
		return FALSE;
	}

	bRet = InitLogin();
	if(!bRet)
	{
		Assert(bRet);
		return FALSE;
	}

	m_bExited = FALSE;
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Login::Loop()
{
	__ENTER_FUNCTION

	BOOL bRet = FALSE ;

	g_pLog->SaveLog(LOGIN_LOGFILE, "\r\nLoop..." ) ;

	
	//连接客户端线程
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pConnectManager->start( )..." ) ;
	g_pConnectManager->start();


	//连接World 和 BillingSystem 的线程
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pThreadManager->Start( )..." ) ;
	bRet = g_pThreadManager->Start( ) ;
	Assert( bRet ) ;
	
	//数据库处理
	//g_pLog->SaveLog(LOGIN_LOGFILE, "g_pDBManager->Start( )..." ) ;
	//g_pDBManager->start();
	
	g_pLog->SaveLog(LOGIN_LOGFILE,"g_pDBThreadManager->Start() ...");
	g_pDBThreadManager->Start();


	//主线程资源留给 ProcessManager
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pProcessManager->Loop( )..." ) ;
	g_pProcessManager->start();

	

	//守护线程
	while(TRUE)
	{
		UINT uDayTime = g_pTimeManager->Time2Day( ) ;
		UINT uTime = g_pTimeManager->CurrentTime() ;//当前时间

		if(!m_FlushLogTimer.IsSetTimer())
		{
			m_FlushLogTimer.BeginTimer(2000,uTime);
		}
		
		if( g_FileNameFix != uDayTime )
		{//设置日志文件名后缀的变化
			g_FileNameFix = uDayTime ;
		}

		//日志刷入文件
		_MY_TRY
		{
			if( m_FlushLogTimer.CountingTimer(uTime) )
			{
				g_pLog->FlushLog_All();
			}
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		//定时清除重连锁[2007/12/03 YangJun]
		if(!g_DBConnectThread)
		{
			if(m_ClearConnectThreadTimer.IsSetTimer())
				m_ClearConnectThreadTimer.CleanUp();
		}
		else
		{
			_MY_TRY
			{
				if(!m_ClearConnectThreadTimer.IsSetTimer())
				{
					//2分钟还未重连完成，清除
					m_ClearConnectThreadTimer.BeginTimer(2*60*1000,uTime);
				}
				
				//2分钟还未重连完成，清除
				if(m_ClearConnectThreadTimer.CountingTimer(uTime))	g_DBConnectThread = 0;
			}
			_MY_CATCH
			{
				SaveCodeLog();
			}
		}

		MySleep(100);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Login::Exit()
{
	__ENTER_FUNCTION

	
	g_pLog->SaveLog(LOGIN_LOGFILE, "Begin delete..." ) ;

	//SAFE_DELETE(g_pDBManager);
	//g_pLog->SaveLog(LOGIN_LOGFILE, "g_pDBManager delete...OK" ) ;

	SAFE_DELETE(g_pProcessManager);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pProcessManager delete...OK" ) ;

	SAFE_DELETE(g_pPlayerPool);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pPlayerPool delete...OK" ) ;
	

	SAFE_DELETE(g_pPacketFactoryManager);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;

	SAFE_DELETE(g_pProcessPlayerQueue);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pProcessPlayerQueue delete...OK" ) ;
	
	SAFE_DELETE(g_pWorldPlayerQueue);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pWorldPlayerQueue delete...OK" ) ;
	
	SAFE_DELETE(g_pThreadManager);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pThreadManager delete...OK" ) ;
	
	SAFE_DELETE(g_pServerManager);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pServerManager delete...OK" ) ;
	
	SAFE_DELETE(g_pConnectManager);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pConnectManager delete...OK" ) ;
	
	SAFE_DELETE(g_pDBThreadManager);
	g_pLog->SaveLog(LOGIN_LOGFILE,"g_pDBThreadManager delete ... OK");

	//SAFE_DELETE(g_pGlobalCaptchaFontFile);
	//g_pLog->SaveLog(LOGIN_LOGFILE,"g_pGlobalCaptchaFontFile delete ... OK");

	SAFE_DELETE(g_pIPRegionTable);
	g_pLog->SaveLog(LOGIN_LOGFILE,"g_pIPRegionTable delete ... OK");

	//SAFE_DELETE(g_pGenerator);
	//g_pLog->SaveLog(LOGIN_LOGFILE,"g_pGenerator delete ... OK");

	g_pLog->SaveLog(LOGIN_LOGFILE, "End delete..." ) ;

	m_bExited = TRUE;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}


/*
*	分配空间相关
*/
BOOL	Login::NewLogin()
{
	__ENTER_FUNCTION

	//g_pDBManager	=	 new	LoginDBManager();
	//AssertEx(g_pDBManager,"分配数据库管理器失败!");
	//g_pLog->SaveLog(LOGIN_LOGFILE,"new LoginDBManager ...OK");

	g_pProcessManager	=	 new  ProcessManager();
	AssertEx( g_pProcessManager,"分配g_pProcessManager 失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE, "new ProcessManager...OK" ) ;
	
	g_pPlayerPool		=	new	  PlayerPool ;
	AssertEx( g_pPlayerPool,"分配g_pPlayerPool 失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE, "new PlayerPool...OK" ) ;

	g_pPacketFactoryManager = new PacketFactoryManager ;
	AssertEx( g_pPacketFactoryManager,"分配g_pFacketFactoryManager 失败!") ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "new PacketFactoryManager...OK" ) ;
	
	g_pBPacketFactoryManager = new BillPacketFactoryManager ;
	AssertEx( g_pBPacketFactoryManager,"分配g_pBFacketFactoryManager 失败!") ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "new BillPacketFactoryManager...OK" ) ;

	g_pProcessPlayerQueue	= new TurnPlayerQueue;
	AssertEx(g_pProcessPlayerQueue,"分配排队队列失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE,"new g_pProcessPlayerQueue...OK");


	g_pWorldPlayerQueue	= new WorldPlayerQueue;
	AssertEx(g_pWorldPlayerQueue,"分配排队队列失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE,"new g_pWorldPlayerQueue...OK");


	g_pThreadManager	=	 new ThreadManager();
	AssertEx( g_pThreadManager,"分配g_pThreadManager 失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE, "new ThreadManager...OK" ) ;

	g_pServerManager    =	 new ServerManager ;
	AssertEx( g_pServerManager,"分配g_pServerManager 失败!" ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "new ServerManager...OK" ) ;

	g_pDBThreadManager	=	new DBThreadManager;
	AssertEx(g_pDBThreadManager,"分配g_pDBThreadManager 失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE, "new DBThreadManager...OK" );

	g_pConnectManager   =    new ConnectManager ;
	AssertEx( g_pConnectManager,"分配g_pConnectManager 失败!");
	g_pLog->SaveLog(LOGIN_LOGFILE, "new ConnectManager...OK" ) ;

	//g_pGlobalCaptchaFontFile = new FontFile ;
	//Assert( g_pGlobalCaptchaFontFile ) ;
	//g_pLog->SaveLog(LOGIN_LOGFILE, "new FontFile...OK" ) ;

	g_pIPRegionTable = new IPRegionTable;
	Assert(g_pIPRegionTable);
	g_pIPRegionTable->Init();
	g_pLog->SaveLog(LOGIN_LOGFILE, "new IPRegionTable...OK");

	g_DefaultCharDataTbl.Init();

	//g_pGenerator = new Generator ;
	//Assert( g_pGenerator ) ;
	//g_pLog->SaveLog(LOGIN_LOGFILE, "new Generator...OK" ) ;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

/*
*	数据赋值相关
*/
BOOL	Login::InitLogin()
{
	__ENTER_FUNCTION

	BOOL	bRet = FALSE;
	
	//DB 的初始化,连接数据库
	//Assert(g_pDBManager);
	//bRet = g_pDBManager->Init();
	//Assert(bRet);
	//g_pLog->SaveLog(LOGIN_LOGFILE,"g_pDBManager->Init()...OK");


	bRet = g_pPlayerPool->Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;

	//
	bRet = g_pPacketFactoryManager->Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;
	
	bRet = g_pBPacketFactoryManager->Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pBPacketFactoryManager->Init()...OK" ) ;
	bRet = g_pProcessPlayerQueue->Init();
	Assert(bRet);
	g_pLog->SaveLog(LOGIN_LOGFILE,"g_pTurnPlayerQueue->Init() ... OK");

	bRet = g_pWorldPlayerQueue->Init();
	Assert(bRet);
	g_pLog->SaveLog(LOGIN_LOGFILE,"g_pWorldPlayerQueue->Init() ... OK");

	bRet = g_pDBThreadManager->Init();
	Assert(bRet);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pDBThreadManager->Init()...OK" ) ;
	
	//对客户端网络管理类的初始化
	bRet = g_pConnectManager->Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pConnectManager->Init()...OK" ) ;
	
	// 登录流程管理器
	bRet	= g_pProcessManager->Init();
	Assert(bRet);
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pProcessManager->Init()...OK" ) ;

	// 线程管理器
	bRet = g_pThreadManager->Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pThreadManager->Init()...OK" ) ;

	//g_pGlobalCaptchaFontFile->openFromFile( FILE_LOGIN_FONT ) ;
	//g_pGenerator->setFontFile( g_pGlobalCaptchaFontFile ) ;
	//g_pLog->SaveLog(LOGIN_LOGFILE, "g_pGenerator->setFontFile()...OK" ) ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

VOID	Login::Stop()
{
	__ENTER_FUNCTION

		if( g_pConnectManager )
		{
			g_pConnectManager->stop( ) ;
		}

	__LEAVE_FUNCTION
}


LoginExceptionHandler::LoginExceptionHandler()
{
#ifdef __LINUX__
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, SEGHandler);
	signal(SIGFPE,  FPEHandler);
	signal(SIGILL,  ILLHandler);
	signal(SIGINT,  INTHandler);  
	signal(SIGTERM, TERMHandler); 
	signal(SIGABRT, ABORTHandler);
	signal(SIGXFSZ, XFSZHandler);
#endif
}

VOID LoginExceptionHandler::INTHandler(INT)
{
	DumpStack("INT exception:\r\n");
	exit(0);
}
VOID LoginExceptionHandler::TERMHandler(INT)
{
	DumpStack("TERM exception:\r\n");
	exit(0);

}
VOID LoginExceptionHandler::ABORTHandler(INT)
{
	DumpStack("ABORT exception:\r\n");
	exit(0);
}

VOID LoginExceptionHandler::ILLHandler(INT)
{
	DumpStack("ILL exception:\r\n");
	exit(0);
}

VOID LoginExceptionHandler::FPEHandler(INT)
{
	DumpStack("FPE exception:\r\n");
	exit(0);
}

VOID LoginExceptionHandler::SEGHandler(INT)
{
	DumpStack("SEG exception:\r\n");
	exit(0);
}
VOID LoginExceptionHandler::XFSZHandler(INT)
{
	DumpStack("XFSZ exception:\r\n");
	exit(0);
}

LoginExceptionHandler g_LoginExceptionHandler;
