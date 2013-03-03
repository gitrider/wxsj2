


#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "Config.h"


//���������ָ��������ķ������˳����ڵ�ǰ�����ڵ����ݱ���

class ServerPlayer : public Player
{
public :
	ServerPlayer( BOOL bIsServer=TRUE ) ;
	~ServerPlayer( ) ;

	//���ݽ���
	virtual BOOL	ProcessInput( ) ;
	//���ݷ���
	virtual BOOL	ProcessOutput( ) ;
	//��Ϣ����
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	//���
	virtual VOID	CleanUp( ) ;

	//�߼��ӿ�
	virtual BOOL	HeartBeat( UINT uTime=0 ) ;


public :
	//ͨ�ýӿ�

	virtual BOOL	IsLoginPlayer( ){ return FALSE ; } ;
	virtual BOOL	IsServerPlayer( ){ return TRUE ; } ;
	virtual BOOL	IsBillingPlayer( ){ return FALSE ; } ;

	//���õ�ǰ����������ҵ�״̬
	VOID			SetPlayerStatus( uint status ){ m_Status = status ; } ;

	//�жϵ�ǰ������������Ƿ���Ч
	virtual BOOL	IsValid( ) ;

	//��˷������˳�������Ϣ
	//�˽ӿڲ�֧���̼߳�ͬ�������ֻ���е�ǰ�߳�������ִ��
	virtual BOOL	SendPacket( Packet* pPacket ) ;

private :
	//��������״̬
	uint			m_Status ;


};




#endif
