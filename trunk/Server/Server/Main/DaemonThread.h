
/////////////////////////////////////////////////////////////////////////////////
//�ļ�����DaemonThread.h
//�����ߣ�jack
//�����������ػ��̣߳����ڼ�ص�ǰ������������Ϣ���Լ�������ݱ��洦��
//�޸ļ�¼��2005-11-10����
/////////////////////////////////////////////////////////////////////////////////

#ifndef __DAEMONTHREAD_H__
#define __DAEMONTHREAD_H__

#include "Type.h"


class DaemonThread
{
public :
	DaemonThread( ) ;
	~DaemonThread( ) ;

	BOOL					Init( ) ;
	VOID					CleanUp( ) ;

	BOOL					Loop( ) ;

	VOID					Stop( ){ m_Active = FALSE ; }

	//ģ���˳�����
	VOID					Quit( ) ;

	//�жϵ�ǰģ���Ƿ��ڻ״̬
	BOOL			IsActive( ){ return m_Active ; } ;

private :
	BOOL			m_Active ;//�Ƿ��ı�־
	CMyTimer		m_WorkingTime ;//

	CMyTimer		m_FlushLogTimer ;
};

extern DaemonThread* g_pDaemonThread ;

#endif
