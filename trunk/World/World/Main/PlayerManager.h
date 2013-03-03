

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "Type.h"
#include "Player.h"
#include "Socket.h"
#include "IDTable.h"


//����һȺ��ҹ��ܵĻ���ģ��,һ��Ϊÿ����������һ��PlayerManager
//������һ���������߳���ִ��
class PlayerManager
{
public :
	PlayerManager( ) ;
	~PlayerManager( ) ;

	//�����ǰģ���ڵ�������Ϣ
	void				CleanUp( ) ;

	//�߼��ӿ�
	virtual BOOL		HeartBeat( DWORD dwTime=0 ) ;

	//����һ�����
	BOOL				AddPlayer( Player* pPlayer ) ;
	//ɾ��һ�����
//	void				RemovePlayer( SOCKET fd ) ;
	void				RemovePlayer( PlayerID_t pid ) ;

	//������������ȡ�����ָ��
//	Player*				GetPlayer( SOCKET fd ) ;

	PlayerID_t*			GetPlayers( ){ return m_pPlayers ; } ;

	uint				GetPlayerNumber( ){ return m_nPlayers ; } ;
	BOOL				HasPlayer( ){ return (BOOL)m_nPlayers ; } ;



	BOOL				AddPlayer( PlayerID_t pid ) ;		//δ�ṩ���ܽӿ�

protected :
	//
	PlayerID_t	m_pPlayers[MAX_PLAYER] ;
	uint		m_nPlayers ;

	//���ڱ����������ź����Player����ָ��Ĺ�ϣ��
//	IDTable		m_SockTable ;


};




#endif
