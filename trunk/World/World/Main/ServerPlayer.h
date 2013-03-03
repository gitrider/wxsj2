


#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "Config.h"


//���������ָ��������ķ������˳����ڵ�ǰ�����ڵ����ݱ���

class ServerPlayer : public Player
{
public :
	ServerPlayer( ) ;
	~ServerPlayer( ) ;

	BOOL			Init( ) ;

	//���ݽ���
	virtual BOOL	ProcessInput( ) ;
	//���ݷ���
	virtual BOOL	ProcessOutput( ) ;
	//��Ϣ����
	virtual BOOL	ProcessCommand( bool Option = true ) ;

	//���
	virtual void	CleanUp( ) ;

	//�߼��ӿ�
	virtual BOOL	HeartBeat( DWORD dwTime=0 ) ;


public :
	//ͨ�ýӿ�

	virtual BOOL	IsGamePlayer( ){ return FALSE ; } ;
	virtual BOOL	IsServerPlayer( ){ return TRUE ; } ;

	//���õ�ǰ����������ҵ�״̬
	void			SetPlayerStatus( uint status ){ m_Status = status ; } ;

	//�жϵ�ǰ������������Ƿ���Ч
	virtual BOOL	IsValid( ) ;

	//��˷������˳�������Ϣ
	//�˽ӿڲ�֧���̼߳�ͬ�������ֻ���е�ǰ�߳�������ִ��
	virtual BOOL	SendPacket( Packet* pPacket ) ;

	_SERVER_DATA*	GetServerData(){ return m_pServerData ; } ;
	void			SetServerData( _SERVER_DATA* pData ){ m_pServerData = pData ; } ;

	void			FreeOwn( ) ;

private :
	//��������״̬
	uint			m_Status ;
	//ָ�����������g_Config�е�����
	_SERVER_DATA*	m_pServerData ;


};




#endif
