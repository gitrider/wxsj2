

#ifndef __SCENETHREAD_H__
#define __SCENETHREAD_H__

#include "Type.h"
#include "Thread.h"
#include "Scene.h"

#define MAX_SCENE_PER_THREAD 128

//�����߳�
//ÿ��������һ���������߳�������ִ��
class SceneThread : public Thread
{
public :
	SceneThread( ) ;
	~SceneThread( ) ;

	//�߳�ִ����ѭ��
	virtual VOID	run( ) ;

	//ִֹͣ��
	//���ô˽ӿں��������߳̾���ֹͣ����������һ��ѭ������ʱ�˳�
	virtual VOID	stop( ) { m_Active = FALSE ; } ;

	//�жϵ�ǰ�߳��Ƿ���Ч�������Ч�����˳�ѭ��
	BOOL			IsActive( ){ return m_Active ; } ;

	VOID			Quit( ) ;

	BOOL			AddScene( Scene* pScene ) ;

private :
	Scene*		m_apScene[MAX_SCENE_PER_THREAD] ;
	INT			m_nSceneCount ;
	BOOL		m_Active ;



};




#endif
