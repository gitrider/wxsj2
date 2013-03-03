
#include "stdafx.h"
#include "PlayerPool.h"


PlayerPool* g_pPlayerPool=NULL ;

PlayerPool::PlayerPool( )
{
__ENTER_FUNCTION

	m_pPlayer = NULL ;
	m_Position = 0 ;
	m_PlayerCount = 0 ;

__LEAVE_FUNCTION
}

PlayerPool::~PlayerPool( ) 
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pPlayer ) ;


__LEAVE_FUNCTION
}

BOOL PlayerPool::Init( )
{
__ENTER_FUNCTION

	m_pPlayer = new ServerPlayer[MAX_POOL_SIZE] ;
	Assert( m_pPlayer ) ;

	for( int i=0; i<MAX_POOL_SIZE; i++ )
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i ) ;
		m_pPlayer[i].SetEmpty(TRUE) ;
	}
	m_Position = 0 ;
	m_PlayerCount = MAX_POOL_SIZE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

/*
ServerPlayer* PlayerPool::GetPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	if( PlayerID == INVALID_ID )
		return NULL ;

	Lock() ;

	Assert( PlayerID < MAX_POOL_SIZE ) ;
	if( PlayerID >= MAX_POOL_SIZE )
	{
		Unlock() ;
		return NULL ;
	}

	ServerPlayer* pPlayer = &(m_pPlayer[PlayerID]) ;

	Unlock() ;
	return pPlayer ;

__LEAVE_FUNCTION

	Unlock() ;
	return NULL ;
}
*/

ServerPlayer* PlayerPool::NewPlayer( )
{
__ENTER_FUNCTION

	Lock() ;

	int iRet = 0 ;
	for( int i=0; i<MAX_POOL_SIZE; i++ )
	{
		if( m_pPlayer[m_Position].IsEmpty() )
		{
			iRet = m_Position ;
			m_pPlayer[m_Position].SetEmpty( FALSE ) ;

			m_Position ++ ;
			if( m_Position >= MAX_POOL_SIZE ) 
				m_Position = 0 ;

			m_PlayerCount -- ;

			Unlock() ;

			return &(m_pPlayer[iRet]) ;
		}

		m_Position ++ ;
		if( m_Position >= MAX_POOL_SIZE ) 
			m_Position = 0 ;
	}

	Unlock() ;

	return NULL ;

__LEAVE_FUNCTION

	Unlock() ;

	return NULL ;
}

void PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	Lock() ;

	if( (uint)PlayerID >= MAX_POOL_SIZE )
	{
		Assert( FALSE ) ;
		Unlock() ;
		return ;
	}

	m_pPlayer[PlayerID].SetEmpty( TRUE ) ;

	m_PlayerCount ++ ;

	Unlock() ;

	return ;

__LEAVE_FUNCTION

	Unlock() ;

	return ;
}
