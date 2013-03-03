
#include "stdafx.h"
#include "IncomingPlayerManager.h"
#include "Config.h"
#include "Log.h"
#include "PlayerPool.h"
#include "ServerManager.h"
#include "TimeManager.h"
#include "PacketFactoryManager.h"
//#include "RecyclePlayerManager.h"


#define ACCEPT_ONESTEP 50


IncomingPlayerManager* g_pIncomingPlayerManager = NULL ;

IncomingPlayerManager::IncomingPlayerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_USE] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;
	
	//mRecyclePlayerManager = new RecyclePlayerManager();
	//Assert(mRecyclePlayerManager);


__LEAVE_FUNCTION
}

IncomingPlayerManager::~IncomingPlayerManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	//SAFE_DELETE( mRecyclePlayerManager);

__LEAVE_FUNCTION
}

BOOL IncomingPlayerManager::Init( )
{
__ENTER_FUNCTION

	//hxj
	m_pServerSocket = new ServerSocket( (g_pServerManager->GetCurrentServerInfo())->m_Port0 ) ;
	Assert( m_pServerSocket ) ;

	m_pServerSocket->setNonBlocking() ;

	m_SocketID = m_pServerSocket->getSOCKET() ;
	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

//	m_ThreadID = MyGetCurrentThreadID( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::Select( )
{
__ENTER_FUNCTION

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	MySleep(100) ;

	_MY_TRY 
	{
		INT iRet = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::RemovePlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	
	//��һ�������PlayerManager�е���Ϣ
	ret = DelPlayer( pPlayer->PlayerID() ) ;
	Assert( ret ) ;

	//�ڶ��������PlayerPool�е���Ϣ��ע��˲������������
	//�����ô˲����󣬵�ǰPlayer���п��ܻᱻ���Ϸ�����½������
//	((GamePlayer*)pPlayer)->ChooseFreeOwn(mRecyclePlayerManager) ;
	((GamePlayer*)pPlayer)->FreeOwn() ;


	Log::SaveLog( SERVER_LOGFILE, "IncomingPlayerManager::RemovePlayer(PID:%d)...OK", pPlayer->PlayerID() ) ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//�����ӽ��룺
	if( FD_ISSET(m_SocketID,&m_ReadFDs[SELECT_USE]) )
	{
		for( INT i=0; i<ACCEPT_ONESTEP; i++ )
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	//���ݶ�ȡ
	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
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
					SaveCodeLog( ) ;
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::ProcessOutputs( )
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

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//���ӳ��ִ���
				RemovePlayer( pPlayer ) ;
			}
			else
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
					SaveCodeLog( ) ;
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::ProcessExceptions( )
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

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
		{//��������������⣬�ѡ�����
			Assert( FALSE ) ;
			continue ;
		}

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			RemovePlayer( pPlayer ) ;
		}
	}





	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::ProcessCommands( )
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

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		if( s == m_SocketID )
			continue ;

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
					RemovePlayer( pPlayer ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
				RemovePlayer( pPlayer ) ;
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::AcceptNewConnection( )
{
__ENTER_FUNCTION

	INT iStep = 0 ;
	BOOL ret = FALSE ;

	//����ҳ����ҳ�һ�����е�������ݼ�
	GamePlayer* client = g_pPlayerPool->NewPlayer() ;
//	Assert( client ) ;
	if( client==NULL )
		return FALSE ;

	iStep = 5 ;
	client->CleanUp( ) ;

	INT fd = INVALID_SOCKET ;

	iStep = 10 ;
	_MY_TRY
	{
		//���ܿͻ��˽���Socket���
		ret = m_pServerSocket->accept( client->GetSocket() ) ;
		if( !ret )
		{
			iStep = 15 ;
			goto EXCEPTION ;
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
		iStep += 1000 ;
		goto EXCEPTION ;
	}

	_MY_TRY
	{
		iStep = 30 ;
		fd = (INT)client->GetSocket()->getSOCKET();
		if( fd == INVALID_SOCKET )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 40 ;
		ret = client->GetSocket()->setNonBlocking() ;
		if( !ret )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 50 ;
		if( client->GetSocket()->getSockError() )
		{
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 60 ;
		ret = client->GetSocket()->setLinger(0) ;
		if( !ret )
		{	
			Assert(FALSE) ;
			goto EXCEPTION ;
		}

		iStep = 70 ;
		//��ʼ�����������Ϣ
		client->Init( ) ;
		//���õ�ǰ�ͻ������ӵ�״̬
		client->SetPlayerStatus( PS_SERVER_CONNECT ) ;

		iStep = 80 ;
		_MY_TRY
		{
			ret = AddPlayer( client ) ;
			if( !ret )
			{
				Assert(FALSE) ;
				goto EXCEPTION ;
			}
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
			iStep += 10000 ;
			goto EXCEPTION ;
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
		iStep += 100000 ;
	}

	Log::SaveLog( SERVER_LOGFILE, "AcceptNewConnection(SOCKET=%d, IP=%s)...OK", 
		client->GetSocket()->getSOCKET(), client->GetSocket()->m_Host ) ;

	return TRUE ;


EXCEPTION:
	client->CleanUp() ;
	g_pPlayerPool->DelPlayer( client->PlayerID() ) ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_nFDSize>=FD_SETSIZE )
	{//�Ѿ������ܹ������������������
		return FALSE ;
	}
	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	Assert( fd != INVALID_SOCKET ) ;

	if( FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]) )
	{
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		return FALSE ;
	}

	m_MinFD = min(fd , m_MinFD);
	m_MaxFD = max(fd , m_MaxFD);

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::DelPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( pid!=INVALID_ID ) ;
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(pid) ;
	Assert( pGamePlayer ) ;

	SOCKET fd = pGamePlayer->GetSocket()->getSOCKET() ;

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	if( fd==INVALID_SOCKET ) 
	{
		Assert(FALSE) ;
	}

	if( fd==m_MinFD ) 
	{
		SOCKET s = m_MaxFD ;
		uint nPlayerCount = GetPlayerNumber() ;
		for( uint i=0; i<nPlayerCount; i++ )
		{
			if( m_pPlayers[i]==INVALID_ID )
				continue ;

			GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
			if( s > m_SocketID )
			{
				m_MinFD = m_SocketID ;
			}
			else
			{
				m_MinFD = s ;
			}
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

			GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
			if( s < m_SocketID )
			{
				m_MaxFD = m_SocketID ;
			}
			else
			{
				m_MaxFD = s ;
			}
		}
	}

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize-- ;
	Assert( m_nFDSize>=0 ) ;

	PlayerManager::RemovePlayer( pid ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::HeartBeat( )
{
__ENTER_FUNCTION

	BOOL ret ;

	UINT uTime = g_pTimeManager->CurrentTime() ;

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

		_MY_TRY
		{
			ret = pPlayer->HeartBeat(uTime,1 ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
			ret = FALSE ;
		}
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			ret = RemovePlayer( pPlayer ) ;
			Assert( ret ) ;
		}
	}

	//ret = mRecyclePlayerManager->HeartBeat(uTime);
	//Assert(ret);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID IncomingPlayerManager::RemoveAllPlayer( )
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
	
		RemovePlayer( pPlayer ) ;
	}

__LEAVE_FUNCTION
}


BOOL IncomingPlayerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			continue ;
		}

		BOOL bNeedRemove = TRUE ;

		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;

				uint uret ;
				_MY_TRY
				{
					uret = pPacket->Execute(pPlayer) ;
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
					uret = PACKET_EXE_ERROR ;
				}

				if( uret == PACKET_EXE_ERROR )
				{
					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;

					RemovePlayer( pPlayer ) ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}

		//������Ϣ
		if( bNeedRemove )
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}


	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::MovePacket( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	Lock( ) ;

	uint Cur = m_Head ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	Unlock( ) ;
	return FALSE ;
}

BOOL IncomingPlayerManager::ResizeCache( )
{
__ENTER_FUNCTION

	ASYNC_PACKET* pNew = new ASYNC_PACKET[m_QueSize+MAX_CACHE_SIZE] ;
	if( pNew==NULL )
		return FALSE ;

	memcpy( pNew, &(m_PacketQue[m_Head]), sizeof(ASYNC_PACKET)*(m_QueSize-m_Head) ) ;
	if( m_Head!=0 )
	{
		memcpy( &(pNew[m_QueSize-m_Head]), &(m_PacketQue[0]), sizeof(ASYNC_PACKET)*(m_Head) ) ;
	}

	memset( m_PacketQue, 0, sizeof(ASYNC_PACKET)*m_QueSize ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	m_PacketQue = pNew ;

	m_Head = 0 ;
	m_Tail = m_QueSize ;
	m_QueSize = m_QueSize+MAX_CACHE_SIZE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL IncomingPlayerManager::SendPacket( Packet* pPacket, PlayerID_t PlayerID, uint Flag )
{
__ENTER_FUNCTION

	Lock( ) ;

	if( m_PacketQue[m_Tail].m_pPacket )
	{//��������
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	Unlock( ) ;
	return FALSE ;
}

BOOL IncomingPlayerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
__ENTER_FUNCTION

	Lock( ) ;

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//��������û����Ϣ
		Unlock( ) ;
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;


	Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	Unlock( ) ;
	return FALSE ;
}


