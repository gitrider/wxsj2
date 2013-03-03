
#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"
#include "Thread.h"
#include "Config.h"


class ServerPlayer ;

class ServerManager : public PlayerManager
{
public :
	ServerManager() ;
	~ServerManager() ;

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

	void				Loop( ) ;

	BOOL				IsActive( ){ return m_bActive ; } ;
	void				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;

public :
	//ͨ�ýӿ�

	//��Player���ݼ���ϵͳ��
	BOOL				AddPlayer( Player* pPlayer ) ;
	//��ӵ��fd�����������ݴӵ�ǰϵͳ�����
	BOOL				DelPlayer( Player* pPlayer ) ;

	//�����쳣��Player��Ϣ���������ϵͳ�е���ϢҲ���
	//�Ͽ���ҵ�����
	BOOL				RemovePlayer( Player* pPlayer ) ;
	void				RemoveAllPlayer( ) ;

	//ͨ��ServerIDȡ��ServerPlayer
	ServerPlayer*		GetServerPlayer( ID_t ServerID ) ;

	//�����з������˳���㲥��Ϣ
	void				BroadCast( Packet* pPacket ) ;


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
	SOCKET		m_MaxFD;

	int			m_iFDSize ;
	//�����������
	//

	BOOL		m_bActive ;


public :
	TID			m_ThreadID ;
	//��������±�ֵΪServerID, ����ֵΪPlayerID
	PlayerID_t	m_aServerHash[OVER_MAX_SERVER] ;	

};

extern ServerManager* g_pServerManager ;

#endif
