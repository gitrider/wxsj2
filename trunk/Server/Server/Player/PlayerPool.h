

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Type.h"
#include "GamePlayer.h"



//����������ݵ����ݳ�
//



class PlayerPool
{

	
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//��ʼ�����ݳ�
	BOOL				Init( ) ;
	
	//ȡ��ShareMem�ĳ�ʼ��
	//BOOL				PostInit();


	//������ҵ�PlayerIDȡ������ָ��
	GamePlayer*			GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=(PlayerID_t)m_MaxPlayerSize || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	//�ӿ������ݳ������һ���������
	GamePlayer*			NewPlayer( ) ;
	GamePlayer*			NewPlayer(PlayerID_t PlayerID);
	//��һ�����������ջ���
	VOID				DelPlayer( PlayerID_t PlayerID ) ;

	VOID				Lock( ){ m_Lock.Lock() ; } ;
	VOID				Unlock( ){ m_Lock.Unlock() ; } ;

private :
	GamePlayer*			m_pPlayer ;
	uint				m_Position ;

	MyLock				m_Lock ;
	uint				m_PlayerCount ;
	VOID				SetPosition(uint pos);

	uint				m_MaxPlayerSize;


	
};


extern PlayerPool* g_pPlayerPool ;


#endif
