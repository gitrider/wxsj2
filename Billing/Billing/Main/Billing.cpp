#include "stdafx.h"
#include "Billing.h"
#include "TimeManager.h"
#include "Log.h"
#include "Config.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "AccountTable.h"

#if defined(__WINDOWS__)
#include "Minidump.h"
char * g_dump_exe_name = "Billing";
#endif

Billing			g_Billing ;

int main(int argc, char* argv[])
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
				if( strcmp(argv[i],"-retryassert")==0 )
				{
					g_Command_Assert=2 ;
				}
				else if( strcmp(argv[i],"-ignoremessagebox")==0 )
				{
					g_Command_IgnoreMessageBox=TRUE ;
				}
			}
		}


		BOOL bRet ;

		//时间模块需要最开始的时候设置
		g_pTimeManager = new TimeManager ;
		Assert( g_pTimeManager ) ;
		g_pTimeManager->Init( ) ;
		g_pLog = new Log ;
		Assert( g_pLog ) ;
		BOOL ret = g_pLog->Init( ) ;
		Assert(ret) ;
		g_pLog->SaveLog(BILLING_LOGFILE, "(###) main..." ) ;
		g_pLog->SaveLog(BILLING_LOGFILE, "Billing Start... " ) ;



		bRet = g_Billing.Init( ) ;
		if( bRet==FALSE )
		{
			Assert(FALSE) ;
			return 1 ;
		}

		bRet = g_Billing.Loop( ) ;
		if( bRet==FALSE )
		{
			Assert(FALSE) ;
			return 1 ;
		}

		bRet = g_Billing.Exit( ) ;
		if( bRet==FALSE )
		{
			Assert(FALSE) ;
			return 1 ;
		}

		__LEAVE_FUNCTION

			return 0 ;
}

Billing::Billing( )
{
	__ENTER_FUNCTION

#if defined(__WINDOWS__)
		WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData ); 
#endif


	__LEAVE_FUNCTION
}

Billing::~Billing( )
{
	__ENTER_FUNCTION

#if defined(__WINDOWS__)
		WSACleanup();
#endif

	__LEAVE_FUNCTION
}

BOOL Billing::Init( )
{
	__ENTER_FUNCTION

	BOOL bRet ;

	g_pLog->SaveLog(BILLING_LOGFILE, "Start Read Config Files..." ) ;
	bRet = g_Config.Init( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "Read Config Files...OK!" ) ;

	g_pLog->SaveLog(BILLING_LOGFILE, "Start New Managers..." ) ;
	bRet = NewStaticManager( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "New Managers...OK!" ) ;

	g_pLog->SaveLog(BILLING_LOGFILE, "Start Init Managers..." ) ;
	bRet = InitStaticManager( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "Init Managers...OK!" ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Billing::Loop( )
{
	__ENTER_FUNCTION

	g_pLog->SaveLog(BILLING_LOGFILE, "Loop..." );
	g_pServerManager->Loop();

	__LEAVE_FUNCTION

		return TRUE ;
}

BOOL Billing::Exit( )
{
	__ENTER_FUNCTION

		BOOL bRet ;

	g_pLog->SaveLog(BILLING_LOGFILE, "Start Exit..." ) ;
	bRet = DelStaticManager( ) ;
	Assert( bRet ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "Exit...OK!" ) ;


	__LEAVE_FUNCTION

		return TRUE ;
}

BOOL Billing::NewStaticManager( )
{
	__ENTER_FUNCTION

		//
	g_pServerManager = new ServerManager ;
	Assert( g_pServerManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "new ServerManager...OK" ) ;
	
	//
	g_pPlayerPool = new PlayerPool ;
	Assert( g_pPlayerPool ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "new PlayerPool...OK" ) ;

	//
	g_pPacketFactoryManager = new PacketFactoryManager ;
	Assert( g_pPacketFactoryManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "new PacketFactoryManager...OK" ) ;

	//
	g_pBPacketFactoryManager = new BillPacketFactoryManager ;
	Assert( g_pBPacketFactoryManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "new g_pBPacketFactoryManager...OK" ) ;

	
	__LEAVE_FUNCTION

		return TRUE ;
}

BOOL Billing::InitStaticManager( )
{
	__ENTER_FUNCTION

	BOOL ret ;
	
	ret = g_Account.Init();
	Assert(ret);
	g_pLog->SaveLog(BILLING_LOGFILE, "g_Account->Init()...OK" ) ;

	//
	ret = g_pServerManager->Init( ) ;
	Assert( ret ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pServerManager->Init()...OK" ) ;
	//
	ret = g_pPlayerPool->Init( ) ;
	Assert( ret ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;
	
	//
	ret = g_pPacketFactoryManager->Init( ) ;
	Assert( ret ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;

	ret = g_pBPacketFactoryManager->Init( ) ;
	Assert( ret ) ;
	g_pLog->SaveLog(LOGIN_LOGFILE, "g_pBPacketFactoryManager->Init()...OK" ) ;
	
	__LEAVE_FUNCTION

		return TRUE ;
}

BOOL Billing::DelStaticManager( )
{
	__ENTER_FUNCTION


	SAFE_DELETE( g_pTimeManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pTimeManager delete...OK" ) ;
	//
	//以下模块放后面删除
	//
	SAFE_DELETE( g_pPacketFactoryManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;

	SAFE_DELETE( g_pBPacketFactoryManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pBPacketFactoryManager delete...OK" ) ;
	
	SAFE_DELETE( g_pPlayerPool ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pPlayerPool delete...OK" ) ;
	
	SAFE_DELETE( g_pServerManager ) ;
	g_pLog->SaveLog(BILLING_LOGFILE, "g_pServerManager delete...OK" ) ;

	__LEAVE_FUNCTION

		return TRUE ;
}