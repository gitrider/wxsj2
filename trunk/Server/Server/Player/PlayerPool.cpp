

#include "stdafx.h"
#include "PlayerPool.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "Config.h"

PlayerPool* g_pPlayerPool=NULL ;

PlayerPool::PlayerPool( )
{
__ENTER_FUNCTION

	m_pPlayer = NULL ;
	m_Position = 0 ;
	m_PlayerCount = 0 ;
	m_MaxPlayerSize = MAX_POOL_SIZE;

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
	
	if(g_Config.m_ConfigInfo.m_SystemMode == 0)
		m_MaxPlayerSize = 100;
		

	m_pPlayer = new GamePlayer[m_MaxPlayerSize] ;
	Assert( m_pPlayer ) ;

	for( UINT i=0; i<m_MaxPlayerSize; i++ )
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i ) ;
		m_pPlayer[i].SetEmpty(TRUE) ;
	}
	m_Position = 0 ;
	m_PlayerCount = m_MaxPlayerSize ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

/*
BOOL	PlayerPool::PostInit()	
{
	for( INT i=0; i<MAX_POOL_SIZE; i++ )
	{
		if(m_pPlayer[i].GetShareMemStatus())
		{
			m_Position=i;
			m_PlayerCount--;
			
			m_pPlayer[i].SetEmpty(FALSE);
			m_pPlayer[i].SetDisconnect( TRUE ) ;
			m_pPlayer[i].ResetKick() ;

			Obj_Human* pHuman =m_pPlayer[i].GetHuman();
			Assert(pHuman);

			m_pPlayer[i].m_HumanGUID	 = pHuman->GetDB()->GetHumanDB()->m_GUID;
			g_pGUIDManager->Add(m_pPlayer[i].m_HumanGUID,pHuman);

		}

	}
	return TRUE;
}*/


/*
GamePlayer* PlayerPool::GetPlayer( PlayerID_t PlayerID )
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

	GamePlayer* pPlayer = &(m_pPlayer[PlayerID]) ;

	Unlock() ;
	return pPlayer ;

__LEAVE_FUNCTION

	Unlock() ;
	return NULL ;
}
*/

GamePlayer* PlayerPool::NewPlayer( )
{
__ENTER_FUNCTION

	Lock() ;

	INT iRet = 0 ;
	for( UINT i=0; i<m_MaxPlayerSize; i++ )
	{
		if( m_pPlayer[m_Position].IsEmpty() )
		{
			iRet = m_Position ;
			m_pPlayer[m_Position].SetEmpty( FALSE ) ;

			m_Position ++ ;
			if( m_Position >= m_MaxPlayerSize ) 
				m_Position = 0 ;

			m_PlayerCount -- ;

			Unlock() ;

			return &(m_pPlayer[iRet]) ;
		}

		m_Position ++ ;
		if( m_Position >= m_MaxPlayerSize ) 
			m_Position = 0 ;
	}

	Unlock() ;

	return NULL ;

__LEAVE_FUNCTION

	Unlock() ;

	return NULL ;
}

GamePlayer* PlayerPool::NewPlayer( PlayerID_t PlayerID)
{
	__ENTER_FUNCTION

	Lock() ;

	if(PlayerID>=m_MaxPlayerSize)
	{
		Unlock();
		return NULL;
	}


	if(m_pPlayer[PlayerID].IsEmpty())
	{
		GamePlayer* pPlayer = &(m_pPlayer[PlayerID]);
		pPlayer->SetEmpty(FALSE);
		m_PlayerCount--;
		m_Position = PlayerID;
		m_Position ++ ;
		if( m_Position >= m_MaxPlayerSize ) 
			m_Position = 0 ;

		Unlock();
		return pPlayer;

	}

	Unlock() ;
	return NULL ;
	__LEAVE_FUNCTION
	Unlock() ;
	return NULL ;
}

VOID PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	Lock() ;

	if( (uint)PlayerID >= m_MaxPlayerSize )
	{
		Assert( FALSE ) ;
		Unlock() ;
		return ;
	}

	if( m_pPlayer[PlayerID].IsEmpty() )
	{
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

