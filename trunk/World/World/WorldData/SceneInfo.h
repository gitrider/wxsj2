/////////////////////////////////////////////////////////////////////////////////
//文件名：SceneInfo.h
//开发者：
//功能描述：统计场景中人数信息
//修改记录：2006-2-13 创建 
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCENEINFO_H__
#define __SCENEINFO_H__


#include "Type.h"



class SceneInfo
{
public :
	SceneInfo( ) ;
	~SceneInfo( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	VOID				IncScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] ++ ;
	}
	VOID				DecScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] -- ;
	}

	ID_t				GetSceneClientRes( SceneID_t SceneID ) ;
	VOID				SetSceneClientRes( SceneID_t SceneID, ID_t ClientRes ) ;

	BOOL				HeartBeat( UINT uTime ) ;


protected :
	INT					m_aScenePlayerCount[MAX_SCENE] ;	//当前的场景实际人数
	INT					m_aScenePlayerCount_Bak[MAX_SCENE] ;//上次逻辑的场景人数

	ID_t				m_aSceneClientRes[MAX_SCENE] ;//每个副本场景的客户端资源号

	CMyTimer			m_LoginTimer ;


};

extern SceneInfo*		g_pSceneInfo ;
extern INT				g_nEnterUserLastMinite ;
extern INT				g_nLeaveUserLastMinite ;


#endif
