#ifndef __RECOVERTHREAD_H__
#define __RECOVERTHREAD_H__

#include "Type.h"
#include "Thread.h"


//�������������Crash���������ݵ��߳�


class RecoverThread : public Thread
{
public :
	RecoverThread( ){} ;
	~RecoverThread( ){} ;

	//��ѭ��
	virtual VOID	run( ) ;

	//ֹͣ�߳�ִ��
	virtual VOID	stop( ) { m_bActive = FALSE ; } ;

	//�жϵ�ǰ�߳��Ƿ񼤻�
	BOOL			IsActive( ){ return m_bActive ; } ;
private :
	//�Ƿ񼤻�
	BOOL		m_bActive ;


};





#endif