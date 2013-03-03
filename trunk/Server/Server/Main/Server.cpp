// Server.cpp : Defines the entry point for the console application.
// 

#include "stdafx.h"
#include "Server.h"
#include "Log.h"
#include "Ini.h"
#include "FileDef.h"
#include "TimeManager.h"
#include "ClientManager.h"
#include "SceneManager.h"
#include "ThreadManager.h"
#include "PlayerPool.h"
#include "Config.h"
#include "Thread.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "WorldManager.h"
#include "MachineManager.h"
#include "ObjManager.h"
#include "GUIDManager.h"
#include "GameTable.h"
#include "MonsterPool.h"
#include "PetPool.h"
#include "HorsePool.h"
#include "PlatformPool.h"
#include "SpecialPool.h"
#include "ItemTable.h"
#include "ItemManager.h"
#include "AbilityManager.h"
#include "ShopManager.h"
#include "CopySceneManager.h"
#include "DynamicSceneManager.h"
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "DiePenaltyManager.h"
#include "ItemBoxPool.h"
#include "Obj_ItemPool.h"
#include "Performance.h"
#include "DaemonThread.h"
#include "PlayerShopPool.h"
#include "Version.h"
#include "BusPool.h"
#include "Obj_Bus.h"

#if defined(__WINDOWS__)
#include "Minidump.h"
char * g_dump_exe_name = "Server";
#endif

extern INT					g_Command_CommandDeny ;//���Ʋ�������ֹʹ��GMָ��
extern INT					g_Command_AlwaysLoadScript ;//ÿ��ִ�нű�������Load
extern INT					g_Command_AlwaysLoadMonsterIni ;//ÿ�ζ���ȡmonster.ini�ļ�
extern SMUPool<HumanSMU>	g_HumanSMUPool;

Server						g_Server;

INT main(INT argc, CHAR* argv[])
{

#if defined(__WINDOWS__)
	//�����쳣��׽����
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	//�ڴ�й¶���
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif

__ENTER_FUNCTION
	//������Ʋ���
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
			if( strcmp(argv[i],"-commanddeny")==0 )
			{
				g_Command_CommandDeny=TRUE ;
			}
			if( strcmp(argv[i],"-loadscriptonce")==0 )
			{
				g_Command_AlwaysLoadScript=FALSE ;
			}
			if( strcmp(argv[i],"-alwaysloadmonsterini")==0  )
			{
				g_Command_AlwaysLoadMonsterIni=TRUE ;
			}
			else if( strcmp(argv[i],"-onelog")==0 )
			{
				extern BOOL g_Log_AllInOne;
				g_Log_AllInOne=TRUE ;
			}
		}
	}

	BOOL ret ;
	

//ʱ��ģ����Ҫ�ʼ��ʱ������
	g_pTimeManager = new TimeManager ;
	Assert( g_pTimeManager ) ;
	ret = g_pTimeManager->Init( ) ;
	Assert(ret) ;

//��־ģ���ʼ����Ҫ�ʼ����
	g_pLog = new Log ;
	Assert( g_pLog ) ;
	ret = g_pLog->Init( ) ;
	Assert(ret) ;

//#define _CHECK_MACHINE_
#if defined _CHECK_MACHINE_

extern BOOL CheckManchine( ) ;
	if( !CheckManchine( ) )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR: Permission error!" ) ;
		return 0 ;
	}

#endif
	g_pTimeManager->SetTime( ) ;
	Log::SaveLog( SERVER_LOGFILE, "GameServer Starting... (%.10d)(%d)",
			g_pTimeManager->Time2DWORD(),
		g_pTimeManager->StartTime() ) ;

	Log::SaveLog( SERVER_LOGFILE, "\n Current VERSION %x\n\n", CURRENT_VERSION );

	Log::SaveLog( SERVER_LOGFILE, "_ITEM=%d _ITEM_VALUE=%d _ITEM_GUID=%d", 
		sizeof(_ITEM), sizeof(_ITEM_VALUE), sizeof(_ITEM_GUID) ) ;
	Log::SaveLog( SERVER_LOGFILE, "_OWN_IMPACT=%d", sizeof(_OWN_IMPACT) ) ;
	Log::SaveLog( SERVER_LOGFILE, "_HUMAN_DB_LOAD=%d \r\n\
_EQUIP_DB_LOAD=%d \r\n\
_BAG_DB_LOAD=%d \r\n\
_BANK_DB_LOAD=%d \r\n\
_SKILL_DB_LOAD=%d \r\n\
_COOLDOWN_DB_LOAD_FOR_HUMAN=%d \r\n\
_XINFA_DB_LOAD=%d \r\n\
_IMPACT_DB_LOAD=%d \r\n\
_ABILITY_DB_LOAD=%d \r\n\
_MISSION_DB_LOAD=%d \r\n\
_SETTING_DB_LOAD=%d \r\n\
_PET_DB_LIST_LOAD=%d \r\n\
_RELATION_DB_LOAD=%d \r\n\
_PRIVATE_INFO_DB_LOAD=%d \r\n\
_SOUXIA_DB_LOAD=%d \r\n\
_MOUNT_DB_LOAD=%d \r\n\
FULLUSERDATA=%d",
//////////////////////////////////////////////////////////////////////////
sizeof(_HUMAN_DB_LOAD), 
sizeof(_EQUIP_DB_LOAD), 
sizeof(_BAG_DB_LOAD) ,
sizeof(_BANK_DB_LOAD) ,
sizeof(_SKILL_DB_LOAD) ,
sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN) ,
sizeof(_XINFA_DB_LOAD) ,
sizeof(_IMPACT_DB_LOAD) ,
sizeof(_ABILITY_DB_LOAD) ,
sizeof(_MISSION_DB_LOAD) ,
sizeof(_SETTING_DB_LOAD) ,
sizeof(_PET_DB_LIST_LOAD) ,
sizeof(_RELATION_DB_LOAD) ,
sizeof(_PRIVATE_INFO_DB_LOAD),
sizeof(_SOUXIA_DB_LOAD),
sizeof(_MOUNT_DB_LOAD),
sizeof(FULLUSERDATA)
) ;  // [7/8/2010 �¾���]
	UINT size = sizeof(_HUMAN_DB_LOAD);
		size +=sizeof(_EQUIP_DB_LOAD); 
		size +=sizeof(_BAG_DB_LOAD);
		size +=sizeof(_BANK_DB_LOAD);
		size +=sizeof(_SKILL_DB_LOAD);
		size +=sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN);
		size +=sizeof(_XINFA_DB_LOAD);
		size +=sizeof(_IMPACT_DB_LOAD);
		size +=sizeof(_ABILITY_DB_LOAD);
		size +=sizeof(_MISSION_DB_LOAD);
		size +=sizeof(_SETTING_DB_LOAD);
		size +=sizeof(_PET_DB_LIST_LOAD);
		size +=sizeof(_RELATION_DB_LOAD);
		size +=sizeof(_PRIVATE_INFO_DB_LOAD);
		size +=sizeof(_SOUXIA_DB_LOAD);
		size +=sizeof(_MOUNT_DB_LOAD);

	//���д����ֹע�͵�,��ֹ�޸�
	//10/03/11 ������������Ϊ�Ĺ��ķ��������޸ġ����������Ը������궨����6��Ϊ8
	//10/03/16 ��_HUMAN_DB_LOAD���������һ��int��������¼�û���ǰ��Ԫ��ֵ
	//10/05/05_EQUIP_DB_LOAD ��������HEQUIP_SOUXIA ����
 //   10/07/08 Ϊ����ϵͳ����_MOUNT_DB_LOAD                    [7/16/2010 �¾���]
	//10/07/23 Ϊ����ϵͳ����m_nMountIndex                     [7/23/2010 �¾���]
	if( sizeof(FULLUSERDATA) !=  189465)//127853��126533��128057��128057,132957,133297,158769,161817,187757,189461
	{
		AssertEx(FALSE,"�û��������ݳߴ緢���仯���벻Ҫ�����޸Ĵ˲��ִ���,��ϵ�¾�˵����������") ;
	}
	

	ret = g_Server.InitServer( ) ;
	Assert( ret ) ;

	ret = g_Server.Loop( ) ;
	Assert( ret ) ;

	ret = g_Server.ExitServer( ) ;
	Assert( ret ) ;
	
	

__LEAVE_FUNCTION
	
	return 0;
}

Server::Server( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	WORD wVersionRequested;
	WSADATA wsaData;
	INT err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData ); 
#endif

	m_TimeToQuit.CleanUp() ;

__LEAVE_FUNCTION
}

Server::~Server( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	WSACleanup();
#endif

__LEAVE_FUNCTION

}

BOOL Server::InitServer( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	Log::SaveLog( SERVER_LOGFILE, "InitServer FD_SETSIZE=%d...", FD_SETSIZE ) ;

	ret = g_Config.Init( ) ;
	Assert( ret ) ;


//�����ڴ�
	ret = NewStaticServer( ) ;
	Assert( ret ) ;



//��ʼ������ģ��
	ret = InitStaticServer( ) ;
	Assert( ret ) ;



__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Server::Loop( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	Log::SaveLog( SERVER_LOGFILE, "\r\nLoop..." ) ;


	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager->Start( )..." ) ;
	ret = g_pThreadManager->Start( ) ;
	Assert( ret ) ;

	MySleep( 1500 ) ;

	//���̵߳�����Դ�ָ�ClientManager��ִ�У�
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager->Loop( )..." ) ;
	g_pClientManager->start( ) ;


	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread->Loop( )..." ) ;
	g_pDaemonThread->Loop( ) ;


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Server::ExitServer( )
{
__ENTER_FUNCTION

	Log::SaveLog( SERVER_LOGFILE, "\r\nExitServer..." ) ;


	//////////////////
	//�ȴ������߳���ȫ�Ƴ���ִ���������
	WaitForAllThreadQuit( ) ;



	Log::SaveLog( SERVER_LOGFILE, "Begin delete..." ) ;
	//��̬����
	SAFE_DELETE( g_pClientManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager delete...OK" ) ;
	SAFE_DELETE( g_pThreadManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager delete...OK" ) ;


	//��̬����
	SAFE_DELETE( g_pSceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pSceneManager delete...OK" ) ;
	SAFE_DELETE( g_pPlayerPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerPool delete...OK" ) ;
	SAFE_DELETE( g_pPacketFactoryManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;
	SAFE_DELETE( g_pServerManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pServerManager delete...OK" ) ;
	SAFE_DELETE( g_pWorldManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pWorldManager delete...OK" ) ;
    SAFE_DELETE( g_pMachineManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMachineManager delete...OK" ) ;
	SAFE_DELETE( g_pGUIDManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pGUIDManager delete...OK" ) ;
	if ( g_pMonsterPool != NULL )
	{
		g_pMonsterPool->Term();
		SAFE_DELETE( g_pMonsterPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pMonsterPool delete...OK" ) ;
	if ( g_pPetPool != NULL )
	{
		g_pPetPool->Term();
		SAFE_DELETE( g_pPetPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pPetPool delete...OK" ) ;
	if ( g_pHorsePool != NULL )
	{
		g_pHorsePool->Term();
		SAFE_DELETE( g_pHorsePool ) ;
	}
	SAFE_DELETE( g_pBusPool ) ;
	Log::SaveLog(  SERVER_LOGFILE, "g_pBusPool delete...OK" ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pHorsePool delete...OK" ) ;
	SAFE_DELETE( g_pItemBoxPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pItemBoxPool delete...OK" ) ;

	SAFE_DELETE( g_pObj_ItemPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pObj_ItemPool delete...OK" ) ;

	if ( g_pDiePenaltyManager != NULL )
	{
		g_pDiePenaltyManager->CleanUp();
		SAFE_DELETE( g_pDiePenaltyManager ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pDiePenaltyManager delete...OK" ) ;

	
	SAFE_DELETE( g_pPlatformPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlatformPool delete...OK" ) ;
	SAFE_DELETE( g_pSpecialPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pSpecialPool delete...OK" ) ;
	SAFE_DELETE( g_pAbilityManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pAbilityManager delete...OK" ) ;
	SAFE_DELETE( g_pStaticShopManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pStaticShopManager delete...OK" ) ;
	SAFE_DELETE( g_pCopySceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pCopySceneManager delete...OK" ) ;
	SAFE_DELETE( g_pDynamicSceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDynamicSceneManager delete...OK" ) ;
	SAFE_DELETE( g_pPerformanceManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPerformanceManager delete...OK" ) ;
	SAFE_DELETE( g_pDaemonThread ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread delete...OK" ) ;
	SAFE_DELETE( g_pTimeManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pTimeManager delete...OK" ) ;
	SAFE_DELETE( g_pLog ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pLog delete...OK" ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID Server::WaitForAllThreadQuit( )
{
__ENTER_FUNCTION


#define MAX_WAIT_QUIT 300

	INT iQuit ;
	//�ʱ����Եȴ�MAX_WAIT_QUIT��
	for( INT i=0;i<MAX_WAIT_QUIT;i++ )
	{
		iQuit = g_QuitThreadCount ;
		printf( "Quit Thread:%d", iQuit ) ;
		MySleep( 1000 ) ;
		if( iQuit==g_pThreadManager->GetTotalThreads()+1 )
			break ;
	}

__LEAVE_FUNCTION
}

BOOL Server::NewStaticServer( )
{
__ENTER_FUNCTION


	BOOL ret = FALSE ;
	Log::SaveLog( SERVER_LOGFILE, "\r\nNewStaticServer( )...OK" ) ;


	//
	g_pMonsterPool = new MonsterPool ;
	Assert( g_pMonsterPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new MonsterPool...OK" ) ;

	//
	g_pPetPool = new PetPool ;
	Assert( g_pPetPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PetPool...OK" ) ;
	
	g_pHorsePool = new HorsePool;
	Assert( g_pHorsePool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new HorsePool...OK" ) ;

	//
	g_pSceneManager = new SceneManager ;
	Assert( g_pSceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new SceneManager...OK" ) ;

	//
	g_pPlayerPool = new PlayerPool ;
	Assert( g_pPlayerPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PlayerPool...OK" ) ;

	//
	g_pPacketFactoryManager = new PacketFactoryManager ;
	Assert( g_pPacketFactoryManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PacketFactoryManager...OK" ) ;

	//
	g_pServerManager = new ServerManager ;
	Assert( g_pServerManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new ServerManager...OK" ) ;

	//
	g_pWorldManager = new WorldManager ;
	Assert( g_pWorldManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new WorldManager...OK" ) ;

	//
	g_pMachineManager = new MachineManager ;
	Assert( g_pMachineManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new MachineManager...OK" ) ;

	//
	g_pGUIDManager = new GUIDManager ;
	Assert( g_pGUIDManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new GUIDManager...OK" ) ;

	
	//�������
	g_pItemBoxPool	=	new ItemBoxPool;
	Assert(g_pItemBoxPool);
	Log::SaveLog( SERVER_LOGFILE, "new ItemBoxPool...OK" ) ;

	//������Ʒ��
	g_pObj_ItemPool	=	new Obj_ItemPool;
	Assert(g_pObj_ItemPool);
	Log::SaveLog( SERVER_LOGFILE, "new Obj_ItemPool...OK" ) ;
	//
	g_pPlatformPool	=	new PlatformPool;
	Assert(g_pPlatformPool);
	Log::SaveLog( SERVER_LOGFILE, "new PlatformPool...OK" ) ;

	g_pSpecialPool	=	new SpecialPool;
	Assert(g_pSpecialPool);
	Log::SaveLog( SERVER_LOGFILE, "new SpecialPool...OK" ) ;

	g_pBusPool	=	new BusPool;
	Assert(g_pBusPool);
	DiskLog( SERVER_LOGFILE, "new BusPool...OK" ) ;

	//
	g_pAbilityManager = new AbilityManager;
	Assert(g_pAbilityManager);
	Log::SaveLog( SERVER_LOGFILE, "new AbilityManager...OK" ) ;

	g_pStaticShopManager = new StaticShopManager;
	Assert(g_pStaticShopManager);
	Log::SaveLog( SERVER_LOGFILE, "new StaticShopManager...OK" ) ;

	g_pCopySceneManager = new CopySceneManager;
	Assert(g_pCopySceneManager);
	Log::SaveLog( SERVER_LOGFILE, "new CopySceneManager...OK" ) ;

	g_pDynamicSceneManager = new DynamicSceneManager;
	Assert(g_pDynamicSceneManager);
	Log::SaveLog( SERVER_LOGFILE, "new DynamicSceneManager...OK" ) ;

	//��Ʒ������
	g_pItemManager = new ItemManager;
	Assert(g_pItemManager);
	Log::SaveLog( SERVER_LOGFILE, "new ItemManager...OK" ) ;

	g_pDaemonThread = new DaemonThread;
	Assert(g_pDaemonThread);
	Log::SaveLog( SERVER_LOGFILE, "new DaemonThread...OK" ) ;

	// �����ͷ�����
	g_pDiePenaltyManager = new DiePenaltyManager;
	Assert(g_pDiePenaltyManager);
	Log::SaveLog( SERVER_LOGFILE, "new DiePenaltyManager...OK" ) ;

	// ���ݼ�ع�����
	g_pPerformanceManager = new PerformanceManager;
	Assert(g_pPerformanceManager);
	Log::SaveLog( SERVER_LOGFILE, "new PerformanceManager...OK" ) ;


	//***ע��***
	//
	//  ��������ģ������������
	//
	g_pThreadManager = new ThreadManager ;
	Assert( g_pThreadManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new pThreadManager...OK" ) ;

	//
	g_pClientManager = new ClientManager ;
	Assert( g_pClientManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new ClientManager...OK" ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Server::Stop( )
{
__ENTER_FUNCTION

	if( g_pThreadManager )
	{
		g_pThreadManager->Stop( )  ;
	}

	if( g_pClientManager )
	{
		g_pClientManager->stop( ) ;
	}

	if( g_pDaemonThread )
	{
		g_pDaemonThread->Stop( ) ;
	}

__LEAVE_FUNCTION
}

BOOL Server::InitStaticServer( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	Log::SaveLog( SERVER_LOGFILE, "\r\nInitStaticServer( )...OK" ) ;

	//ShareMemory ����
	
	const _SERVER_DATA*	pCurrentSData =	g_pServerManager->GetCurrentServerInfo();
	
	if(pCurrentSData->m_EnableShareMem)
	{
		ret = g_HumanSMUPool.Init(MAX_POOL_SIZE,pCurrentSData->m_HumanSMKey,SMPT_SERVER,MAP_SERVER_SHAREMEM);
		AssertEx(ret,"�����ڴ��ʼ������,��������ShareMemory");

		if(g_HumanSMUPool.GetHeadVer()!=0)
		{
			AssertEx(ret,"ShareMemory ������δ��ɣ����Ժ�����Server");
		}

		ret = g_ItemSerialSMUPool.Init(1,pCurrentSData->m_ItemSerialKey,SMPT_SERVER,MAP_SERVER_SHAREMEM);
		AssertEx(ret,"�����ڴ��ʼ������,��������ShareMemory");

		if(g_ItemSerialSMUPool.GetHeadVer()!=0)
		{
			AssertEx(ret,"ShareMemory ������δ��ɣ����Ժ�����Server");
		}


	}

//�ȱ������
	//
	ret = g_ItemTable.Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_ItemTable.Init()...OK" ) ;

	//
	ret = g_TableInit.Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_TableInit.Init...OK" ) ;

//���߼�ģ������
	//
	ret = g_pMonsterPool->Init( g_Config.m_ConfigInfo.m_MaxMonster ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMonsterPool->Init()...OK" ) ;

	ret = g_pPetPool->Init( g_Config.m_ConfigInfo.m_MaxPet ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPetPool->Init()...OK" ) ;

	ret = g_pHorsePool->Init( g_Config.m_ConfigInfo.m_MaxPet ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pHorsePool->Init()...OK" ) ;

	//
	ret = g_pPlayerPool->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;

	//
	ret = g_pPacketFactoryManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;

	//
	ret = g_pWorldManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pWorldManager->Init()...OK" ) ;

	//
	ret = g_pMachineManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMachineManager->Init()...OK" ) ;

	//
	ret = g_pGUIDManager->Init( ) ;
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pGUIDManager->Init()...OK" ) ;
	
	//��Ʒ�г�ʼ��
	ret  =  g_pItemBoxPool->Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pItemBoxPool->Init()...OK" ) ;

	//��Ʒ�г�ʼ��
	ret  =  g_pObj_ItemPool->Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pObj_ItemPool->Init()...OK" ) ;

	//
	ret = g_pPlatformPool->Init( g_Config.m_ConfigInfo.m_MaxPlatform );
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pPlatformPool->Init( )...OK" );

	//
	ret = g_pSpecialPool->Init( g_Config.m_ConfigInfo.m_MaxSpecial );
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pSpecialPool->Init( )...OK" );

	//
	ret = g_pBusPool->Init( g_Config.m_ConfigInfo.m_MaxBus );
	Assert( ret );
	DiskLog( SERVER_LOGFILE, "g_pBusPool->Init( )...OK" );

	//
	ret = g_pAbilityManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pAbilityManager->Init()...OK" ) ;

	//
	ret = 	g_pStaticShopManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "m_pStaticShopManager->Init()...OK" ) ;

	//
	ret = g_pCopySceneManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pCopySceneManager->Init()...OK" ) ;

	//
	ret = g_pDynamicSceneManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDynamicSceneManager->Init()...OK" ) ;

	//
	ret = g_pSceneManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_SceneManager->Init()...OK" ) ;
	
	//	
	ret = g_pItemManager->Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pItemManager->Init()...OK" ) ;

	ret = g_pDiePenaltyManager->Init( 1024 );
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDiePenaltyManager->Init()...OK" ) ;

	ret = g_pPerformanceManager->Init( );
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPerformanceManager->Init()...OK" ) ;

	/************************************************************************/
	/*
	����������,��֮��������ݵ�����,��Щ�����Ǵ�sharemem�е��뵽���еģ�
	��Ҫ������Щ���ݵ��������ӵ�ָ���Ĺ�����
    */
	/************************************************************************/
	
	//***ע��***
	//
	//��������ʼ�������߳�����
	//
	ret = g_pServerManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pServerManager->Init()...OK" ) ;

	//
	ret = g_pThreadManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager->Init()...OK" ) ;

	//
	ret = g_pClientManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager->Init()...OK" ) ;

	//
	ret = g_pDaemonThread->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread->Init()...OK" ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CheckManchine( )
{
	FILE* f = NULL ;

	f = fopen( FILE_SERVER_VERSION, "r" ) ;
	if( f==NULL )
		return FALSE ;

#define SN_SIZE_SN 30

	CHAR szSN[SN_SIZE_SN+1] ;
	memset( szSN, 0, SN_SIZE_SN+1 ) ;
	INT iRead = (INT)(fread( szSN, SN_SIZE_SN, 1, f )) ;
	if( iRead!=1 )
	{
		fclose(f) ;
		return FALSE ;
	}

	if( strcmp(szSN,"B5A4498F61E0095288CD19BB439779")!=0 )
	{
		fclose(f) ;
		return FALSE ;
	}

	fclose(f) ;

	return TRUE ;
}
