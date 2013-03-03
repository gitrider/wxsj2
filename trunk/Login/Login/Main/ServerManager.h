

#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "Config.h"
#include "ServerSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "ServerPlayer.h"
#include "BillingPlayer.h"


#define WORLD_PLAYER_ID			0
#define BILLING_PLAYER_ID		1

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
	BOOL				RemoveServer(PlayerID_t id) ;
	//��ȡָ����ŵ�ServerPlayer 
	Player*				GetServerPlayer(PlayerID_t id);
	//���ӵ�����������˳���
	BOOL				ConnectWorldServer( ) ;
	//���ӵ�Billing����������
	BOOL				ConnectBillingServer( );

	//ȡWorldQueue�û����������ݵ�World
	BOOL				SendQueuePlayerToWorld();
	BOOL				ResetQueuePos();
public :
	//******************************************************************************
    //ͨ�ýӿ�
	//******************************************************************************

	//ȡ�õ�ǰ�ķ������˳���ID
	ID_t				GetLoginID( ) ;
	//ȡ�õ�ǰ�ķ������˳�����Ϣ
	_SERVER_DATA*		GetCurrentServerInfo( ) ;
	//ȡ��ָ���ķ������˳������Ϣ
	_SERVER_DATA*		FindServerInfo( ID_t ServerID ) ;

	//�ж�ָ���ķ������������Ƿ��ڻ״̬
	BOOL				IsWorldServerActive( ) ;
	BOOL				IsBillingServerActive( );

	//���ģ�鷢����Ϣ
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ��������������ķ�����������Ϣ��Ψһ�ӿ�
	BOOL				SendPacket( Packet* pPacket , 
									ID_t ServerID , 
									uint Flag=PF_NONE ) ;

	//����ĳ�������е�����
	VOID			SetScenePlayerCount( SceneID_t SceneID, INT count ){
		if( SceneID < 0 || SceneID>=MAX_SCENE ) return ;
		m_aScenePlayerCount[SceneID] = count ;
	};
	//��ȡĳ�������е�����
	INT				GetScenePlayerCount( SceneID_t SceneID ){
		if( SceneID < 0 || SceneID>=MAX_SCENE ) return 0 ;
		return m_aScenePlayerCount[SceneID] ;
	};

	INT					GetServerPlayerPoolSize( ID_t ServerID ) ;
	VOID				SetServerPlayerPoolSize( ID_t ServerID, INT nPlayerPoolSize ) ;

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
	//ServerSocket*	m_pServerSocket ;
	//SOCKET			m_SocketID ;

	//��ǰ�������˳������Ϣ
	_SERVER_DATA*	m_pCurServerInfo ;

	//��ǰ��Ϸ���������õ����з������˳�Ա, 
	//ÿ���������˳������������з������˳�����һ����Ӧ��tcp����
	//��������±꼴Ϊ��ǰ���ӵ�PlayerID_t
	ServerPlayer	m_WorldPlayer;

	BillingPlayer	m_BillingPlayer;

	//���ٲ�������
	//�����±�ֵ��ServerID, ����������ServerPlayerID(����m_pServerPlayer[]���±�)
	PlayerID_t		m_pServerHash[MAX_SERVER] ;

	//ÿ���߼������ڵ�����
	INT				m_aScenePlayerCount[MAX_SCENE] ;//����������

	//��ǰ����ÿ��Server��ʹ�õ���ҳص������������±�ΪServerID ;
	INT				m_aServerPlayerPoolSize[MAX_SERVER] ;

	//��Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
	BOOL					m_KickALL;

	uint			m_CurrentTime;
	INT				m_LogTurnTime;
public :
	TID						m_ThreadID ;
};

extern ServerManager* g_pServerManager ;

#endif
