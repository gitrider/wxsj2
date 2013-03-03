#ifndef _DBTHREAD_H_
#define _DBTHREAD_H_

#include "Type.h"
#include "Thread.h"
#include "Packet.h"

class ODBCInterface;

struct  IN_PACKET 
{
	Packet*			m_pPacket;
	PlayerID_t		m_PlayerID;

	IN_PACKET()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		m_pPacket	=	 NULL;
		m_PlayerID	=	 INVALID_ID;
	}
};

class DBThread: public Thread
{
public:
	DBThread();
	virtual ~DBThread();

	//��ѭ��
	virtual		VOID		run();
	//ֹͣ�߳�ִ��
	virtual VOID			stop( ) { m_Active = FALSE ; } ;
	//�жϵ�ǰ�߳��Ƿ񼤻�
	BOOL					IsActive( )
	{
		return (m_Active && IsConnected()); 
	} ;

	BOOL					IsConnected();

	//����һ��DB������Ϣ
	BOOL					SendPacket(Packet* pPacket,PlayerID_t pID);
	//��ȡ������Ϣ
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID) ;
	
	//��ʼ������
	BOOL					Init();

	ODBCInterface*			GetInterface();
private:
	BOOL					ProcessCacheCommand();
private :
	
	BOOL					m_Active ;
	MyLock					m_Lock ;
	IN_PACKET				m_Input;
	//Ĭ�����ݿ�����
	ODBCInterface*			mCharDBInterface;


};


#endif