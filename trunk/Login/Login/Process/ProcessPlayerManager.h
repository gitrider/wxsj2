#ifndef _PROCESS_PLAYER_MANAGER_H_
#define _PROCESS_PLAYER_MANAGER_H_
#include "Type.h"
#include "PlayerManager.h"
#include "StrTable.h"


#define MAX_PROCESS_PLAYER 1024
#define MAX_NORMAL_PLAYER  256

//��½���̵Ŀͻ��˹�����
class ProcessPlayerManager:public PlayerManager
{
public:
	
	ProcessPlayerManager( ) ;
	~ProcessPlayerManager( ) ;

	//�������ӿ�
	BOOL				Select( ) ;
	//���ݽ��մ���
	BOOL				ProcessInputs( ) ;
	//���ݷ��ʹ���
	BOOL				ProcessOutputs( ) ;
	//�쳣���Ӵ���
	BOOL				ProcessExceptions( ) ;
	//��Ϣ����
	BOOL				ProcessCommands( ) ;
	//�߼�����
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	Player*				FindAccName(const CHAR* szAccount);
public:

	BOOL				AddPlayer( Player* pPlayer ) ;
	//ɾ��һ����ң���������
	BOOL				DelPlayer( PlayerID_t pid ) ;

	BOOL				AddPlayerSocket( SOCKET fd ) ;
	BOOL				DelPlayerSocket( SOCKET fd ) ;

	//�Ͽ�һ����ҡ�����������
	BOOL				RemovePlayer( Player* pPlayer) ;
	VOID				RemoveAllPlayer( );

	//VOID				Lock(){ m_Lock.Lock() ; } ;
	//VOID				Unlock(){ m_Lock.Unlock() ; } ;

	INT					GetCount( ){ return m_nFDSize ; } ;

	UINT				GetNormalPlayerCount()
	{
		return m_NormalPlayers;
	}

	VOID				IncNormalPlayerCount()
	{
		m_NormalPlayers++;
	}

	VOID				DecNormalPlayerCount()
	{
		if(m_NormalPlayers>0)
			m_NormalPlayers--;
	}

	TID					m_ThreadID ;
private:
	//��������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set				m_ReadFDs[SELECT_MAX];
	fd_set				m_WriteFDs[SELECT_MAX];
	fd_set				m_ExceptFDs[SELECT_MAX];
	timeval				m_Timeout[SELECT_MAX];
	SOCKET				m_MinFD;
	SOCKET				m_MaxFD;
	INT					m_nFDSize ;
	//MyLock				m_Lock ;

	UINT				m_NormalPlayers;
	StrTable			m_AccTable;
};	

extern ProcessPlayerManager*	g_pProcessPlayerManager;

#endif