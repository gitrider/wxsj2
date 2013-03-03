#include "stdafx.h" 
#include "ProcessPlayerManager.h"
#include "Log.h"
#include "LoginPlayer.h"
#include "PlayerPool.h"
#include "TurnPlayerQueue.h"
#include "LoginPlayerManager.h"
#if __LINUX__
#include <errno.h>
#endif

#define MAX_READY_HEARTBEAT	100*4


ProcessPlayerManager*	g_pProcessPlayerManager = NULL;

ProcessPlayerManager::ProcessPlayerManager()
{
	__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_USE] ) ;

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_USE].tv_sec = 0;
	m_Timeout[SELECT_USE].tv_usec = 100;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_NormalPlayers = 0;

	m_AccTable.InitTable(MAX_PROCESS_PLAYER+1,MAX_ACCOUNT_LENGTH);
	__LEAVE_FUNCTION
}

ProcessPlayerManager::~ProcessPlayerManager( )
{
	__ENTER_FUNCTION

	__LEAVE_FUNCTION
}


BOOL ProcessPlayerManager::Select( )
{
	__ENTER_FUNCTION

	{
		MySleep( 100 ) ;
	}

	if( m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET )
		return TRUE ;

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	_MY_TRY 
	{
		INT ret = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
			&m_ReadFDs[SELECT_USE] , 
			&m_WriteFDs[SELECT_USE] , 
			&m_ExceptFDs[SELECT_USE] , 
			&m_Timeout[SELECT_USE] ) ;
		if( ret == SOCKET_ERROR )
		{
#if __LINUX__
			switch(errno) 
			{
				case EBADF:
					g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ProcessPlayerManager::Select( )...EBADF" ) ;
					break;
				case EINTR:
					g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ProcessPlayerManager::Select( )...EINTR" ) ;
					break;
				case EINVAL:
					g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ProcessPlayerManager::Select( )...EINVAL nfds=%d", (INT)m_MaxFD+1) ;
					break;
				default:
					break;
			};
#endif
			Assert(FALSE) ;
		}
	} 
	_MY_CATCH
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ProcessPlayerManager::Select( )..." ) ;
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessPlayerManager::ProcessInputs( )
{
	__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//���ݶ�ȡ
	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		//�����ѶϿ�
		if( pPlayer->IsDisconnect() )
			continue;

		if(pPlayer->GetPlayerStatus()==PS_LOGIN_SERVER_READY)
		{
			pPlayer->SetReadyKickCount(pPlayer->GetReadyKickCount()+1);
			if(pPlayer->GetReadyKickCount()>= MAX_READY_HEARTBEAT)
				RemovePlayer( pPlayer);
		}

		//��Normal ״̬������ε�Input
		if( pPlayer->GetPlayerStatus() != PS_LOGIN_NORMAL
			&& pPlayer->GetPlayerStatus()!=PS_LOGIN_SERVER_READY
			&& pPlayer->GetPlayerStatus()!=PS_LOGIN_PROCESS_TURN
			&& pPlayer->GetPlayerStatus()!=PS_LOGIN_WORLD_PROCESS_TURN)
			continue;
	
		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			RemovePlayer( pPlayer ) ;
		}
		else if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{//��������
			_MY_TRY
			{
				ret = pPlayer->ProcessInput( ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( pPlayer ) ;
			}
		}
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessPlayerManager::ProcessOutputs( )
{
	__ENTER_FUNCTION

		BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//���ݷ���
	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect( ) )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			RemovePlayer( pPlayer ) ;
		}
		else if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{//��������
			_MY_TRY
			{
				ret = pPlayer->ProcessOutput( ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer ) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( pPlayer ) ;
			}
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL ProcessPlayerManager::ProcessExceptions( )
{
	__ENTER_FUNCTION

		if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
		{ 
			return TRUE ;
		}

		uint nPlayerCount = GetPlayerNumber() ;
		for( uint i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			//ĳ����ҶϿ���������

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;

			if( pPlayer->IsDisconnect() )
				continue ;

			SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
			Assert( s!=INVALID_SOCKET ) ;

			if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
			{
				RemovePlayer( pPlayer ) ;
			}
		}




		return TRUE ;

		__LEAVE_FUNCTION

			return FALSE ;
}

BOOL ProcessPlayerManager::ProcessCommands( )
{
	__ENTER_FUNCTION

		BOOL ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect() )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( pPlayer->GetSocket()->isSockError() )
		{//���ӳ��ִ���
			RemovePlayer( pPlayer ) ;
		}
		else
		{//��������
			_MY_TRY
			{
				ret = pPlayer->ProcessCommand( FALSE ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer) ;
				}
			}
			_MY_CATCH
			{
				RemovePlayer( pPlayer) ;
			}
		}
	}




	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}


BOOL ProcessPlayerManager::RemovePlayer( Player* pPlayer)
{
	__ENTER_FUNCTION

	Assert( pPlayer ) ;

	BOOL ret = FALSE ;

	const CHAR* pAccountName = ((LoginPlayer*)pPlayer)->GetAccount();

	//��һ�������Socket��Ϣ
	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	if( fd != INVALID_SOCKET )
	{
		DelPlayerSocket( fd ) ;
		pPlayer->Disconnect( ) ;

		g_pLog->FastSaveLog(LOG_FILE_0, "ProcessPlayerManager::RemovePlayer Disconnect(SOCKET=%d)...OK", 
			fd ) ;
	}

	//�ڶ��������ProcessManager�е��������
	_MY_TRY
	{
		if(((LoginPlayer*)pPlayer)->GetPlayerStatus() == PS_LOGIN_NORMAL
			||((LoginPlayer*)pPlayer)->GetPlayerStatus() == PS_LOGIN_SERVER_READY
			||((LoginPlayer*)pPlayer)->GetPlayerStatus() == PS_LOGIN_WAIT_WORLD_ASK
			||((LoginPlayer*)pPlayer)->GetPlayerStatus() == PS_LOGIN_WORLD_PROCESS_TURN
			||((LoginPlayer*)pPlayer)->GetPlayerStatus() == PS_LOGIN_WAIT_WORLD_KICK)
		{
			DecNormalPlayerCount();
		}
		
	}
	_MY_CATCH
	{
	}

	//�����������ProcessPlayerManager�е���Ϣ
	_MY_TRY
	{
		ret = DelPlayer( pPlayer->PlayerID() ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
	}

	//���Ĳ����������Table�е��������
	if(pPlayer == m_AccTable.Get(((LoginPlayer*)pPlayer)->GetAccount()))
	{
		m_AccTable.Remove(pAccountName);
	}

	//���Ĳ������PlayerPool�е���Ϣ��ע��˲������������
	//�����ô˲����󣬵�ǰPlayer���п��ܻᱻ���Ϸ�����½���ͻ���
	((LoginPlayer*)pPlayer)->FreeOwn( ) ;
	
	


	g_pLog->FastSaveLog(LOG_FILE_0, "ProcessPlayerManager::Real RemovePlayer(AccountName =%s)...OK",
		pAccountName ) ;

	return ret ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessPlayerManager::AddPlayer(Player* pPlayer )
{
	__ENTER_FUNCTION

	if( m_nFDSize>=MAX_PLAYER )
	{//�Ѿ������ܹ������������������
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:ProcessPlayerManager::AddPlayer...m_nFDSize>=MAX_PLAYER");
		return FALSE ;
	}
	BOOL ret = PlayerManager::AddPlayer( pPlayer,MAX_PROCESS_PLAYER) ;
	if( !ret )
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:ProcessPlayerManager::AddPlayer...FALSE==PlayerManager::AddPlayer");
		return FALSE ;
	}

	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	AddPlayerSocket( fd ) ;

	LoginPlayer* pLoginPlayer = (LoginPlayer*)pPlayer;
	if(FindAccName(pLoginPlayer->GetAccount()) == NULL)
	{
		m_AccTable.Add(pLoginPlayer->GetAccount(),pLoginPlayer);
	}
	else
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:ProcessPlayerManager::AddPlayer...acc=%s already in m_AccTable",
			pLoginPlayer->GetAccount());
		return FALSE;
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessPlayerManager::AddPlayerSocket( SOCKET fd )
{
	__ENTER_FUNCTION

	Assert(fd!=INVALID_SOCKET) ;
	// �Ѿ����ڵ�Socket��������� [6/30/2007]
	if(FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]))	return FALSE;

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd , m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd,m_MaxFD));
	Assert( m_MaxFD<FD_SETSIZE && m_MaxFD>=0 ) ;


	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}


BOOL ProcessPlayerManager::DelPlayer( PlayerID_t pid )
{
	__ENTER_FUNCTION

	Assert( pid!=INVALID_ID) ;

	//���ProcessPlayerManager�е���Ϣ
	PlayerManager::RemovePlayer( pid ) ;
	
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessPlayerManager::DelPlayerSocket( SOCKET fd )
{
__ENTER_FUNCTION
	// ���Socket������Assert�����Ƿ���FALSE [6/29/2007]
	if(m_MinFD==INVALID_SOCKET)					return FALSE;
	if(m_MaxFD==INVALID_SOCKET)					return FALSE;
	if(fd == INVALID_SOCKET)					return FALSE;
	if(!FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]))	return FALSE;

	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		uint nPlayerCount = GetPlayerNumber() ;
		for( uint i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp < s )
			{
				s = temp ;
			}
		}

		if( m_MinFD == m_MaxFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			m_MinFD = s ;
		}
	} 
	else if( fd==m_MaxFD ) 
	{
		SOCKET s = m_MinFD ;
		uint nPlayerCount = GetPlayerNumber() ;
		for( uint i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			LoginPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
			Assert( pPlayer ) ;
			if( pPlayer==NULL )
				continue ;


			SOCKET temp = pPlayer->GetSocket()->getSOCKET() ;
			if( temp == fd )
				continue ;
			if( temp==INVALID_SOCKET )
				continue ;

			if( temp > s )
			{
				s = temp ;
			}
		}

		if( m_MaxFD == m_MinFD )
		{
			m_MinFD = m_MaxFD = INVALID_SOCKET ;
		}
		else
		{
			m_MaxFD = s ;
		}
	}

	if(FD_ISSET(fd, &m_ReadFDs[SELECT_BAK]))	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	if(FD_ISSET(fd, &m_ReadFDs[SELECT_USE]))	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	if(FD_ISSET(fd, &m_WriteFDs[SELECT_BAK]))	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	if(FD_ISSET(fd, &m_WriteFDs[SELECT_USE]))	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	if(FD_ISSET(fd, &m_ExceptFDs[SELECT_BAK]))	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	if(FD_ISSET(fd, &m_ExceptFDs[SELECT_USE]))	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize-- ;
	Assert( m_nFDSize>=0 ) ;

	return TRUE ;
__LEAVE_FUNCTION
	return FALSE ;
}


BOOL ProcessPlayerManager::HeartBeat( UINT uTime )
{
	__ENTER_FUNCTION

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
		ret = pPlayer->HeartBeat(uTime ) ;
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			ret = RemovePlayer( pPlayer) ;
			Assert( ret ) ;
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}


VOID ProcessPlayerManager::RemoveAllPlayer( )
{
	__ENTER_FUNCTION

		uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[0] == INVALID_ID )
			break ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[0]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			break ;
		}

		RemovePlayer( pPlayer) ;
	}

	__LEAVE_FUNCTION
}

Player* ProcessPlayerManager::FindAccName(const CHAR* szAccount)
{
__ENTER_FUNCTION
	return (Player*)(m_AccTable.Get((CHAR*)szAccount));
__LEAVE_FUNCTION
	return NULL;
}