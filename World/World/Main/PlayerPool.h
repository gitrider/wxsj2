

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Type.h"
#include "ServerPlayer.h"

#define MAX_SERVER_POOL_SIZE 10

//分配玩家数据的数据池
//
class PlayerPool
{
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//初始化数据池
	BOOL				Init( ) ;

	//根据玩家的PlayerID取得数据指针
	ServerPlayer*		GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=MAX_SERVER_POOL_SIZE || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	//从空闲数据池里分配一个玩家数据
	ServerPlayer*		NewPlayer( ) ;
	//将一个玩家数据清空回收
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
