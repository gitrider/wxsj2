

#ifndef __COPYSCENEMANAGER_H__
#define __COPYSCENEMANAGER_H__

#include "Type.h"
#include "Config.h"

class Scene ;

struct COPYSCENE_SELECT
{
	SCENE_LOAD			m_Load ;
	INT					m_SceneDataType ;
	COPYSCENE_DATA		m_CopyData ;

	VOID CleanUp( )
	{
		m_Load.CleanUp() ;
		m_SceneDataType=0 ;
		m_CopyData.CleanUp( ) ;
	};
};

struct LOAD_NODE
{
	Scene*				m_pScene ;

	LOAD_NODE( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		m_pScene = NULL ;
	}
};





class CopySceneManager
{
public :
	CopySceneManager( ) ;
	~CopySceneManager( ) ;

	BOOL				Init( ) ;

	SceneID_t			SelectScene( COPYSCENE_SELECT* pSelect ) ;
	BOOL				PushScene( Scene* pScene ) ;
	Scene*				PopScene( ) ;

	BOOL				HeartBeat( UINT uTime=0 ) ;
	BOOL				AddCopyScene( SceneID_t SceneID, Scene* pScene ) ;

private :
	MyLock				m_Lock ;

	LOAD_NODE			m_aLoadNode[MAX_SCENE] ;
	UINT				m_uHead ;
	UINT				m_uTail ;
	UINT				m_uTotalCount ;

	//配置在相同Server里面的副本场景
	Scene*				m_aTotalCopyScene[MAX_SCENE] ;
};

extern CopySceneManager* g_pCopySceneManager ;

#endif
