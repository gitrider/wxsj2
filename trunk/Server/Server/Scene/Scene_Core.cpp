
#include "stdafx.h"
#include "Scene.h"
#include "Patrol.h"
#include "FileDataMgr.h"
#include "ChatPipe.h"
#include "MonsterManager.h"
#include "PetManager.h"
#include "HorseManager.h"
#include "SceneTimer.h"
#include "PacketFactoryManager.h"
#include "TimeManager.h"
#include "PlayerPool.h"
#include "Ini.h"
#include "Log.h"
#include "Config.h"
#include "ObjManager.h"
#include "StoreManager.h"
#include "RecyclePlayerManager.h"


#include "GCNewPlayer.h"
#include "GCNewPlayer_Move.h"
#include "GCNewPlayer_Death.h"
#include "GCNewMonster.h"
#include "GCNewMonster_Move.h"
#include "GCNewMonster_Death.h"
#include "GCNewPet.h"
#include "GCNewPet_Move.h"
#include "GCNewPet_Death.h"
#include "GCDelObject.h"
#include "GCNewPlatform.h"
#include "GCNewSpecial.h"
#include "GCNewItemBox.h"
#include "GCNewObjItem.h"
#include "GCNewBus_Move.h"
#include "GCNewBus.h"
#include "MissionManager.h"

#include "FileDef.h"
#include "ItemBoxManager.h"
#include "Obj_ItemManager.h"
#include "BusManager.h"



#define DEFAULT_MANAGER_NPC_COUNT	(4096)
#define DEFAULT_MANAGER_HUMAN_COUNT	(1024)

#define DEFAULT_SINGLE_MANAGER_MONSTER_COUNT	(2048)
#define DEFAULT_SINGLE_MANAGER_PET_COUNT		(1024)
#define DEFAULT_SINGLE_MANAGER_HUMAN_COUNT		(1024)
#define DEFAULT_SINGLE_MANAGER_PLATFORM_COUNT	(256)
#define DEFAULT_SINGLE_MANAGER_SPECIAL_COUNT	(1024)
#define DEFAULT_SINGLE_MANAGER_BUS_COUNT		(128)

#define MANAGER_HUMAN_LOGIC_INTERVAL			(0)
#define MANAGER_MONSTER_LOGIC_INTERVAL			(200)
#define MANAGER_PET_LOGIC_INTERVAL				(100)
#define MANAGER_PLATFORM_LOGIC_INTERVAL			(10000000)		// �������HeartBeat
#define MANAGER_SPECIAL_LOGIC_INTERVAL			(200)
#define MANAGER_BUS_LOGIC_INTERVAL				(200)

Scene::Scene( SceneID_t SceneID )
{
__ENTER_FUNCTION

	m_nSceneStatus = SCENE_STATUS_SLEEP ;
	m_SceneID = SceneID ;
	m_pScenePlayerManager = new ScenePlayerManager ;
	Assert( m_pScenePlayerManager ) ;
	m_pScenePlayerManager->SetSceneID( SceneID ) ;

	m_pRecyclePlayerManager = new RecyclePlayerManager();
	Assert( m_pRecyclePlayerManager );
	m_pRecyclePlayerManager->SetSceneID(SceneID);


	m_pMap = new Map ;
	Assert( m_pMap ) ;
	
	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;

	m_pObjManager = new ObjManager ;
	Assert(m_pObjManager) ;
	_OBJMAGAGER_INIT initObjManager;
	initObjManager.m_aInitCount[OBJ_ID_TYPE_NORMAL]		= DEFAULT_MANAGER_NPC_COUNT;
	initObjManager.m_aInitCount[OBJ_ID_TYPE_HUMAN]	= DEFAULT_MANAGER_HUMAN_COUNT;
	m_pObjManager->Init( &initObjManager );

	{
		m_pHumanManager = new HumanManager;
		Assert( m_pHumanManager ) ;
		_OBJSINGLEMANAGER_INIT initHumanManager;
		initHumanManager.m_uLength				= DEFAULT_SINGLE_MANAGER_HUMAN_COUNT;
		initHumanManager.m_pScene				= this;
		initHumanManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initHumanManager.m_uLogicInterval		= MANAGER_HUMAN_LOGIC_INTERVAL;
		m_pHumanManager->Init( &initHumanManager ) ;
	}

	{
		m_pMonsterManager = new MonsterManager ;
		Assert( m_pMonsterManager ) ;
		_OBJSINGLEMANAGER_INIT initMonsterManager;
		initMonsterManager.m_uLength			= DEFAULT_SINGLE_MANAGER_MONSTER_COUNT;
		initMonsterManager.m_pScene				= this;
		initMonsterManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initMonsterManager.m_uLogicInterval	= MANAGER_MONSTER_LOGIC_INTERVAL;
		m_pMonsterManager->Init( &initMonsterManager ) ;
	}

	{
		m_pPetManager = new PetManager;
		m_pPetManager->SetScene( this );
		Assert( m_pPetManager );
		_OBJSINGLEMANAGER_INIT initPetManager;
		initPetManager.m_uLength				= DEFAULT_SINGLE_MANAGER_PET_COUNT;
		initPetManager.m_pScene					= this;
		initPetManager.m_uCurrentTime			= g_pTimeManager->CurrentTime();
		initPetManager.m_uLogicInterval		= MANAGER_PET_LOGIC_INTERVAL;
		m_pPetManager->Init( &initPetManager );
	}


	{
		m_pHorseManager = new HorseManager;
		m_pHorseManager->SetScene( this );
		Assert( m_pHorseManager );
		_OBJSINGLEMANAGER_INIT initPetManager;
		initPetManager.m_uLength			= DEFAULT_SINGLE_MANAGER_PET_COUNT;
		initPetManager.m_pScene				= this;
		initPetManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initPetManager.m_uLogicInterval		= MANAGER_PET_LOGIC_INTERVAL;
		m_pHorseManager->Init( &initPetManager );

	}

	m_pItemBoxManager	=	new	ItemBoxManager;
	Assert(m_pItemBoxManager);
	m_pItemBoxManager->SetScene(this);

	m_pObjItemManager  =  new	Obj_ItemManager;
	Assert(m_pObjItemManager);
	m_pObjItemManager->SetScene(this);

	{
		m_pPlatformManager	= new PlatformManager;
		Assert( m_pPlatformManager );
		_OBJSINGLEMANAGER_INIT initPlatformManager;
		initPlatformManager.m_uLength			= DEFAULT_SINGLE_MANAGER_PLATFORM_COUNT;
		initPlatformManager.m_pScene			= this;
		initPlatformManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initPlatformManager.m_uLogicInterval	= MANAGER_PLATFORM_LOGIC_INTERVAL;
		m_pPlatformManager->Init( &initPlatformManager );
	}

	{
		m_pSpecialManager	= new SpecialManager;
		Assert( m_pSpecialManager );
		_OBJSINGLEMANAGER_INIT initSpecialManager;
		initSpecialManager.m_uLength			= DEFAULT_SINGLE_MANAGER_SPECIAL_COUNT;
		initSpecialManager.m_pScene				= this;
		initSpecialManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initSpecialManager.m_uLogicInterval		= MANAGER_SPECIAL_LOGIC_INTERVAL;
		m_pSpecialManager->Init( &initSpecialManager );
	}

	{
		m_pBusManager	= new BusManager;
		Assert( m_pBusManager );
		_OBJSINGLEMANAGER_INIT initBusManager;
		initBusManager.m_uLength			= DEFAULT_SINGLE_MANAGER_BUS_COUNT;
		initBusManager.m_pScene				= this;
		initBusManager.m_uCurrentTime		= g_pTimeManager->CurrentTime();
		initBusManager.m_uLogicInterval		= MANAGER_BUS_LOGIC_INTERVAL;
		m_pBusManager->Init( &initBusManager );
	}

	m_pLuaInterface = new LuaInterface;
	Assert( m_pLuaInterface ) ;

	m_pSceneTimers = new SceneTimer;
	Assert( m_pSceneTimers);

	m_pScriptFileMgr = new CFileDataMgr;
	Assert( m_pScriptFileMgr ) ;
		
	m_GrowPointGroup.SetScene(this);

	m_pStallInfoManager = new StallInfoManager;
	Assert(m_pStallInfoManager);

	m_pStoreManager	=	new StoreManager;
	Assert(m_pStoreManager);
	
	m_pAreaManager	=	new AreaManager;
	Assert(m_pAreaManager);
	m_pAreaManager->SetScene(this);
	
	m_pTimerDoingManager	=	new TimerDoingManager();
	Assert(m_pTimerDoingManager);
	m_pTimerDoingManager->SetScene(this);

	m_pDropPosManager = new SceneDropPosManager();
	Assert(m_pDropPosManager);
	m_pDropPosManager->SetScene(this);

	m_pZone = NULL ;

	//��ʼ������ܵ�����
	m_pChatPipe = new ChatPipe ;
	Assert( m_pChatPipe ) ;
	m_pChatPipe->Init( this ) ;

	m_QuitTimer.CleanUp() ;
	m_SceneTimer.CleanUp() ;
	m_CopySceneQuitTimer.CleanUp() ;
	m_SceneType = SCENE_TYPE_GAMELOGIC ;
	m_Perfor.m_SceneID = SceneID ;

	//��������
	m_CityData.CleanUp();

	//��ʼ��Ѳ��·������
	m_pPatrolPathMgr = new PatrolPathMgr;
	Assert( m_pPatrolPathMgr );
	m_pPatrolPathMgr->Init( this );

	//ս���¼���������ʼ��
	m_EventCore.Init(this);

	m_pPacket_NewPlayer			= new GCNewPlayer;
	m_pPacket_NewPlayer_Move	= new GCNewPlayer_Move;
	m_pPacket_NewPlayer_Death	= new GCNewPlayer_Death;
	m_pPacket_NewMonster		= new GCNewMonster;
	m_pPacket_NewMonster_Move	= new GCNewMonster_Move;
	m_pPacket_NewMonster_Death	= new GCNewMonster_Death;
	m_pPacket_NewPet			= new GCNewPet;
	m_pPacket_NewPet_Move		= new GCNewPet_Move;
	m_pPacket_NewPet_Death		= new GCNewPet_Death;
	m_pPacket_NewPlatform		= new GCNewPlatform;
	m_pPacket_NewSpecial		= new GCNewSpecial;
	m_pPacket_NewItemBox		= new GCNewItemBox;
	m_nPacket_NewObj_Item		= new GCNewObjItem;
	m_pPacket_DelObject			= new GCDelObject;

	m_pPacket_NewBus_Move		= new GCNewBus_Move;
	AssertEx(m_pPacket_NewBus_Move, "[Scene::Scene]: New Memory failed!!");

	m_pPacket_NewBus			= new GCNewBus;
	AssertEx(m_pPacket_NewBus, "[Scene::Scene]: New Memory failed!!");

	m_pMissionData				= new MISSION_DATA;

__LEAVE_FUNCTION
}

Scene::~Scene( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pMissionData );

	SAFE_DELETE( m_pPacket_NewPlayer );
	SAFE_DELETE( m_pPacket_NewPlayer_Move );
	SAFE_DELETE( m_pPacket_NewPlayer_Death );
	SAFE_DELETE( m_pPacket_NewMonster );
	SAFE_DELETE( m_pPacket_NewMonster_Move );
	SAFE_DELETE( m_pPacket_NewMonster_Death );
	SAFE_DELETE( m_pPacket_NewPet );
	SAFE_DELETE( m_pPacket_NewPet_Move );
	SAFE_DELETE( m_pPacket_NewPet_Death );
	SAFE_DELETE( m_pPacket_NewPlatform );
	SAFE_DELETE( m_pPacket_NewSpecial );
	SAFE_DELETE( m_pPacket_NewItemBox );
	SAFE_DELETE( m_nPacket_NewObj_Item );
	SAFE_DELETE( m_pPacket_DelObject );
	SAFE_DELETE( m_pPacket_NewBus_Move );
	SAFE_DELETE( m_pPacket_NewBus );

	SAFE_DELETE( m_pMap ) ;
	SAFE_DELETE( m_pScenePlayerManager ) ;
	SAFE_DELETE( m_pRecyclePlayerManager);
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	SAFE_DELETE( m_pObjManager ) ;
	SAFE_DELETE( m_pHumanManager ) ;
	SAFE_DELETE( m_pMonsterManager ) ;
	if ( m_pMonsterManager != NULL )
	{
		m_pMonsterManager->RemoveAllMonster();
		SAFE_DELETE( m_pMonsterManager );
	}
	if ( m_pPetManager != NULL )
	{
		m_pPetManager->RemoveAllPet();
		SAFE_DELETE( m_pPetManager );
	}
	if( m_pHorseManager != NULL )
	{
		m_pHorseManager->RemoveAllHorse();
		SAFE_DELETE( m_pHorseManager );
	}
	SAFE_DELETE_ARRAY( m_pZone ) ;
	SAFE_DELETE( m_pItemBoxManager ) ;
	SAFE_DELETE( m_pObjItemManager );
	if ( m_pPlatformManager != NULL )
	{
		m_pPlatformManager->RemoveAllPlatform();
		SAFE_DELETE( m_pPlatformManager );
	}
	if ( m_pSpecialManager != NULL )
	{
		m_pSpecialManager->RemoveAllSpecial();
		SAFE_DELETE( m_pSpecialManager );
	}
	if ( m_pBusManager != NULL )
	{
		m_pBusManager->RemoveAllBus();
		SAFE_DELETE( m_pBusManager );
	}
	//SAFE_DELETE( m_pSkillObjManager ) ;
	SAFE_DELETE( m_pLuaInterface );
	SAFE_DELETE( m_pScriptFileMgr );
	SAFE_DELETE( m_pStallInfoManager ) ;
	SAFE_DELETE( m_pStoreManager ) ;
	SAFE_DELETE( m_pChatPipe ) ;
	SAFE_DELETE( m_pPatrolPathMgr ) ;
	SAFE_DELETE( m_pAreaManager ) ;
	SAFE_DELETE( m_pTimerDoingManager ) ;
	SAFE_DELETE( m_pDropPosManager );
__LEAVE_FUNCTION
}

BOOL Scene::Load( SCENE_LOAD* load )
{
__ENTER_FUNCTION

	CHAR	szTemp[_MAX_PATH], 
			szMap[_MAX_PATH],
			szMonster[_MAX_PATH],
			szPlatform[_MAX_PATH],
			szGrowPointData[_MAX_PATH],
			szGrowPointSetup[_MAX_PATH],
			szPatrolPointData[_MAX_PATH],
			szArea[_MAX_PATH],
			//szPet[_MAX_PATH],
			szStallInfo[_MAX_PATH],
			szBus[_MAX_PATH];

	memset( szTemp, 0, _MAX_PATH ) ;
	memset( szMap, 0, _MAX_PATH ) ;
	memset( szMonster, 0, _MAX_PATH ) ;
	memset( szPlatform, 0, _MAX_PATH ) ;
	memset( szGrowPointData, 0, _MAX_PATH ) ;
	memset( szGrowPointSetup, 0, _MAX_PATH ) ;
	memset( szPatrolPointData, 0, _MAX_PATH ) ;
	memset( szArea, 0, _MAX_PATH ) ;
	//memset( szPet, 0, _MAX_PATH ) ;
	memset( szStallInfo, 0, _MAX_PATH ) ;
	memset( szBus, 0, _MAX_PATH ) ;

	BOOL	IsPlatformExist;
	BOOL	IsGrowPointExist;
	BOOL	IsStallInfoExist;
	BOOL	IsBusExist;


	strncpy( szMap, load->m_szMap, _MAX_PATH-1 ) ;
	strncpy( szMonster, load->m_szMonster, _MAX_PATH-1 ) ;
	strncpy( szPlatform, load->m_szPlatform, _MAX_PATH-1 ) ;
	strncpy( szGrowPointData, load->m_szGrowPointData, _MAX_PATH-1 ) ;
	strncpy( szGrowPointSetup, load->m_szGrowPointSetup, _MAX_PATH-1 ) ;
	strncpy( szPatrolPointData, load->m_szPatrolPointData, _MAX_PATH-1 ) ;
	strncpy( szArea, load->m_szArea, _MAX_PATH-1 ) ;
	//strncpy( szPet, load->m_szPet, _MAX_PATH-1 ) ;
	strncpy( szStallInfo, load->m_szStallInfo, _MAX_PATH-1 ) ;
	strncpy( szBus, load->m_szBus, _MAX_PATH-1 ) ;

	IsGrowPointExist = load->m_IsGrowPointExist ;
	IsPlatformExist = load->m_IsPlatformExist ;
	IsStallInfoExist = load->m_IsStallInfoExist ;
	IsBusExist = load->m_IsBusExist ;

	//�����ǰ�����е�Player��Ϣ
	if( m_pScenePlayerManager )
	{
		m_pScenePlayerManager->RemoveAllPlayer() ;
	}

	Log::SaveLog( SERVER_LOGFILE, "BeginLoad %d", this->SceneID() ) ;

	////////////////////////////////////////////////////////////////////////////////
	//��ȡ��ͼ��Ϣ
	memset( szTemp, 0, _MAX_PATH ) ;
	GET_SCENE_FULL_PATH( szTemp, szMap );
	
	BOOL ret = m_pMap->Load( szTemp ) ;
	if( !ret )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	

	if(m_pZone==NULL)
	{
		INT cx = (INT)(m_pMap->CX()/g_Config.m_ConfigInfo.m_ZoneSize) ;
		INT cz = (INT)(m_pMap->CZ()/g_Config.m_ConfigInfo.m_ZoneSize) ;
		if( (INT)m_pMap->CX()%g_Config.m_ConfigInfo.m_ZoneSize > 0 ) cx ++ ;
		if( (INT)m_pMap->CZ()%g_Config.m_ConfigInfo.m_ZoneSize > 0 ) cz ++ ;

		m_ZoneInfo.m_wZoneSize = (WORD)(cx*cz) ;
		m_ZoneInfo.m_wZoneW = (WORD)cx ;
		m_ZoneInfo.m_wZoneH = (WORD)cz ;

		m_pZone = new Zone[m_ZoneInfo.m_wZoneSize] ;
		if( m_pZone==NULL )
		{
			Assert( FALSE ) ;
			return FALSE ;
		}
		for( WORD i=0; i<m_ZoneInfo.m_wZoneSize; i++ )
		{
			m_pZone[i].SetZoneID( (ZoneID_t)i ) ;
		}
	}
	else
	{
		for( WORD i=0; i<m_ZoneInfo.m_wZoneSize; i++ )
		{
			m_pZone[i].CleanUp() ;
		}
	}


	Log::SaveLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp ) ;

	m_ClientRes = g_pSceneManager->GetClientResByNavName( szMap ) ;

	//////////////////////////////////////////////////////////////////////////
	// ��ȡѲ��·����Ϣ FrankWu[9/20/2005]
	memset( szTemp, 0, _MAX_PATH ) ;
	GET_SCENE_FULL_PATH( szTemp, szPatrolPointData );

	m_pPatrolPathMgr->CleanUp() ;
	m_pPatrolPathMgr->LoadPatrolPoint(szTemp);
	Log::SaveLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp ) ;


	////////////////////////////////////////////////////////////////////////////////
	//��ȡ������Ϣ �Ѿ���������ȡ������Ϣ��
	memset( szTemp, 0, _MAX_PATH ) ;
	GET_SCENE_FULL_PATH( szTemp, szMonster );

	// ���ó�����Ϣ
	m_pPetManager->RemoveAllPet() ;
	m_pPetManager->Reset();
	
	m_pHorseManager->RemoveAllHorse();
	m_pHorseManager->Reset();

	m_pMonsterManager->RemoveAllMonster() ;
	m_pMonsterManager->Reset() ;
	ret = m_pMonsterManager->LoadMonster( szTemp ) ;
	if( !ret )
	{
		Assert(FALSE) ;
		return FALSE ;
	}


	////////////////////////////////////////////////////////////////////////////////
	// ��ȡ����̨
	if( IsPlatformExist != FALSE )
	{
		memset( szTemp, 0, _MAX_PATH );
		GET_SCENE_FULL_PATH( szTemp, szPlatform );

		m_pPlatformManager->RemoveAllPlatform( ) ;
		ret = m_pPlatformManager->LoadPlatform( szTemp );
		if( ret == FALSE )
		{
			Assert(FALSE) ;
			Log::SaveLog( SERVER_LOGFILE, "This scene read platform error!" );
		}
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp );
		}
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "This scene has no platform!" );
	}

	////////////////////////////////////////////////////////////////////////////////
	//��ȡ̯λ��Ϣ, �������ڿ��ٸ���
	if(IsStallInfoExist)
	{
		memset( szTemp, 0, _MAX_PATH );
		GET_SCENE_FULL_PATH( szTemp, szStallInfo );
		Assert(m_pStallInfoManager);
		m_pStallInfoManager->CleanUp();
		//�����ڴ�
		m_pStallInfoManager->Init( m_pMap->CX(), m_pMap->CZ() );
		//��ȡ����
		if(m_pStallInfoManager->Load( szTemp ) == FALSE)
		{
			m_pStallInfoManager->CleanUp();
			Assert(FALSE) ;
			Log::SaveLog( SERVER_LOGFILE, "This scene read stallinfo error!" );
		}
		else
		{
			Log::SaveLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp );
		}
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "This scene has no stallinfo!" );
	}

	////////////////////////////////////////////////////////////////////////////////
	//��ȡ��ʼ�������������
	m_pItemBoxManager->Init();
	Log::SaveLog( SERVER_LOGFILE, "Init ItemBoxManager OK!" );

	m_pObjItemManager->Init();
	Log::SaveLog( SERVER_LOGFILE, "Init Obj_ItemManager OK!" );
	////////////////////////////////////////////////////////////////////////////////

	//��ȡ��ʼ�������������
	m_pTimerDoingManager->Init();
	m_pTimerDoingManager->SetScene(this);
	Log::SaveLog( SERVER_LOGFILE, "Init TimerDoingManager OK!" );

	//�������
	m_pLuaInterface->Destroy() ;
	m_pLuaInterface->Init(this);
	Log::SaveLog( SERVER_LOGFILE, "Init LuaInterface OK!" );

	////////////////////////////////////////////////////////////////////////////////
	//�ű�ע�����
	if( !m_pScriptFileMgr->IsInit() )
	{
		m_pScriptFileMgr->Init( FILE_SCRIPT, FALSE);
	}
	Log::SaveLog( SERVER_LOGFILE, "Load ../Public/Data/script.dat OK!" );

	////////////////////////////////////////////////////////////////////////////////
	//������ʱ��
	if( !m_pSceneTimers->IsInit() )
	{
		m_pSceneTimers->CreateTimer(g_Config.m_ConfigInfo.m_MaxTimerCount,this);
	}
	else
	{
		m_pSceneTimers->CleanUp() ;
	}
								
	////////////////////////////////////////////////////////////////////////////////
	//����������
	if(IsGrowPointExist)
	{
		memset( szTemp, 0, _MAX_PATH ) ;
		GET_SCENE_FULL_PATH( szTemp, szGrowPointData );
		strcpy(szGrowPointData,szTemp);

		memset( szTemp, 0, _MAX_PATH ) ;
		GET_SCENE_FULL_PATH( szTemp, szGrowPointSetup );
		strcpy(szGrowPointSetup,szTemp);

		m_GrowPointGroup.CleanUp() ;
		BOOL	bInitGp =  m_GrowPointGroup.Load(szGrowPointData,szGrowPointSetup);
		if(!bInitGp)
		{
			Log::SaveLog( SERVER_LOGFILE, "This scene has no growpoint!" );
		}
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "This scene has no growpoint!" );
	}

	////////////////////////////////////////////////////////////////////////////////
	// ��ȡBus
	if(IsBusExist)
	{
		memset( szTemp, 0, _MAX_PATH );
		GET_SCENE_FULL_PATH( szTemp, szBus );

		_MY_TRY
		{
			m_pBusManager->RemoveAllBus( ) ;
		}
		_MY_CATCH
		{
		}
		m_pBusManager->Reset( ) ;
		ret = m_pBusManager->LoadBus( szTemp );
		if( ret == FALSE )
		{
			DiskLog( SERVER_LOGFILE, "This scene read bus error!" );
		}
		else
		{
			DiskLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp );
		}
	}
	else
	{
		DiskLog( SERVER_LOGFILE, "This scene has no bus!" );
	}

	////////////////////////////////////////////////////////////////////////////////
	//�¼���
	memset( szTemp, 0, _MAX_PATH ) ;
	GET_SCENE_FULL_PATH( szTemp, szArea );

	m_pAreaManager->Term() ;
	ret = m_pAreaManager->Init( szTemp ) ;
	if( !ret )
	{
		Assert(FALSE) ;
		return FALSE ;
	}
	Log::SaveLog( SERVER_LOGFILE, "Load %s ...OK!", szTemp );

	m_EventCore.Reset() ;
	Log::SaveLog( SERVER_LOGFILE, "Reset EventCore ...OK!" );

	Log::SaveLog( SERVER_LOGFILE, "EndLoad %d", this->SceneID() ) ;

	////////////////////////////////////////////////////////////////////////////////
	m_pDropPosManager->CleanUp();
	m_pDropPosManager->Init((INT)m_pMap->CX(), (INT)m_pMap->CZ());

	Log::SaveLog( SERVER_LOGFILE, "Init DropPos ...OK!" );
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::Init( )
{
__ENTER_FUNCTION



__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::Tick( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if( StatusLogic( ) == FALSE )
	{
		//MySleep(1000) ;
		return TRUE ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_LOGIC] ++ ;

	//���紦��
	_MY_TRY
	{
		ret = m_pScenePlayerManager->Select( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_SELECT] ++ ;

	_MY_TRY
	{
		ret = m_pScenePlayerManager->ProcessExceptions( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_PROCESSEXCEPTIONS] ++ ;

	_MY_TRY
	{
		ret = m_pScenePlayerManager->ProcessInputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_PROCESSINPUTS] ++ ;

	_MY_TRY
	{
		ret = m_pScenePlayerManager->ProcessOutputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_PROCESSOUTPUTS] ++ ;

	//��Ϣ����
	_MY_TRY
	{
		ret = m_pScenePlayerManager->ProcessCommands( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_PROCESSCOMMANDS] ++ ;

	//������Ϣ����
	_MY_TRY
	{
		ProcessCacheCommands( ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_PROCESSCACHECOMMANDS] ++ ;


	//�߼�����
	_MY_TRY
	{
		ret = HeartBeat( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_TICK_HEARTBEAT] ++ ;

	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::StatusLogic( )
{
__ENTER_FUNCTION

	switch( m_nSceneStatus )
	{
	case SCENE_STATUS_SLEEP:
		{
			return FALSE ;
		}
		break ;
	case SCENE_STATUS_SELECT:
		{
			return FALSE ;
		}
		break ;
	case SCENE_STATUS_LOAD:
		{
			return FALSE ;
		}
		break ;
	case SCENE_STATUS_INIT:
		{
			SetSceneStatus( SCENE_STATUS_RUNNING ) ;
			OnSceneInit( ) ;
			return FALSE ;
		}
		break ;
	case SCENE_STATUS_RUNNING:
		{
			return TRUE ;
		}
		break ;
	default :
		{
			Assert( FALSE ) ;
		}
		break ;
	};


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
BOOL Scene::HeartBeat( )
{
	__ENTER_FUNCTION

	BOOL ret ;

	UINT uTime = g_pTimeManager->CurrentTime() ;
	//�趨�����ʱ��
	g_pTimeManager->SetTime();

	//�����ر��߼�
	if( m_QuitTimer.IsSetTimer() )
	{
		if( m_QuitTimer.CountingTimer( uTime ) )
		{
			Log::SaveLog( SERVER_LOGFILE, "Scene %d has been closed!", 
				SceneID() ) ;
			SetSceneStatus( SCENE_STATUS_SLEEP ) ;
			m_QuitTimer.CleanUp( ) ;
			m_CopyData.CleanUp( ) ;
			m_CopySceneQuitTimer.CleanUp( ) ;
			return TRUE;
		}
	}
	else
	{
		if( m_pScenePlayerManager->HasPlayer() )
		{//����������
			m_CopySceneQuitTimer.CleanUp() ;
		}
		else if( GetSceneType()==SCENE_TYPE_COPY )
		{//�����ǰ��������û���ˣ�����û����
			if( !m_CopySceneQuitTimer.IsSetTimer() )
			{
				m_CopySceneQuitTimer.BeginTimer(m_CopyData.m_NoUserCloseTime, uTime) ;

				Log::SaveLog( SERVER_LOGFILE, "Scene %d will been closed in %d seconds!", 
					SceneID(), m_CopyData.m_NoUserCloseTime/1000 ) ;
			}
			if( m_CopySceneQuitTimer.CountingTimer(uTime) )
			{
				CloseScene() ;
			}
		}
		else if( GetSceneType()==SCENE_TYPE_CIT && m_CityData.m_bClose)
		{//�����رճ����߼�
			if( !m_CityData.m_DynamicSceneQuitTimer.IsSetTimer() )
			{
				m_CityData.m_DynamicSceneQuitTimer.BeginTimer(5000, uTime) ;

				Log::SaveLog( SERVER_LOGFILE, "Scene %d will been closed in %d seconds!", 
					SceneID(), 5 ) ;
			}
			if( m_CityData.m_DynamicSceneQuitTimer.CountingTimer(uTime) )
			{
				m_CityData.m_bClose = FALSE;
				m_CityData.m_DynamicSceneQuitTimer.CleanUp();
				CloseScene() ;
			}
		}
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_CLOSELOGIC] ++ ;

	//������ʱ���߼�
	_MY_TRY
	{
		if( m_SceneTimer.CountingTimer(uTime) )
		{
			OnSceneTimer( uTime ) ;
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_SCENETIMER] ++ ;

	_MY_TRY
	{
		ret = m_pScenePlayerManager->HeartBeat(uTime ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_SCENEPLAYERMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pHumanManager->HeartBeat(uTime ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_HUMANMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pMonsterManager->HeartBeat(uTime ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_MONSTERMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pPetManager->HeartBeat(uTime ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = m_pHorseManager->HeartBeat(uTime ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	m_Perfor.m_aPerfor[SPT_HEARTBEAT_PETMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pBusManager->HeartBeat(uTime );
		Assert(ret) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_BUSMANANGER] ++ ;

	_MY_TRY
	{
		ret = m_pItemBoxManager->HeartBeat(uTime );
		Assert(ret) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	m_Perfor.m_aPerfor[SPT_HEARTBEAT_ITEMBOXMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pObjItemManager->HeartBeat(uTime );
		Assert(ret) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_OBJITEMMANAGER] ++ ;

	_MY_TRY
	{
		ret = m_pSpecialManager->HeartBeat(uTime );
		Assert(ret) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_SPECIALMANANGER] ++ ;

	_MY_TRY
	{
		ret = m_GrowPointGroup.HeartBeat(uTime);
		Assert(ret);
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_GROWPOINTGROUP] ++ ;

	_MY_TRY
	{
		ret = m_pChatPipe->HeartBeat(uTime ) ;
		Assert(ret);
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_CHATPIPE] ++ ;

	_MY_TRY
	{
		ret = m_pTimerDoingManager->HeartBeat(uTime );
		Assert(ret) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_TIMERDOING] ++ ;

	_MY_TRY
	{
		if(m_pSceneTimers->mRefeshTimer.CountingTimer(uTime))
		{
			m_pSceneTimers->OnTimer(uTime);
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_SCENETIMERS] ++ ;


	//�û����մ���
	_MY_TRY
	{
		ret = m_pRecyclePlayerManager->HeartBeat(uTime);
		Assert(ret);
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}
	m_Perfor.m_aPerfor[SPT_HEARTBEAT_RECYCLEPLAYERMANAGER] ++ ;
	

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			continue ;
		}

		BOOL bNeedRemove = TRUE ;

		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				this->BroadCast_Scene( pPacket ) ;
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
		else if( PlayerID==INVALID_ID_EX )
		{
			uint uret ;
			_MY_TRY
			{
				uret = pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
				uret = PACKET_EXE_ERROR ;
			}
			if( uret == PACKET_EXE_ERROR )
			{
			}
			else if( uret == PACKET_EXE_BREAK )
			{
			}
			else if( uret == PACKET_EXE_CONTINUE )
			{
			}
			else if( uret == PACKET_EXE_NOTREMOVE )
			{
				bNeedRemove = FALSE ;
			}
			else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
			{
				bNeedRemove = FALSE ;
			}	
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;

				uint uret ;
				_MY_TRY
				{
					uret = pPacket->Execute(pPlayer) ;
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
					uret = PACKET_EXE_ERROR ;
				}
				if( uret == PACKET_EXE_ERROR )
				{
					GetScenePlayerManager()->RemovePlayer( pPlayer, TRUE ) ;
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;

					GetScenePlayerManager()->RemovePlayer( pPlayer, TRUE ) ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}

		//������Ϣ
		if( bNeedRemove )
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}


	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::ResizeCache( )
{
__ENTER_FUNCTION

	ASYNC_PACKET* pNew = new ASYNC_PACKET[m_QueSize+MAX_CACHE_SIZE] ;
	if( pNew==NULL )
		return FALSE ;

	memcpy( pNew, &(m_PacketQue[m_Head]), sizeof(ASYNC_PACKET)*(m_QueSize-m_Head) ) ;
	if( m_Head!=0 )
	{
		memcpy( &(pNew[m_QueSize-m_Head]), &(m_PacketQue[0]), sizeof(ASYNC_PACKET)*(m_Head) ) ;
	}

	memset( m_PacketQue, 0, sizeof(ASYNC_PACKET)*m_QueSize ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	m_PacketQue = pNew ;

	m_Head = 0 ;
	m_Tail = m_QueSize ;
	m_QueSize = m_QueSize+MAX_CACHE_SIZE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::SendPacket( Packet* pPacket, PlayerID_t PlayerID, uint Flag )
{
__ENTER_FUNCTION

	m_pScenePlayerManager->Lock( ) ;

	if( m_PacketQue[m_Tail].m_pPacket )
	{//��������
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	m_pScenePlayerManager->Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	m_pScenePlayerManager->Unlock( ) ;
	return FALSE ;
}

BOOL Scene::MovePacket( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	m_pScenePlayerManager->Lock( ) ;

	uint Cur = m_Head ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	m_pScenePlayerManager->Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	m_pScenePlayerManager->Unlock( ) ;
	return FALSE ;
}

BOOL Scene::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
__ENTER_FUNCTION

	m_pScenePlayerManager->Lock( ) ;

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//��������û����Ϣ
		m_pScenePlayerManager->Unlock( ) ;
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;


	m_pScenePlayerManager->Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	m_pScenePlayerManager->Unlock( ) ;
	return FALSE ;
}

BOOL Scene::CloseScene( )
{
__ENTER_FUNCTION

	if( !m_QuitTimer.IsSetTimer() 
		&& GetSceneStatus()!=SCENE_STATUS_SLEEP
		&& GetSceneType()!=SCENE_TYPE_GAMELOGIC
		)
	{
		UINT uTime = 5000 ;
		m_QuitTimer.BeginTimer( uTime, g_pTimeManager->CurrentTime() ) ;

		OnSceneQuit( ) ;

		Log::SaveLog( SERVER_LOGFILE, "Scene %d will close in %d seconds!", 
			SceneID(), uTime/1000 ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
