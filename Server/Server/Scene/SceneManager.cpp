
#include "stdafx.h"
#include "SceneManager.h"
#include "Ini.h"
#include "ScenePlayerManager.h"
#include "ServerManager.h"
#include "MachineManager.h"
#include "Obj_Monster.h"
#include "Patrol.h"
#include "Obj_Platform.h"
#include "Obj_Bus.h"
#include "AreaManager.h"
#include "GrowPointManager.h"


SceneManager* g_pSceneManager=NULL ;

INT g_Command_AlwaysLoadMonsterIni=0 ;

/////////////////////////////////////////////////////////////////////////////////
//MONSTER_FILE
MONSTER_FILE::~MONSTER_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID MONSTER_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, _MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pInit) ;
	SAFE_DELETE_ARRAY(m_pAllocFlag) ;
	m_Count = 0 ;

	SAFE_DELETE_ARRAY(m_pInitUsing);
	m_CountUsing=0;

	m_OperateCount = 0 ;
	SAFE_DELETE_ARRAY(m_pOperate) ;

	m_DataCount = 0 ;
	SAFE_DELETE_ARRAY(m_pData) ;

__LEAVE_FUNCTION
}

BOOL MONSTER_FILE::Do( )
{
__ENTER_FUNCTION

	memset( m_pAllocFlag, 0, m_Count*sizeof(BOOL) ) ;

	INT i ;
	for( i=0; i<m_OperateCount; i++ )
	{
		if( !DoOneOperate(i) )
			return FALSE ;
	}

	for( i=0; i<m_Count; i++ )
	{
		if( m_pAllocFlag[i]==TRUE )
			continue ;

		m_pInitUsing[m_CountUsing]=m_pInit[i] ;
		m_CountUsing ++ ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL MONSTER_FILE::DoOneOperate( INT iIndex )
{
__ENTER_FUNCTION

//一个monster.ini文件中可以配置的怪物数量上限
#define MAX_MONSTER_FILE 512

	_OBJ_MONSTER_OPT* pOperate = &(m_pOperate[iIndex]) ;
	if( pOperate->m_DataDef>=m_DataCount )
	{
		AssertEx(FALSE,"数据定义范围超标") ;
		return FALSE ;
	}
	_OBJ_MONSTER_DATA* pData = &(m_pData[pOperate->m_DataDef]) ;
	
	INT iSelectCount = 0 ;
	INT aSelectMonsterIndex[MAX_MONSTER_FILE] ;
	memset( aSelectMonsterIndex, -1, sizeof(INT)*MAX_MONSTER_FILE ) ;

	INT i ;

	//select 符合条件的怪物索引号
	for( i=0; i<m_Count; i++ )
	{
		if( m_pAllocFlag[i]==TRUE )
		{//此怪物已经被分配
			continue ;
		}

		switch( pOperate->m_SelectType )
		{
		case MONSTER_VALUE_GUID:
			if( pOperate->m_Value.m_GUID == m_pInit[i].m_GUID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_TYPE:
			if( pOperate->m_Value.m_Type == m_pInit[i].m_uDataID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_NAME:
			if( strcmp(pOperate->m_Value.m_Name, m_pInit[i].m_szName)==0 )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_POS:
			if(    pOperate->m_Value.m_Pos.m_fX == m_pInit[i].m_Pos.m_fX 
				&& pOperate->m_Value.m_Pos.m_fZ == m_pInit[i].m_Pos.m_fZ )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_SCRIPTID:
			if( pOperate->m_Value.m_ScriptID == m_pInit[i].m_idScript )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_GROUPID:
			if( pOperate->m_Value.m_GroupID == m_pInit[i].m_uGroupID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_TEAMID:
			if( pOperate->m_Value.m_TeamID == m_pInit[i].m_uTeamID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_BASEAI:
			if( pOperate->m_Value.m_BaseAI == m_pInit[i].m_BaseAI )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_AIFILE:
			if( pOperate->m_Value.m_AIFile == m_pInit[i].m_ExtAIScript )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_PATROLID:
			if( pOperate->m_Value.m_PatrolID == m_pInit[i].m_nPatrolID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		default :
			{
				Assert(FALSE) ;
			}
			break ;
		}
	}

	if( pData->m_DataType == MONSTER_VALUE_TRAND ||
		pData->m_DataType == MONSTER_VALUE_GRAND )
	{//
		UINT uTotalRand = 0 ;
		UINT i;
		for( i=0; i<(UINT)iSelectCount; i++ )
		{
			if( pData->m_DataType == MONSTER_VALUE_TRAND )
			{
				uTotalRand += m_pInit[aSelectMonsterIndex[i]].m_uTeamID ;
			}
			else if( pData->m_DataType == MONSTER_VALUE_GRAND )
			{
				uTotalRand += m_pInit[aSelectMonsterIndex[i]].m_uGroupID ;
			}
		}
		uTotalRand==0?uTotalRand=1:uTotalRand ;

		INT nCountMin = pOperate->m_MaxLeft>iSelectCount?iSelectCount:pOperate->m_MaxLeft ;

		m_CountUsing = 0 ;
		for( INT j=0; j<nCountMin; j++ )
		{
			UINT uRandH = rand() ;
			UINT uRandL = rand() ;
			UINT uRand = ( (uRandH&0xFFFF)*0xFFFF + uRandL&0xFFFF )%uTotalRand ;

			//清除当前使用怪物初始化结构体
			m_pInitUsing[m_CountUsing].CleanUp() ;

			UINT uTotalC = 0 ;
			for( i=0; i<(UINT)iSelectCount; i++ )
			{
				if( pData->m_DataType == MONSTER_VALUE_TRAND )
				{
					uTotalC += m_pInit[aSelectMonsterIndex[i]].m_uTeamID ;
				}
				else if( pData->m_DataType == MONSTER_VALUE_GRAND )
				{
					uTotalC += m_pInit[aSelectMonsterIndex[i]].m_uGroupID ;
				}

				if( uRand < uTotalC )
				{
					m_pInitUsing[m_CountUsing] = m_pInit[aSelectMonsterIndex[i]] ;
					break ;
				}
			}

			m_CountUsing ++ ;
		}
	}
	else
	{
		//配对操作
		INT aDataIndex[MAX_MONSTER_FILE] ;
		for( i=0; i<MAX_MONSTER_FILE; i++ )
		{
			if( i>= pData->m_DataCount ) aDataIndex[i]=INVALID_INDEX ;
			else aDataIndex[i] = i ;
		}
		INT iMinCount = ((iSelectCount>pData->m_DataCount)?(pData->m_DataCount):(iSelectCount)) ;
		m_CountUsing = 0 ;
		for( i=0; i<iMinCount; i++ )
		{
			//清除当前使用怪物初始化结构体
			m_pInitUsing[m_CountUsing].CleanUp() ;
			
			//随机取一个Select出来的怪物
			INT iCurSelect = rand()%iSelectCount ;
			INT iSelectRet = INVALID_INDEX ;
			for( INT j=0; j<iSelectCount; j++ )
			{
				if( aSelectMonsterIndex[iCurSelect]!=INVALID_INDEX )
				{
					iSelectRet = aSelectMonsterIndex[iCurSelect] ;
					aSelectMonsterIndex[iCurSelect] = INVALID_INDEX ;
					break ;
				}
				iCurSelect ++ ;
				if( iCurSelect>=iSelectCount ) iCurSelect = 0 ;
			}

			//随机取一个数据
			INT iCurData = rand()%pData->m_DataCount ;
			INT iDataRet = INVALID_INDEX ;
			for( INT k=0; k<pData->m_DataCount; k++ )
			{
				if( aDataIndex[iCurData]!=INVALID_INDEX )
				{
					iDataRet = aDataIndex[iCurData] ;
					aDataIndex[iCurData] = INVALID_INDEX ;
					break ;
				}
				iCurData ++ ;
				if( iCurData >= pData->m_DataCount ) iCurData = 0 ;
			}

			Assert(iSelectRet!=INVALID_INDEX&&iDataRet!=INVALID_INDEX) ;
			m_pInitUsing[m_CountUsing] = m_pInit[iSelectRet] ;
			switch( pData->m_DataType )
			{
			case MONSTER_VALUE_GUID:
				m_pInitUsing[m_CountUsing].m_GUID = pData->m_pDataValue[iDataRet].m_GUID ;
				break ;
			case MONSTER_VALUE_TYPE:
				m_pInitUsing[m_CountUsing].m_uDataID = pData->m_pDataValue[iDataRet].m_Type ;
				break ;
			case MONSTER_VALUE_NAME:
				strncpy(m_pInitUsing[m_CountUsing].m_szName,pData->m_pDataValue[iDataRet].m_Name,NPC_NAME_LEN) ;
				break ;
			case MONSTER_VALUE_POS:
				m_pInitUsing[m_CountUsing].m_Pos.m_fX = pData->m_pDataValue[iDataRet].m_Pos.m_fX ;
				m_pInitUsing[m_CountUsing].m_Pos.m_fZ = pData->m_pDataValue[iDataRet].m_Pos.m_fZ ;
				break ;
			case MONSTER_VALUE_SCRIPTID:
				m_pInitUsing[m_CountUsing].m_idScript = pData->m_pDataValue[iDataRet].m_ScriptID ;
				break ;
			case MONSTER_VALUE_GROUPID:
				m_pInitUsing[m_CountUsing].m_uGroupID = pData->m_pDataValue[iDataRet].m_GroupID ;
				break ;
			case MONSTER_VALUE_TEAMID:
				m_pInitUsing[m_CountUsing].m_uTeamID = pData->m_pDataValue[iDataRet].m_TeamID ;
				break ;
			case MONSTER_VALUE_BASEAI:
				m_pInitUsing[m_CountUsing].m_BaseAI = pData->m_pDataValue[iDataRet].m_BaseAI ;
				break ;
			case MONSTER_VALUE_AIFILE:
				m_pInitUsing[m_CountUsing].m_ExtAIScript = pData->m_pDataValue[iDataRet].m_AIFile ;
				break ;
			case MONSTER_VALUE_PATROLID:
				m_pInitUsing[m_CountUsing].m_nPatrolID = pData->m_pDataValue[iDataRet].m_PatrolID ;
				break ;
			default :
				{
					Assert(FALSE) ;
				}
				break ;
			};//end switch

			m_CountUsing ++ ;
			if( m_CountUsing>=pOperate->m_MaxLeft )
				break ;
		}//end for
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
/////////////////////////////////////////////////////////////////////////////////
//PATROL_FILE
PATROL_FILE::~PATROL_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//BUS_FILE
VOID BUS_FILE::CleanUp(VOID)
{
	__ENTER_FUNCTION
		memset(m_szFileName, 0, sizeof(CHAR)*_MAX_PATH);
	SAFE_DELETE_ARRAY(m_paInit);
	m_nCount = 0 ;
	__LEAVE_FUNCTION
}

VOID PATROL_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_PatrolPathList) ;
	m_PatrolPathCount = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//PLATFORM_FILE
PLATFORM_FILE::~PLATFORM_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID PLATFORM_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pInit) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//AREA_FILE
AREA_FILE::~AREA_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID AREA_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pArea) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//生长点
GROWPOINT_FILE::~GROWPOINT_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID GROWPOINT_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pGrowpoint) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}


/////////////////////////////////////////////////////////////////////////////////
//
//	SceneManager
//
/////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_pScene[i]=NULL ;
		m_aScenePlayerCount[i]=0 ;
		m_aClientRes[i]=INVALID_ID ;
	}
	m_Count = 0 ;

__LEAVE_FUNCTION
}

SceneManager::~SceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		SAFE_DELETE(m_pScene[i]) ;
	}
	m_Count = 0 ;

__LEAVE_FUNCTION
}

Scene* SceneManager::GetScene( SceneID_t SceneID )
{
__ENTER_FUNCTION

	if( (uint)SceneID>=MAX_SCENE )
	{
		return NULL ;
	}

	return m_pScene[SceneID] ;

__LEAVE_FUNCTION

	return NULL ;
}

// 根据场景号得到该场景对应的资源 ID，如果没有该场景信息则返回 Invalid_id
ID_t SceneManager::GetSceneResID( SceneID_t SceneID )
{
__ENTER_FUNCTION

	if( SceneID == INVALID_ID )
	{
		return INVALID_ID;
	}

	_SCENE_DATA* pSceneData = GetSceneInfo(SceneID);
	if( pSceneData != NULL )
	{
		if( pSceneData->m_Type==SCENE_TYPE_COPY )
		{//如果是副本，则使用数据集里面的客户端资源值
		 //此值由副本创建时候通过World得到
			return m_aClientRes[SceneID] ;
		}
		else
		{
			return pSceneData->_GetClientRes();
		}
	}

	return INVALID_ID;

__LEAVE_FUNCTION

	return INVALID_ID;
}

VOID SceneManager::SetSceneResID( SceneID_t SceneID, ID_t ClientRes )
{
__ENTER_FUNCTION

	if( SceneID == INVALID_ID )
	{
		return ;
	}

	_SCENE_DATA* pSceneData = GetSceneInfo(SceneID);
	if( pSceneData!=NULL && pSceneData->m_Type==SCENE_TYPE_COPY )
	{
		m_aClientRes[SceneID] = ClientRes ;
	}

__LEAVE_FUNCTION
}

_SCENE_DATA* SceneManager::GetSceneInfo( SceneID_t SceneID )
{
__ENTER_FUNCTION

	Assert( SceneID != INVALID_ID && SceneID < g_Config.m_SceneInfo.m_SceneCount) ;

	INT iIndex = g_Config.m_SceneInfo.m_HashScene[SceneID] ;

	return &(g_Config.m_SceneInfo.m_pScene[iIndex]) ;

//	for( uint i=0; i<g_Config.m_SceneInfo.m_SceneCount; i++ )
//	{
//		if( SceneID == g_Config.m_SceneInfo.m_pScene[i].m_SceneID )
//		{
//			return &(g_Config.m_SceneInfo.m_pScene[i]) ;
//		}
//	}

__LEAVE_FUNCTION

	return NULL ;
}


BOOL SceneManager::Init( )
{
__ENTER_FUNCTION

	BOOL ret ;
	//根据配置文件，读取所有的场景数据
	//读取场景数量
	uint count = g_Config.m_SceneInfo.m_SceneCount ;
	Assert( count<=MAX_SCENE ) ;

	InitClientRes( ) ;

	for( uint i=0; i<count; i++ )
	{
		SceneID_t SceneID = (SceneID_t)(g_Config.m_SceneInfo.m_pScene[i].m_SceneID) ;
		Assert( SceneID<MAX_SCENE ) ;

		uint ServerID = g_Config.m_SceneInfo.m_pScene[i].m_ServerID ;
		if( ServerID != g_Config.m_ConfigInfo.m_ServerID )
		{//不是当前服务器的程序运行的场景
			continue ;
		}
		if( g_Config.m_SceneInfo.m_pScene[i].m_IsActive==0 )
		{//不是激活的场景
			continue ;
		}

		Scene* pScene = new Scene(SceneID) ;
		Assert( pScene ) ;

		pScene->SetSceneType( g_Config.m_SceneInfo.m_pScene[i].m_Type ) ;

		switch( pScene->GetSceneType() )
		{
		case SCENE_TYPE_GAMELOGIC://游戏逻辑场景
			{
				//read scn data
				SCENE_LOAD load ;
				SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, load ) ;
				ret = pScene->Load( &load ) ;
				Assert( ret ) ;
				//启动时候创建的场景直接进入运行模式
				//普通游戏场景没有OnSceneInit事件
				pScene->SetSceneStatus( SCENE_STATUS_RUNNING ) ;
			}
			break ;
		case SCENE_TYPE_COPY://副本场景
			{
				//空白副本是不需要添写地图资源的
				if(strlen(g_Config.m_SceneInfo.m_pScene[i].m_szFileName) > 0)
				{
					SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, pScene->m_SceneLoad ) ;
					pScene->SetMapName(pScene->m_SceneLoad.m_szMap);				
				}
				pScene->SetSceneStatus( SCENE_STATUS_SLEEP ) ;

				//将本服务器内的副本场景注册到副本管理器里面
				g_pCopySceneManager->AddCopyScene( SceneID, pScene ) ;
			}
			break ;
		case SCENE_TYPE_CIT://城市场景
			{
				//SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, pScene->m_SceneLoad ) ;
				//strncpy( pScene->GetMapName(), pScene->m_SceneLoad.m_szMap, _MAX_PATH-1 ) ;
				pScene->SetSceneStatus( SCENE_STATUS_SLEEP ) ;
			}
			break ;
		default:
			{
				Assert(FALSE) ;
			}
			break; 
		};


		ret = this->AddScene( pScene ) ;
		Assert( ret ) ;
	}



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::InitClientRes( )
{
__ENTER_FUNCTION

	uint count = g_Config.m_SceneInfo.m_SceneCount ;
	Assert( count<=MAX_SCENE ) ;

	for( UINT i=0; i<count; i++ )
	{
		//针对空白场景不添写地图资源的情况
		if(strlen(g_Config.m_SceneInfo.m_pScene[i].m_szFileName) > 0)
		{
			SCENE_LOAD load ;

			SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, load ) ;
			
			AddResNav( load.m_szMap, g_Config.m_SceneInfo.m_pScene[i]._GetClientRes() ) ;		
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::AddScene( Scene* pScene )
{
__ENTER_FUNCTION

	Assert( pScene ) ;
	if( pScene==NULL )
		return FALSE ;

	SceneID_t SceneID = pScene->SceneID() ;
	Assert( SceneID < MAX_SCENE ) ;
	
	Assert( m_pScene[SceneID]==NULL ) ;
	m_pScene[SceneID] = pScene ;

	m_Count ++ ;
	Assert( m_Count<MAX_SCENE ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::DelScene( SceneID_t SceneID )
{
__ENTER_FUNCTION

	Assert( SceneID < MAX_SCENE ) ;
	Assert( m_pScene[SceneID] ) ;
	if( m_pScene[SceneID] ) 
		return FALSE ;

	m_pScene[SceneID] = NULL ;
	m_Count -- ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::IsInCurServer( SceneID_t SceneID )
{
__ENTER_FUNCTION

	_SCENE_DATA* pData = GetSceneInfo( SceneID ) ;
	Assert( pData ) ;

	if( g_pServerManager->GetServerID()==pData->m_ServerID )
		return TRUE ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::IsInCurMachine( SceneID_t SceneID )
{
__ENTER_FUNCTION

	_SCENE_DATA* pData = GetSceneInfo( SceneID ) ;
	Assert( pData ) ;

	_SERVER_DATA* pServerData = g_pServerManager->FindServerInfo(pData->m_ServerID) ;
	Assert( pServerData ) ;

	if( g_pMachineManager->GetMachineID() == pServerData->m_MachineID )
		return TRUE ;


	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::SetLoadData( const CHAR* filename, SCENE_LOAD& SceneLoad ) 
{
__ENTER_FUNCTION

	CHAR	szTemp[_MAX_PATH], 
			szMap[_MAX_PATH],
			szMonster[_MAX_PATH],
			szBus[_MAX_PATH],
			szPlatform[_MAX_PATH],
			szGrowPointData[_MAX_PATH],
			szGrowPointSetup[_MAX_PATH],
			szPatrolPointData[_MAX_PATH],
			szArea[_MAX_PATH],
			//szPet[_MAX_PATH],
			szStallInfo[_MAX_PATH];
			
	BOOL	IsPlatformExist;
	BOOL	IsGrowPointExist;
	BOOL	IsStallInfoExist;
	BOOL	IsBusExist;
     

	SCENE_FILE* pSceneFile = FindSceneFile(filename) ;
	if( pSceneFile==NULL )
	{
		pSceneFile = FindEmptySceneFile() ;
		if( pSceneFile==NULL )
			return FALSE ;

		////////////////////////////////////////////////////////////////////////////////
		//读取场景文件 .scn
		memset( szTemp, 0, _MAX_PATH ) ;
		GET_SCENE_FULL_PATH( szTemp, filename);

		Ini f(szTemp) ;

		memset( szMap, 0, _MAX_PATH ) ;
		f.ReadText( "System", "navmapname", szMap, _MAX_PATH ) ;
		
		memset( szMonster, 0, _MAX_PATH ) ;
		f.ReadText( "System", "monsterfile", szMonster, _MAX_PATH ) ;

		memset( szBus, 0, _MAX_PATH ) ;
		IsBusExist = f.ReadTextIfExist( "System", "busfile", szBus, _MAX_PATH ) ;
		
		memset( szArea, 0, _MAX_PATH ) ;
		f.ReadText( "System", "eventfile", szArea, _MAX_PATH ) ;
		
		memset( szPlatform, 0, _MAX_PATH );
		if(NULL == f.ReadText( "System", "platformfile", szPlatform, _MAX_PATH ))
		{
			IsPlatformExist = FALSE;
		}
		else
		{
			IsPlatformExist = TRUE;
		}
		
		
		memset( szGrowPointData, 0, _MAX_PATH ) ;
		if(NULL == f.ReadText( "System", "growpointdata", szGrowPointData, _MAX_PATH ))
		{
			IsGrowPointExist=FALSE;
		}
		else
		{
			IsGrowPointExist=TRUE;
		}
		
		if(IsGrowPointExist)
		{
			memset( szGrowPointSetup, 0, _MAX_PATH ) ;
			f.ReadText( "System", "growpointsetup", szGrowPointSetup, _MAX_PATH ) ;
		}

		//摊位信息
		memset( szStallInfo, 0, _MAX_PATH ) ;
		if(NULL == f.ReadText( "System", "stallinfodata", szStallInfo, _MAX_PATH ))
		{
			IsStallInfoExist=FALSE;		
		}
		else
		{
			IsStallInfoExist=TRUE;
		}
		
		if(IsStallInfoExist)
		{
			f.ReadText( "System", "stallinfodata", szStallInfo, _MAX_PATH ) ;
		}

		memset( szPatrolPointData,0,_MAX_PATH );
		f.ReadText( "System", "patrolpoint", szPatrolPointData, _MAX_PATH );
		
		//memset(szPet, 0, _MAX_PATH);
		//f.ReadText("System", "petfile", szPet, _MAX_PATH);
	    
		SCENE_LOAD SL ;
		strncpy( SL.m_szMap, szMap, _MAX_PATH-1 ) ;
		strncpy( SL.m_szMonster, szMonster, _MAX_PATH-1 ) ;
		strncpy( SL.m_szBus, szBus, _MAX_PATH-1 ) ;
		strncpy( SL.m_szPlatform, szPlatform, _MAX_PATH-1 ) ;
		strncpy( SL.m_szGrowPointData, szGrowPointData, _MAX_PATH-1 ) ;
		strncpy( SL.m_szGrowPointSetup, szGrowPointSetup, _MAX_PATH-1 ) ;
		strncpy( SL.m_szPatrolPointData, szPatrolPointData, _MAX_PATH-1 ) ;
		strncpy( SL.m_szArea, szArea, _MAX_PATH-1 ) ;
		strncpy( SL.m_szStallInfo, szStallInfo, _MAX_PATH-1 ) ;
		SL.m_IsGrowPointExist = IsGrowPointExist ;
		SL.m_IsPlatformExist = IsPlatformExist ;
		SL.m_IsStallInfoExist = IsStallInfoExist ;
		SL.m_IsBusExist = IsBusExist ;

		pSceneFile->m_Load = SL ;
		strncpy( pSceneFile->m_szFileName, filename, _MAX_PATH-1 ) ;
	}

	SceneLoad.CleanUp() ;
	SceneLoad = pSceneFile->m_Load ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

ID_t SceneManager::GetClientResByNavName( const CHAR* szNavName )
{
__ENTER_FUNCTION

	if( szNavName==NULL || szNavName[0]==0 )
	{
		return INVALID_ID ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aSceneRes[i].m_szNavName[0]==0 )
			break ;

		if( strcmp(m_aSceneRes[i].m_szNavName, szNavName)==0 )
			return m_aSceneRes[i].m_ClientRes ;
	}

__LEAVE_FUNCTION

	return INVALID_ID ;
}

BOOL SceneManager::AddResNav( const CHAR* szNavName, ID_t ClientRes )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aSceneRes[i].m_szNavName[0]==0 )
		{
			strncpy( m_aSceneRes[i].m_szNavName, szNavName, _MAX_PATH-1 ) ;
			m_aSceneRes[i].m_ClientRes = ClientRes ;
			break ;
		}

		if( strcmp(m_aSceneRes[i].m_szNavName,szNavName)==0 )
		{
			//Assert(ClientRes==m_aSceneRes[i].m_ClientRes) ;
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

MONSTER_FILE* SceneManager::FindMonsterFile( const CHAR* szMonsterFileName )
{
__ENTER_FUNCTION

	if( szMonsterFileName==NULL || szMonsterFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aMonsterFile[i].m_pInit==NULL )
			continue ;
		if( m_aMonsterFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aMonsterFile[i].m_szFileName, szMonsterFileName)==0 )
			return &m_aMonsterFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

MONSTER_FILE* SceneManager::FindEmptyMonsterFile( )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadMonsterIni )
	{
		m_aMonsterFile[0].CleanUp() ;
		return &m_aMonsterFile[0] ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aMonsterFile[i].m_szFileName[0]==0 )
		{
			return &m_aMonsterFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

PATROL_FILE* SceneManager::FindPatrolFile( const CHAR* szPatrolFileName )
{
__ENTER_FUNCTION

	if( szPatrolFileName==NULL || szPatrolFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPatrolFile[i].m_PatrolPathList==NULL )
			continue ;
		if( m_aPatrolFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aPatrolFile[i].m_szFileName, szPatrolFileName)==0 )
			return &m_aPatrolFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

PATROL_FILE* SceneManager::FindEmptyPatrolFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPatrolFile[i].m_szFileName[0]==0 )
		{
			return &m_aPatrolFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}
 
PLATFORM_FILE* SceneManager::FindPlatformFile( const CHAR* szPlatformFileName )
{
__ENTER_FUNCTION

	if( szPlatformFileName==NULL || szPlatformFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPlatformFile[i].m_pInit==NULL )
			continue ;
		if( m_aPlatformFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aPlatformFile[i].m_szFileName, szPlatformFileName)==0 )
			return &m_aPlatformFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

PLATFORM_FILE* SceneManager::FindEmptyPlatformFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPlatformFile[i].m_szFileName[0]==0 )
		{
			return &m_aPlatformFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

BUS_FILE* SceneManager::FindBusFile(const CHAR* szBusFileName)
{
	__ENTER_FUNCTION

		if( szBusFileName==NULL || szBusFileName[0]==0 )
		{
			return NULL ;
		}

		for( INT i=0; i<MAX_SCENE; i++ )
		{
			if( m_aBusFile[i].m_paInit==NULL )
				continue ;
			if( m_aBusFile[i].m_szFileName[0]==0 )
				continue ;

			if( strcmp(m_aBusFile[i].m_szFileName, szBusFileName)==0 )
				return &m_aBusFile[i] ;
		}

		__LEAVE_FUNCTION

			return NULL ;
}

BUS_FILE* SceneManager::FindEmptyBusFile( )
{
	__ENTER_FUNCTION

		for( INT i=0; i<MAX_SCENE; i++ )
		{
			if( m_aBusFile[i].m_szFileName[0]==0 )
			{
				return &m_aBusFile[i] ;
			}
		}

		__LEAVE_FUNCTION

			return NULL ;
}

AREA_FILE* SceneManager::FindAreaFile( const CHAR* szAreaFileName )
{
__ENTER_FUNCTION

	if( szAreaFileName==NULL || szAreaFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aAreaFile[i].m_pArea==NULL )
			continue ;
		if( m_aAreaFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aAreaFile[i].m_szFileName, szAreaFileName)==0 )
			return &m_aAreaFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

AREA_FILE* SceneManager::FindEmptyAreaFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aAreaFile[i].m_szFileName[0]==0 )
		{
			return &m_aAreaFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}


GROWPOINT_FILE* SceneManager::FindGrowpointFile( const CHAR* szGrowpointFileName )
{
__ENTER_FUNCTION

	if( szGrowpointFileName==NULL || szGrowpointFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aGrowpointFile[i].m_pGrowpoint==NULL )
			continue ;
		if( m_aGrowpointFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aGrowpointFile[i].m_szFileName, szGrowpointFileName)==0 )
			return &m_aGrowpointFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

GROWPOINT_FILE* SceneManager::FindEmptyGrowpointFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aGrowpointFile[i].m_szFileName[0]==0 )
		{
			return &m_aGrowpointFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

SCENE_FILE* SceneManager::FindSceneFile( const CHAR* szSceneFileName )
{
__ENTER_FUNCTION

	if( szSceneFileName==NULL || szSceneFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aSceneLoadFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aSceneLoadFile[i].m_szFileName, szSceneFileName)==0 )
			return &m_aSceneLoadFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

SCENE_FILE* SceneManager::FindSceneFileByNavName( const CHAR* szNavName )
{
__ENTER_FUNCTION

	if( szNavName==NULL || szNavName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aSceneLoadFile[i].m_szFileName[0]==0 )
			continue ;
		if( m_aSceneLoadFile[i].m_Load.m_szMap[0]==0 )
			continue ;

		if( strcmp(m_aSceneLoadFile[i].m_Load.m_szMap, szNavName)==0 )
			return &m_aSceneLoadFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

SCENE_FILE* SceneManager::FindEmptySceneFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aSceneLoadFile[i].m_szFileName[0]==0 )
		{
			return &m_aSceneLoadFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

