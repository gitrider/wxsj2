

#include "stdafx.h"
#include "World.h"
#include "TimeManager.h"
#include "Log.h"
#include "ServerManager.h"
#include "Config.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "OnlineUser.h"
#include "Team.h"
#include "ChatCenter.h"
#include "WorldTable.h"
#include "GuildManager.h"
#include "CityManager.h"
#include "CountryManager.h"
#include "TopListManager.h"
#include "TeamRecruitManager.h"

#include "MailPool.h"
#include "MailCenter.h"
#include "AllUser.h"
#include "SceneInfo.h"
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "WorldTimeInfo.h"
#include "Version.h"
#include "World.h"

#if defined(__WINDOWS__)
#include "Minidump.h"
char * g_dump_exe_name = "World";
#endif

World g_World ;


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
			else if( strcmp(argv[i],"-onelog")==0 )
			{
				extern BOOL g_Log_AllInOne;
				g_Log_AllInOne=TRUE ;
			}
		}
	}


	BOOL bRet ;

	//时间模块需要最开始的时候设置
	g_pTimeManager = new TimeManager ;
	Assert( g_pTimeManager ) ;
	g_pTimeManager->Init( ) ;
	g_pTimeManager->CurrentDate();

	Log::SaveLog( WORLD_LOGFILE, "World Server Start... " ) ;
	Log::SaveLog( WORLD_LOGFILE, "World Server Current VERSION %x\n\n", CURRENT_VERSION );
	
	


	bRet = g_World.Init( ) ;
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}
	
	bRet = g_World.Loop( ) ;
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}

	bRet = g_World.Exit( ) ;
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}

__LEAVE_FUNCTION

	return 0 ;
}


World::World( )
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

World::~World( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	WSACleanup();
#endif

__LEAVE_FUNCTION
}

BOOL World::Init( )
{
__ENTER_FUNCTION

	BOOL bRet ;

	Log::SaveLog( WORLD_LOGFILE, "Start Read Config Files..." ) ;
	bRet = g_Config.Init( ) ;
	Assert( bRet ) ;
	Log::SaveLog( WORLD_LOGFILE, "Read Config Files...OK!" ) ;

	Log::SaveLog( WORLD_LOGFILE, "Start New Managers..." ) ;
	bRet = NewStaticManager( ) ;
	Assert( bRet ) ;
	Log::SaveLog( WORLD_LOGFILE, "New Managers...OK!" ) ;

	Log::SaveLog( WORLD_LOGFILE, "Start Init Managers..." ) ;
	bRet = InitStaticManager( ) ;
	Assert( bRet ) ;
	Log::SaveLog( WORLD_LOGFILE, "Init Managers...OK!" ) ;



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL World::Loop( )
{
__ENTER_FUNCTION

	Log::SaveLog( WORLD_LOGFILE, "Loop..." ) ;
	g_pServerManager->Loop( ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

BOOL World::Exit( )
{
__ENTER_FUNCTION

	BOOL bRet ;

	Log::SaveLog( WORLD_LOGFILE, "Start Exit..." ) ;
	bRet = DelStaticManager( ) ;
	Assert( bRet ) ;
	Log::SaveLog( WORLD_LOGFILE, "Exit...OK!" ) ;


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL World::NewStaticManager( )
{
__ENTER_FUNCTION
	
//ShareMemory 最先

	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	if(WorldInfo.m_EnableShareMem)
	{
		BOOL ret;
		ret = g_GuildSMUPool.Init(MAX_GUILD_SIZE,WorldInfo.m_GuildKey,SMPT_WORLD,GL_SERVER_SHAREMEM);
		AssertEx(ret,"Guild共享内存初始化错误,请先启动ShareMemory ");

		ret = g_MailSMUPool.Init(MAX_MAILNODE_SIZE,WorldInfo.m_MailKey,SMPT_WORLD,GL_SERVER_SHAREMEM);
		AssertEx(ret,"Mail共享内存初始化错误,请先启动ShareMemory ");
	}

	g_pLog = new Log ;
	Assert( g_pLog ) ;
	BOOL ret = g_pLog->Init( ) ;
	Assert(ret) ;

	//
	g_pServerManager = new ServerManager ;
	Assert( g_pServerManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new ServerManager...OK" ) ;

	//
	g_pPlayerPool = new PlayerPool ;
	Assert( g_pPlayerPool ) ;
	Log::SaveLog( WORLD_LOGFILE, "new PlayerPool...OK" ) ;

	//
	g_pPacketFactoryManager = new PacketFactoryManager ;
	Assert( g_pPacketFactoryManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new PacketFactoryManager...OK" ) ;

	//
	g_pOnlineUser = new OnlineUser ;
	Assert( g_pOnlineUser ) ;
	Log::SaveLog( WORLD_LOGFILE, "new OnlineUser...OK" ) ;

	//
	g_pTeamList = new TeamList ;
	Assert( g_pTeamList ) ;
	Log::SaveLog( WORLD_LOGFILE, "new TeamList...OK" ) ;

	//
	g_pChatCenter = new ChatCenter ;
	Assert( g_pChatCenter ) ;
	Log::SaveLog( WORLD_LOGFILE, "new ChatCenter...OK" ) ;

	//
	g_pGuildManager = new GuildManager ;
	Assert( g_pGuildManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new GuildManager...OK" ) ;

	//
	g_pCityManager = new CityManager ;
	Assert( g_pCityManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new CityManager...OK" ) ;

	//
	g_pTopListManager = new TopListManager ;
	Assert( g_pTopListManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new g_pTopListManager...OK" ) ;

	g_pTeamRecruitManager = new TeamRecruitManager;
	Assert( g_pTeamRecruitManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "new g_pTeamRecruitManager...OK" ) ;

	g_pMailNodePool = new MailNodePool ;
	Assert( g_pMailNodePool ) ;
	Log::SaveLog( WORLD_LOGFILE, "new MailNodePool...OK" ) ;

	//
	g_pUserNodePool = new UserNodePool ;
	Assert( g_pUserNodePool ) ;
	Log::SaveLog( WORLD_LOGFILE, "new UserNodePool...OK" ) ;

	//
	g_pMailCenter = new MailCenter ;
	Assert( g_pMailCenter ) ;
	Log::SaveLog( WORLD_LOGFILE, "new MailCenter...OK" ) ;

	//
	g_pAllUser = new AllUser ;
	Assert( g_pAllUser ) ;
	Log::SaveLog( WORLD_LOGFILE, "new AllUser...OK" ) ;

	//
	g_pSceneInfo = new SceneInfo ;
	Assert( g_pSceneInfo ) ;
	Log::SaveLog( WORLD_LOGFILE, "new SceneInfo...OK" ) ;

	g_pWorldTimeInfo = new WorldTimeInfo;
	Assert(g_pWorldTimeInfo);
	Log::SaveLog(WORLD_LOGFILE,"new WorldTimeInfo....OK");

    g_pCountryManager = new CountryManager;
    Assert(g_pCountryManager);
    Log::SaveLog(WORLD_LOGFILE,"new CountryManager....OK");
	
__LEAVE_FUNCTION

	return TRUE ;
}

BOOL World::InitStaticManager( )
{
__ENTER_FUNCTION

	BOOL ret ;

	
	
	ret = g_TableInit.Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_TableInit.Init()...OK!" ) ;

	//
	ret = g_pServerManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pServerManager->Init()...OK" ) ;

	//
	ret = g_pPlayerPool->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;

	//
	ret = g_pPacketFactoryManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;

	//
	ret = g_pOnlineUser->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pOnlineUser->Init()...OK" ) ;

	//
	ret = g_pTeamList->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTeamList->Init()...OK" ) ;

	//
	ret = g_pChatCenter->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pChatCenter->Init()...OK" ) ;

	//
	ret = g_pGuildManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pGuildManager->Init()...OK" ) ;

	//
	ret = g_pCityManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pCityManager->Init()...OK" ) ;

	//
	ret = g_pTopListManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "TopListManager->Init()...OK" ) ;
	
	ret = g_pTeamRecruitManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTeamRecruitManager->Init()...OK" ) ;

	//
	ret = g_pMailNodePool->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pMailNodePool->Init()...OK" ) ;

	//
	ret = g_pUserNodePool->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pUserNodePool->Init()...OK" ) ;

	//
	ret = g_pMailCenter->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pMailCenter->Init()...OK" ) ;

	//
	ret = g_pAllUser->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pAllUser->Init()...OK" ) ;

	//
	ret = g_pSceneInfo->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pSceneInfo->Init()...OK" ) ;

	ret = g_pWorldTimeInfo->Init();
	Assert(ret);
	Log::SaveLog(WORLD_LOGFILE,"g_pWorldTimeInfo.Init()...OK");

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL World::DelStaticManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( g_pTeamList ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTeamList delete...OK" ) ;

	SAFE_DELETE( g_pChatCenter ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pChatCenter delete...OK" ) ;

	SAFE_DELETE( g_pGuildManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pGuildManager delete...OK" ) ;

	SAFE_DELETE( g_pCityManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pCityManager delete...OK" ) ;

	SAFE_DELETE( g_pTopListManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTopListManager delete...OK" ) ;

	SAFE_DELETE( g_pTeamRecruitManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTeamRecruitManager delete...OK" ) ;

	SAFE_DELETE( g_pTimeManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pTimeManager delete...OK" ) ;

	SAFE_DELETE( g_pMailNodePool ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pMailNodePool delete...OK" ) ;

	SAFE_DELETE( g_pUserNodePool ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pUserNodePool delete...OK" ) ;

	SAFE_DELETE( g_pMailCenter ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pMailCenter delete...OK" ) ;

	SAFE_DELETE( g_pAllUser ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pAllUser delete...OK" ) ;

	SAFE_DELETE( g_pSceneInfo ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pSceneInfo delete...OK" ) ;

	//
	//以下模块放后面删除
	//
	SAFE_DELETE( g_pOnlineUser ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pOnlineUser delete...OK" ) ;

	SAFE_DELETE( g_pPacketFactoryManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;

	SAFE_DELETE( g_pPlayerPool ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pPlayerPool delete...OK" ) ;

	SAFE_DELETE( g_pServerManager ) ;
	Log::SaveLog( WORLD_LOGFILE, "g_pServerManager delete...OK" ) ;

	SAFE_DELETE( g_pLog);

__LEAVE_FUNCTION

	return TRUE ;
}

