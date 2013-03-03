
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

	m_pPlayer = new LoginPlayer[MAX_LOGIN_POOL_SIZE] ;
	Assert( m_pPlayer ) ;

	for( INT i=0; i<MAX_LOGIN_POOL_SIZE; i++ )
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i ) ;
		m_pPlayer[i].SetEmpty(TRUE) ;
	}
	m_Position = 0 ;
	m_PlayerCount = MAX_LOGIN_POOL_SIZE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}



LoginPlayer* PlayerPool::NewPlayer( )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	INT iRet = 0 ;
	for( INT i=0; i<MAX_LOGIN_POOL_SIZE; i++ )
	{
		if( m_pPlayer[m_Position].IsEmpty() )
		{
			iRet = m_Position ;
			m_pPlayer[m_Position].SetEmpty( FALSE ) ;

			m_Position ++ ;
			if( m_Position >= MAX_LOGIN_POOL_SIZE ) 
				m_Position = 0 ;

			m_PlayerCount -- ;

			return &(m_pPlayer[iRet]) ;
		}

		m_Position ++ ;
		if( m_Position >= MAX_LOGIN_POOL_SIZE ) 
			m_Position = 0 ;
	}

	return NULL ;

__LEAVE_FUNCTION

	return NULL ;
}

LoginPlayer* PlayerPool::NewPlayer( PlayerID_t PlayerID)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if(PlayerID>=MAX_LOGIN_POOL_SIZE)
	{
		return NULL;
	}


	if(m_pPlayer[PlayerID].IsEmpty())
	{
		LoginPlayer* pPlayer = &(m_pPlayer[PlayerID]);
		pPlayer->SetEmpty(FALSE);
		m_PlayerCount--;
		m_Position = PlayerID;
		m_Position ++ ;
		if( m_Position >= MAX_LOGIN_POOL_SIZE ) 
			m_Position = 0 ;

		return pPlayer;

	}

	return NULL ;
	__LEAVE_FUNCTION
	return NULL ;
}

VOID PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( (uint)PlayerID >= MAX_LOGIN_POOL_SIZE )
	{
		Assert( FALSE ) ;
		return ;
	}

	m_pPlayer[PlayerID].SetEmpty( TRUE ) ;
	

	m_PlayerCount ++ ;

	return ;

__LEAVE_FUNCTION

	return ;
}

