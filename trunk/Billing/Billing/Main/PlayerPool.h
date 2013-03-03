

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Type.h"
#include "ServerPlayer.h"

#undef MAX_POOL_SIZE
#define MAX_POOL_SIZE 1280

//����������ݵ����ݳ�
//
class PlayerPool
{
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//��ʼ�����ݳ�
	BOOL				Init( ) ;

	//������ҵ�PlayerIDȡ������ָ��
	ServerPlayer*		GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=MAX_POOL_SIZE || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	//�ӿ������ݳ������һ���������
	ServerPlayer*		NewPlayer( ) ;
	//��һ�����������ջ���
	void				DelPlayer( PlayerID_t PlayerID ) ;

	void				Lock( ){ m_Lock.Lock() ; } ;
	void				Unlock( ){ m_Lock.Unlock() ; } ;

private :
	ServerPlayer*		m_pPlayer ;
	uint				m_Position ;

	MyLock				m_Lock ;
	uint				m_PlayerCount ;



};


extern PlayerPool* g_pPlayerPool ;


#endif
