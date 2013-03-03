

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include "Type.h"
#include "ServerThread.h"



//�������߳����⣬�����̶߳��ɴ�ģ��������
//Login �˵� ThreadManager ȡ��ThreadPool
//��Ϊ����Ҫ�ܳ��� 
class ThreadManager
{
public :
	ThreadManager( ) ;
	~ThreadManager( ) ;

	//��ʼ��
	BOOL				Init( ) ;
	//���������߳�
	BOOL				Start( ) ;
	//ֹͣ�����߳�
	BOOL				Stop( ) ;
	
	//ȡ�õ�ǰ�ķ������߳�
	ServerThread*		GetServerThread(){ 
		return m_pServerThread ; 
	} ;
	//ȡ�õ�ǰ���е��߳�����
	uint				GetTotalThreads(){ return m_nThreads ; } ;
protected :
	ServerThread*		m_pServerThread ;
	uint				m_nThreads ;

};
extern ThreadManager*	g_pThreadManager ;




#endif
