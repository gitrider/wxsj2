

#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include "Type.h"
#include "Thread.h"


//�������������֮������ͨѶ���߳�

class ServerThread : public Thread
{
public :
	ServerThread( ) ;
	~ServerThread( ) ;

	//��ѭ��
	virtual VOID	run( ) ;

	//ֹͣ�߳�ִ��
	virtual VOID	stop( ) { m_Active = FALSE ; } ;

	//�жϵ�ǰ�߳��Ƿ񼤻�
	BOOL			IsActive( ){ return m_Active ; } ;



private :
	BOOL		m_Active ;


};





#endif
