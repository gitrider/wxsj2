
#ifndef __LOGINPLAYERMANAGER_H__
#define __LOGINPLAYERMANAGER_H__

#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"
#include "Thread.h"


class LoginPlayerManager : public PlayerManager
{
public :
	LoginPlayerManager() ;
	~LoginPlayerManager() ;

	//ģ���ʼ���ӿ�
	BOOL				Init( ) ;
	//�������
	BOOL				Select( ) ;
	//���ݽ��ܽӿ�
	BOOL				ProcessInputs( ) ;
	//���ݷ��ͽӿ�
	BOOL				ProcessOutputs( ) ;
	//�쳣���Ӵ���
	BOOL				ProcessExceptions( ) ;
	//��Ϣִ��
	BOOL				ProcessCommands( ) ;
	//�����ӽ��մ���
	BOOL				AcceptNewConnection( ) ;
	//�߼��ӿ�
	virtual BOOL		HeartBeat( ) ;
	//��������Ϣ
	BOOL				ProcessCacheCommands( ) ;
	//��ȡ������Ϣ
	BOOL				RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;
	//���µ��������С
	BOOL				ResizeCache( ) ;
	//ɾ����ЧPlayer����Ϣ
	BOOL				MovePacket( PlayerID_t PlayerID ) ;

public :
	//ͨ�ýӿ�

	//��Player���ݼ���ϵͳ��
	BOOL				AddPlayer( Player* pPlayer ) ;
	//��ӵ��fd�����������ݴӵ�ǰϵͳ�����
	BOOL				DelPlayer( PlayerID_t pid ) ;

	//�����쳣��Player��Ϣ���������ϵͳ�е���ϢҲ���
	//�Ͽ���ҵ�����
	BOOL				RemovePlayer( Player* pPlayer ) ;
	VOID				RemoveAllPlayer( ) ;

	//*********
	//*********
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ����첽ͨѶ��Ψһ�ӿ�
	//ע�⣺pPacket��Ϣ��Ҫ��g_pPacketFactoryManager�����������������ɾ��
	BOOL				SendPacket( Packet* pPacket, 
									PlayerID_t PlayerID, 
									uint Flag=PF_NONE ) ;
	//*********
	//*********

private :
	//���������ķ�����Socket
	ServerSocket*		m_pServerSocket ;
	//���������ķ�����SOCKET���ֵ�������ݼ�m_pServerSocket��ӵ�е�SOCKET���ֵ��
	SOCKET				m_SocketID ;

	//
	//�����������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET					m_MaxFD;

	INT						m_nFDSize ;
	//�����������
	//

	MyLock					m_Lock ;


	//��ǰ����Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;

public :
	TID			m_ThreadID ;

};

extern LoginPlayerManager* g_pLoginPlayerManager ;

#endif
