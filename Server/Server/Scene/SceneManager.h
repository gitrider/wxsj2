

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Type.h"
#include "Scene.h"
#include "IDTable.h"
#include "Config.h"

struct SCENE_FILE
{
	CHAR				m_szFileName[_MAX_PATH] ;	//.scn文件名
	SCENE_LOAD			m_Load ;//scn文件里的数据

	SCENE_FILE( )
	{
		memset( m_szFileName, 0, _MAX_PATH ) ;
	}
};

struct SCENE_RES
{
	CHAR				m_szNavName[_MAX_PATH] ;	//.nav文件名
	ID_t				m_ClientRes ;//nav文件对应的客户端资源号

	SCENE_RES( )
	{
		memset( m_szNavName, 0, _MAX_PATH ) ;
		m_ClientRes = INVALID_ID ;
	}
};

/////////////////////////////////////////////////////////////////////////////////
//怪物数据
struct _OBJ_MONSTER_INIT ;
struct FLOAT_POS
{
	FLOAT	 m_fX ;
	FLOAT	 m_fZ ;
};
enum MONSTER_VALUE
{
	MONSTER_VALUE_GUID		= 0 ,
	MONSTER_VALUE_TYPE		= 1 ,
	MONSTER_VALUE_NAME		= 2 ,
	MONSTER_VALUE_POS		= 3 ,
	MONSTER_VALUE_SCRIPTID	= 4 ,
	MONSTER_VALUE_GROUPID	= 5 ,
	MONSTER_VALUE_TEAMID	= 6 ,
	MONSTER_VALUE_BASEAI	= 7 ,
	MONSTER_VALUE_AIFILE	= 8 ,
	MONSTER_VALUE_PATROLID	= 9 ,
	MONSTER_VALUE_TRAND		= 10 ,
	MONSTER_VALUE_GRAND		= 11 ,
};
struct MONSTER_ATTR_VALUE
{
	union
	{
		GUID_t			m_GUID ;
		INT				m_Type ;
		CHAR			m_Name[NPC_NAME_LEN+1] ;
		FLOAT_POS		m_Pos ;
		ScriptID_t		m_ScriptID ;
		GroupID_t		m_GroupID ;
		TeamID_t		m_TeamID ;
		INT				m_BaseAI ;
		INT				m_AIFile ;
		INT				m_PatrolID ;
	};
};
struct _OBJ_MONSTER_OPT
{
	INT					m_SelectType ;
	MONSTER_ATTR_VALUE	m_Value ;
	INT					m_DataDef ;
	INT					m_MaxLeft ;
};
struct _OBJ_MONSTER_DATA
{
	INT					m_DataType ;
	INT					m_DataCount ;
	MONSTER_ATTR_VALUE*	m_pDataValue ;

	_OBJ_MONSTER_DATA()
	{
		m_DataType = 0 ;
		m_DataCount = 0 ;
		m_pDataValue = NULL ;
	};
	~_OBJ_MONSTER_DATA()
	{
		SAFE_DELETE_ARRAY(m_pDataValue) ;
	};
};
struct MONSTER_FILE
{
	CHAR				m_szFileName[_MAX_PATH] ;	//monster.ini文件名
	_OBJ_MONSTER_INIT*	m_pInit ;					//怪物初始化数据结构（编辑器定义的总量）
	BOOL*				m_pAllocFlag ;				//每个怪物是否分配使用的标志
	INT					m_Count ;					//怪物数量
	_OBJ_MONSTER_INIT*	m_pInitUsing ;				//实际使用的怪物初始化数据
	INT					m_CountUsing ;				//实际使用的怪物数量
	INT					m_OperateCount ;			//转换操作数量
	_OBJ_MONSTER_OPT*	m_pOperate ;				//转换操作数据结构
	INT					m_DataCount ;				//数据区数量
	_OBJ_MONSTER_DATA*	m_pData ;					//数据区结构

	MONSTER_FILE( )
	{
		memset( m_szFileName, 0, _MAX_PATH ) ;
		m_pInit = NULL ;
		m_pAllocFlag = NULL ;
		m_Count = 0 ;
		m_pInitUsing = NULL ;
		m_CountUsing = 0 ;

		m_OperateCount = 0 ;
		m_pOperate = NULL ;

		m_DataCount = 0 ;
		m_pData = NULL ;
	};
	~MONSTER_FILE( ) ;

	VOID				CleanUp( ) ;

	BOOL				Do( ) ;

protected :
	BOOL				DoOneOperate( INT iIndex ) ;
};


/////////////////////////////////////////////////////////////////////////////////
//BUS OBJ数据
struct _OBJ_BUS_INIT ;
struct BUS_FILE
{
	CHAR					m_szFileName[_MAX_PATH] ;	//platform.ini文件名
	_OBJ_BUS_INIT			*m_paInit ;
	INT						m_nCount ;


	BUS_FILE(VOID)
	{
		memset(m_szFileName, 0, sizeof(CHAR)*_MAX_PATH);
		m_paInit = NULL ;
		m_nCount = 0 ;
	}

	~BUS_FILE(VOID)
	{
		CleanUp();
	}

	VOID CleanUp(VOID);
};


/////////////////////////////////////////////////////////////////////////////////
//巡逻路径数据
struct PatrolPath ;
struct PATROL_FILE
{
	CHAR				m_szFileName[_MAX_PATH] ;	//patrol.ini文件名
	PatrolPath*			m_PatrolPathList ;			//路径数量
	INT					m_PatrolPathCount ;			//路径数量

	PATROL_FILE()
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_PatrolPathList = NULL ;
		m_PatrolPathCount = 0 ;
	};
	~PATROL_FILE() ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//操作台数据
struct _OBJ_PLATFORM_INIT ;
struct PLATFORM_FILE
{
	CHAR					m_szFileName[_MAX_PATH] ;	//platform.ini文件名
	_OBJ_PLATFORM_INIT*		m_pInit ;
	INT						m_Count ;

	PLATFORM_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pInit = NULL ;
		m_Count = 0 ;
	}
	~PLATFORM_FILE( ) ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//事件区数据
struct Area ;
struct AREA_FILE
{
	CHAR		m_szFileName[_MAX_PATH] ;	//area.ini文件名
	Area*		m_pArea ;
	INT			m_Count ;

	AREA_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pArea = NULL ;
		m_Count = 0 ;
	};
	~AREA_FILE( ) ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//生长点数据
struct GROWPOINT_LOAD ;
struct GROWPOINT_FILE
{
	CHAR			m_szFileName[_MAX_PATH] ;	//growpoint.ini文件名
	GROWPOINT_LOAD*	m_pGrowpoint ;
	INT				m_Count ;

	GROWPOINT_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pGrowpoint = NULL ;
		m_Count = 0 ;
	}
	~GROWPOINT_FILE( ) ;
	VOID CleanUp( ) ;
};


/////////////////////////////////////////////////////////////////////////////////
// SceneManager
/////////////////////////////////////////////////////////////////////////////////
class SceneManager
{
public :
	SceneManager( ) ;
	~SceneManager( ) ;

	//初始化接口
	BOOL			Init( ) ;

	//初始化客户端资源信息
	BOOL			InitClientRes( ) ;

	//根据场景号取得场景对象指针
	Scene*			GetScene( SceneID_t SceneID ) ;
	
	//得到当前场景数量
	uint			GetSceneCount() { return m_Count; }

	// 根据场景号得到该场景对应的资源 ID，如果没有该场景信息则返回 Invalid_id
	ID_t			GetSceneResID( SceneID_t SceneID );

	//将某个副本场景号和某个客户端资源号挂接
	VOID			SetSceneResID( SceneID_t SceneID, ID_t ClientRes );
	
	//根据场景号取得场景配置信息
	_SCENE_DATA*	GetSceneInfo( SceneID_t SceneID ) ;
	//根据场景号判断此场景是否在当前服务器端程序处理
	BOOL			IsInCurServer( SceneID_t SceneID ) ;
	//根据场景号判断此场景是否被当前服务器上的某个服务器端程序处理
	BOOL			IsInCurMachine( SceneID_t SceneID ) ;

	//向系统内添加一个场景
	BOOL			AddScene( Scene* pScene ) ;
	//将一个场景从系统中删除
	BOOL			DelScene( SceneID_t SceneID ) ;

	//设置某个场景中的人数
	VOID			SetScenePlayerCount( SceneID_t SceneID, INT count ){
		if( SceneID < 0 && SceneID>=MAX_SCENE ) return ;
		m_aScenePlayerCount[SceneID] = count ;
	};
	//读取某个场景中的人数
	INT				GetScenePlayerCount( SceneID_t SceneID ){
		if( SceneID < 0 && SceneID>=MAX_SCENE ) return 0 ;
		return m_aScenePlayerCount[SceneID] ;
	};

	VOID			SetTotalPlayerCount(INT nCount){ m_nTotalPlayerCount=nCount; } ;
	INT				GetTotalPlayerCount(){ return m_nTotalPlayerCount ; } ;

	//读取某个.scn文件的内容
	BOOL			SetLoadData( const CHAR* filename, SCENE_LOAD& SceneLoad ) ;
		//根据nav文件名得到对应的客户端资源号
	ID_t			GetClientResByNavName( const CHAR* szNavName ) ;
	//向数据集里增加.nav文件和客户端资源号的对应数据对
	BOOL			AddResNav( const CHAR* szNavName, ID_t ClientRes ) ;

	MONSTER_FILE*	FindMonsterFile( const CHAR* szMonsterFileName ) ;
	MONSTER_FILE*	FindEmptyMonsterFile( ) ;

	PATROL_FILE*	FindPatrolFile( const CHAR* szPatrolFileName ) ;
	PATROL_FILE*	FindEmptyPatrolFile( ) ;

	PLATFORM_FILE*	FindPlatformFile( const CHAR* szPlatformFileName ) ;
	PLATFORM_FILE*	FindEmptyPlatformFile( ) ;

	BUS_FILE*		FindBusFile( const CHAR* szBusFileName ) ;
	BUS_FILE*		FindEmptyBusFile( ) ;

	AREA_FILE*		FindAreaFile( const CHAR* szAreaFileName ) ;
	AREA_FILE*		FindEmptyAreaFile( ) ;

	GROWPOINT_FILE*	FindGrowpointFile( const CHAR* szGrowpointFileName ) ;
	GROWPOINT_FILE*	FindEmptyGrowpointFile( ) ;

	SCENE_FILE*		FindSceneFile( const CHAR* szSceneFileName ) ;
	SCENE_FILE*		FindSceneFileByNavName( const CHAR* szNavName ) ;
	SCENE_FILE*		FindEmptySceneFile( ) ;

private :
	Scene*			m_pScene[MAX_SCENE] ;			//场景数据指针
	INT				m_aScenePlayerCount[MAX_SCENE] ;//场景中人数
	uint			m_Count ;//当前系统中的场景数量

	//nav地图对应的客户端资源号
	//注：针对普通场景有效
	SCENE_RES		m_aSceneRes[MAX_SCENE] ;
	//场景号对应客户端资源号，数组下标为场景号
	//注：针对副本场景有效
	ID_t			m_aClientRes[MAX_SCENE] ;

	MONSTER_FILE	m_aMonsterFile[MAX_SCENE] ;//怪物*_monster.ini的数据，原始数据
	PATROL_FILE		m_aPatrolFile[MAX_SCENE] ;//巡逻路径*partrol.ini的数据，原始数据
	PLATFORM_FILE	m_aPlatformFile[MAX_SCENE] ;//操作台*platform.ini的数据，原始数据
	AREA_FILE		m_aAreaFile[MAX_SCENE] ;//事件区*area.ini的数据，原始数据
	GROWPOINT_FILE	m_aGrowpointFile[MAX_SCENE] ;//生长点原始数据
	SCENE_FILE		m_aSceneLoadFile[MAX_SCENE] ;//.scn原始数据
	BUS_FILE		m_aBusFile[MAX_SCENE] ;		//Bus数据*bus.ini的数据，原始数据
	INT				m_nTotalPlayerCount ;//整个游戏世界的人数总和
};

extern SceneManager* g_pSceneManager ;


#endif
