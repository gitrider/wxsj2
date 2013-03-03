#ifndef __COMMANDTHREAD_H__
#define __COMMANDTHREAD_H__

#include "Type.h"
#include "Thread.h"







//������������������
class CommandThread : public Thread
{
public :
	CommandThread( ){m_Active = TRUE;} 
	~CommandThread( ){} ;

	//��ѭ��
	virtual VOID	run( ) ;

	//ֹͣ�߳�ִ��
	virtual VOID	stop( ) { m_Active = FALSE ; } ;

	//�жϵ�ǰ�߳��Ƿ񼤻�
	BOOL			IsActive( ){ return m_Active ; } ;

	GLOBAL_CONFIG	m_SMConfig;	
private :
	BOOL			m_Active ;
	
};


extern CommandThread g_CommandThread;




class CmdParser
{
public:
	static  BOOL	HandleInput();

private:
	static SM_COMMANDS_STATE	    ParserCommandType(CHAR* pBuff);
};



#endif