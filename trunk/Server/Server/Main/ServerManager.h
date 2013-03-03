

#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "Config.h"
#include "ServerSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "ServerPlayer.h"


//ϵͳ�ڿ���ӵ�е����ķ������˳�������
#define MAX_SERVER 24

class ServerManager
{
public :
	ServerManager( ) ;
	~ServerManager( ) ;

	VOID				CleanUp( ) ;

	//��ʼ��
	BOOL				Init( ) ;

	//���߼��ӿ�
	BOOL				Tick( ) ;

	//�����������
	BOOL				Select( ) ;
	//���ݽ��ջ��������ӽ���
	BOOL				ProcessInputs( ) ;
	//���ݷ���
	BOOL				ProcessOutputs( ) ;
	//�����쳣�Ͽ�����
	BOOL				ProcessExceptions( ) ;
	//��Ϣ����
	BOOL				ProcessCommands( ) ;

	//�߼�����
	virtual BOOL		HeartBeat( ) ;

	BOOL				ProcessCacheCommands( ) ;

	BOOL				RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;
	//���µ��������С
	BOOL				ResizeCache( ) ;

public :
	//������ע�ᣨֻ��ע�������������ܽ������ݣ�
	BOOL				AddServer( SOCKET fd ) ;
	//���ӳ���
	BOOL				DelServer( SOCKET fd ) ;
	//�Ͽ����Ӻ��������մ���
	BOOL				RemoveServer( ) ;
	//���ӵ�ָ���ķ������˳���
	//indexָ���Ƿ����������ݵ�������m_pServerPlayer��
	BOOL				ConnectWorldServer( ) ;

public :
	//******************************************************************************
    //ͨ�ýӿ�
	//******************************************************************************

	//ȡ�õ�ǰ�ķ������˳���ID
	ID_t				GetServerID( ) ;
	//ȡ�õ�ǰ�ķ������˳�����Ϣ
	_SERVER_DATA*		GetCurrentServerInfo( ) ;
	//ȡ��ָ���ķ������˳������Ϣ
	_SERVER_DATA*		FindServerInfo( ID_t ServerID ) ;
	//ȡ�õ�ǰ������ShareMem״̬
	BOOL				IsEnableShareMem();	

	//�ж�ָ���ķ������������Ƿ��ڻ״̬
	BOOL				IsWorldServerActive( ) ;

	//���ģ�鷢����Ϣ
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ��������������ķ�����������Ϣ��Ψһ�ӿ�
	BOOL				SendPacket( Packet* pPacket , 
									ID_t ServerID , 
									uint Flag=PF_NONE ) ;

	VOID				Lock(){ m_Lock.Lock() ; } ;
	VOID				Unlock(){ m_Lock.Unlock() ; } ;


private :
	//
	//��������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set			m_ReadFDs[SELECT_MAX];
	fd_set			m_WriteFDs[SELECT_MAX];
	fd_set			m_ExceptFDs[SELECT_MAX];
	
	timeval			m_Timeout[SELECT_MAX];

	SOCKET			m_MinFD;
	SOCKET			m_MaxFD;

	INT				m_nFDSize ;
	//��������
	//

	MyLock			m_Lock ;

	//��ǰ���������������ӵ�Socket
	ServerSocket*	m_pServerSocket ;
	SOCKET			m_SocketID ;

	//��ǰ�������˳������Ϣ
	_SERVER_DATA*	m_pCurServerInfo ;

	//��ǰ��Ϸ���������õ����з������˳�Ա, 
	//ÿ���������˳������������з������˳�����һ����Ӧ��tcp����
	//��������±꼴Ϊ��ǰ���ӵ�PlayerID_t
	ServerPlayer	m_ServerPlayer ;

	//���ٲ�������
	//�����±�ֵ��ServerID, ����������ServerPlayerID(����m_pServerPlayer[]���±�)
	PlayerID_t		m_pServerHash[MAX_SERVER] ;

	//��Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;

	BOOL					m_IsConnectedWorld ;//�Ƿ��Ѿ����ӹ�World

public :
	TID						m_ThreadID ;
};

extern ServerManager* g_pServerManager ;

#endif
