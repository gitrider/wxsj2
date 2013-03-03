#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "Type.h"
#include "LoginPlayerManager.h"
#include "Thread.h"
//���������ӽ����ͻ����߼�
//�ͻ������̹���
//ͨ���ͻ���״̬�����߼�Ǩ��

class ProcessPlayerManager;

class	ProcessManager: public Thread
{
public:

	ProcessManager();
	~ProcessManager();
	
	//��ʼ������
	BOOL			Init();
	
	//ģ�鴦��ѭ��
	virtual VOID	run();

	//ģ���˳�
	virtual VOID	stop(){	m_Active = FALSE;}
	
	//�˳�����
	VOID			Quit();

	BOOL			IsActive()	{return m_Active;}

	//*********
	//*********
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ����첽ͨѶ��Ψһ�ӿ�
	//ע�⣺pPacket��Ϣ��Ҫ��g_pPacketFactoryManager�����������������ɾ��
	BOOL			SendPacket( Packet* pPacket, 
								PlayerID_t PlayerID, 
								uint Flag=PF_NONE ) ;

private:
	BOOL			m_Active;

	uint			m_CurrentTime;
	INT				m_LogTurnTime;
private :
	
	//��ǰģ�����Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
	MyLock					m_Lock ;

private:
	BOOL					DoTick(UINT uTime);

	//��������Ϣ
	BOOL					ProcessCacheCommands( ) ;

	//��ȡ������Ϣ
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//���µ��������С
	BOOL					ResizeCache( ) ;

	//ɾ��ĳ��Player������Ϣ�����е���Ϣ
	BOOL					MovePacket( PlayerID_t PlayerID ) ;

	//����������Ҽ��뵽�߼�����
	BOOL					MoveQueuePlayer();
	BOOL					ResetQueuePos();
	

};


extern ProcessManager*	g_pProcessManager;






#endif
