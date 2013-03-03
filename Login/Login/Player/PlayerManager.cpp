
#include "stdafx.h" 
#include "PlayerManager.h"
#include "PlayerPool.h"
#include "Log.h"


PlayerManager::PlayerManager( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	

	for( INT i=0; i<MAX_PLAYER; i++ )
	{
		m_pPlayers[i] = INVALID_ID ;
	}



//	m_SockTable.InitTable( MAX_PLAYER ) ;

__LEAVE_FUNCTION
}

PlayerManager::~PlayerManager( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID PlayerManager::CleanUp( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	

	for( INT i=0; i<MAX_PLAYER; i++ )
	{
		m_pPlayers[i] = INVALID_ID ;
	}
//	m_SockTable.CleanUp() ;

__LEAVE_FUNCTION
}

BOOL PlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( pPlayer ) ;
	if( pPlayer->PlayerManagerID()!=INVALID_ID )	return FALSE;

	if( m_pPlayers[m_nPlayers] == INVALID_ID )
	{
		m_pPlayers[m_nPlayers] = pPlayer->PlayerID() ;
		pPlayer->SetPlayerManagerID( m_nPlayers ) ;

		m_nPlayers ++ ;
		Assert( m_nPlayers<=MAX_PLAYER ) ;
	}
	else
	{
		Assert(FALSE) ;
	}

	return TRUE ;
	
__LEAVE_FUNCTION

	return FALSE ;
}

BOOL PlayerManager::AddPlayer( Player* pPlayer,UINT MaxPlayer )
{
	__ENTER_FUNCTION

	Assert( pPlayer ) ;

	if(m_nPlayers<MaxPlayer)
	{
		if( pPlayer->PlayerManagerID()!=INVALID_ID )	return FALSE;

		if( m_pPlayers[m_nPlayers] == INVALID_ID )
		{
			m_pPlayers[m_nPlayers] = pPlayer->PlayerID() ;
			pPlayer->SetPlayerManagerID( m_nPlayers ) ;

			m_nPlayers ++ ;
			Assert( m_nPlayers<=MAX_PLAYER ) ;
		}
		else
		{
			Assert(FALSE) ;
		}
		return TRUE;
	}
	
	return FALSE;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL PlayerManager::AddPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	//未提供函数
	Assert( FALSE ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

/*
VOID PlayerManager::RemovePlayer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( m_nPlayers > 0 ) ;

	Player* pPlayer = GetPlayer(fd) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	ID_t PlayerManagerID = pPlayer->PlayerManagerID() ;
	if( PlayerManagerID < 0 || PlayerManagerID >= MAX_PLAYER )
	{
		Assert( FALSE ) ;
		return ;
	}

	pPlayer = g_pPlayerPool->GetPlayer( m_pPlayers[m_nPlayers-1] ) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1] ;
	m_pPlayers[m_nPlayers-1] = INVALID_ID ;

	pPlayer->SetPlayerManagerID( PlayerManagerID ) ;


	m_nPlayers -- ;
	Assert( m_nPlayers>=0 ) ;

	m_SockTable.Remove( (uint)fd ) ;


__LEAVE_FUNCTION
}
*/

VOID PlayerManager::RemovePlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	if(m_nPlayers <= 0)			return;

	//////////////////////////////////////////////////////////////////////////
	//pid在池中的Player
	Player* pPlayer = g_pPlayerPool->GetPlayer(pid) ;
	if( pPlayer==NULL )
	{
		//g_pLog->FastSaveLog(LOG_FILE_0,"ERROR: PlayerManager pRemovePlayer==NULL PID=%d",
		//	pid);
		return ;
	}

	ID_t PlayerManagerID = pPlayer->PlayerManagerID() ;
	if( PlayerManagerID < 0 || PlayerManagerID >= MAX_PLAYER )
	{
		//g_pLog->FastSaveLog(LOG_FILE_0,"ERROR: PlayerManager RemovePlayer PMID=%d PID=%d",
		//	PlayerManagerID, pid);
		return ;
	}

	pPlayer->SetPlayerManagerID(INVALID_ID);
	//////////////////////////////////////////////////////////////////////////
	//池尾Player
	pPlayer = g_pPlayerPool->GetPlayer( m_pPlayers[m_nPlayers-1] ) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	if(pid == m_pPlayers[m_nPlayers-1])
	{
		//要删除的正好是池尾单元
		m_pPlayers[m_nPlayers-1] = INVALID_ID;
	}
	else
	{
		//将池尾单元和被删除单元互换
		m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1] ;
		m_pPlayers[m_nPlayers-1] = INVALID_ID ;

		pPlayer->SetPlayerManagerID( PlayerManagerID ) ;
	}

	m_nPlayers -- ;
	Assert( m_nPlayers>=0 ) ;



__LEAVE_FUNCTION
}


BOOL PlayerManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

/*
	BOOL ret ;

	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}
		ret = pPlayer->HeartBeat( ) ;
		Assert( ret ) ;
	}
*/

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
