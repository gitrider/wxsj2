/////////////////////////////////////////////////////////////////////////////////
//�ļ�����SceneInfo.h
//�����ߣ�
//����������ͳ�Ƴ�����������Ϣ
//�޸ļ�¼��2006-2-13 ���� 
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
	INT					m_aScenePlayerCount[MAX_SCENE] ;	//��ǰ�ĳ���ʵ������
	INT					m_aScenePlayerCount_Bak[MAX_SCENE] ;//�ϴ��߼��ĳ�������

	ID_t				m_aSceneClientRes[MAX_SCENE] ;//ÿ�����������Ŀͻ�����Դ��

	CMyTimer			m_LoginTimer ;


};

extern SceneInfo*		g_pSceneInfo ;
extern INT				g_nEnterUserLastMinite ;
extern INT				g_nLeaveUserLastMinite ;


#endif
