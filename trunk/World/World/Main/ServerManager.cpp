
#include "stdafx.h"
#include "ServerManager.h"
#include "Config.h"
#include "Log.h"
#include "PlayerPool.h"
#include "Config.h"
#include "TimeManager.h"
#include "PacketFactoryManager.h"
#include "OnlineUser.h"
#include "Team.h"
#include "ChatCenter.h"
#include "GuildManager.h"
#include "CityManager.h"
#include "MailCenter.h"
#include "SceneInfo.h"
#include "WorldTimeInfo.h"
#include "CountryManager.h"


#define ACCEPT_ONESTEP 50


ServerManager* g_pServerManager = NULL ;

ServerManager::ServerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_iFDSize = 0 ;
	
	SetActive(TRUE) ;


__LEAVE_FUNCTION
}

ServerManager::~ServerManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;

__LEAVE_FUNCTION
}

BOOL ServerManager::Init( )
{
__ENTER_FUNCTION

	//hxj
	m_pServerSocket = new ServerSocket( g_Config.m_ServerInfo.m_World.m_Port ) ;
	Assert( m_pServerSocket ) ;

	m_pServerSocket->setNonBlocking() ;

	m_SocketID = m_pServerSocket->getSOCKET() ;
	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_ThreadID = MyGetCurrentThreadID( ) ;

	for( int i=0; i<OVER_MAX_SERVER; i++ )
	{
		m_aServerHash[i] = INVALID_ID ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Select( )
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
		int iRet = SocketAPI::select_ex(	(int)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
		Log::SaveLog( WORLD_LOGFILE, "ERROR: ServerManager::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::RemovePlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	ID_t	ServerID		=	((ServerPlayer*)pPlayer)->GetServerData()->m_ServerID ;
	BOOL	bUseShareMem	=   FALSE;	
	
	//��һ�������PlayerManager�е���Ϣ
	ret = DelPlayer( pPlayer ) ;
	Assert( ret ) ;

	//�ڶ��������PlayerPool�е���Ϣ��ע��˲������������
	//�����ô˲����󣬵�ǰPlayer���п��ܻᱻ���Ϸ�����½������
	((ServerPlayer*)pPlayer)->FreeOwn( ) ;

	//������������˷���������ҵ�״̬
	if(bUseShareMem)
	{//���˹����ڴ�ķ�����ֻ��������ShutDown
		g_pOnlineUser->ChangeUserStatus( ServerID ,US_WAIT_SHUTDOWN) ;
	}
	else
		g_pOnlineUser->RemoveUser(ServerID);

	//���Ĳ�: �޸�m_aServerHash �����PlayerID ΪINVALID_ID
	Assert( ServerID>=0 && ServerID<OVER_MAX_SERVER ) ;
	m_aServerHash[ServerID] = INVALID_ID ;

	


	Log::SaveLog( WORLD_LOGFILE, "ServerManager::RemovePlayer(PID:%d)...OK", pPlayer->PlayerID() ) ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessInputs( )
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
		for( int i=0; i<ACCEPT_ONESTEP; i++ )
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

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessOutputs( )
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

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
					RemovePlayer( pPlayer ) ;
				}
			}
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessExceptions( )
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

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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

BOOL ServerManager::ProcessCommands( )
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

		ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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
			}
			_MY_CATCH
			{
			}
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::AcceptNewConnection( )
{
__ENTER_FUNCTION

	int iStep = 0 ;
	BOOL ret = FALSE ;

	//����ҳ����ҳ�һ�����е�������ݼ�
	ServerPlayer* client = g_pPlayerPool->NewPlayer() ;
//	Assert( client ) ;
	if( client==NULL )
		return FALSE ;

	iStep = 5 ;
	client->CleanUp( ) ;

	int fd = INVALID_SOCKET ;

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
		iStep += 1000 ;
		goto EXCEPTION ;
	}

	_MY_TRY
	{
		iStep = 30 ;
		fd = (int)client->GetSocket()->getSOCKET();
		if( fd == INVALID_SOCKET )
		{
			goto EXCEPTION ;
		}

		iStep = 40 ;
		ret = client->GetSocket()->setNonBlocking() ;
		if( !ret )
		{
			goto EXCEPTION ;
		}

		iStep = 50 ;
		if( client->GetSocket()->getSockError() )
		{
			goto EXCEPTION ;
		}

		iStep = 60 ;
		ret = client->GetSocket()->setLinger(0) ;
		if( !ret )
		{	
			goto EXCEPTION ;
		}

		iStep = 70 ;
		//��ʼ�����������Ϣ
		client->Init( ) ;
		//���õ�ǰ�ͻ������ӵ�״̬
		client->SetPlayerStatus( PS_WORLD_CONNECT ) ;

		iStep = 80 ;
		_MY_TRY
		{
			ret = AddPlayer( client ) ;
			if( !ret )
			{
				goto EXCEPTION ;
			}
		}
		_MY_CATCH
		{
			iStep += 10000 ;
			goto EXCEPTION ;
		}
	}
	_MY_CATCH
	{
		iStep += 100000 ;
	}

	Log::SaveLog( WORLD_LOGFILE, "ServerManager::AcceptNewConnection(SOCKET:%d)...OK", 
		client->GetSocket()->getSOCKET() ) ;

	return TRUE ;


EXCEPTION:
	Log::SaveLog( WORLD_LOGFILE, "ServerManager::AcceptNewConnection(SOCKET:%d Step:%d)...OK", 
		client->GetSocket()->getSOCKET(), iStep ) ;
	client->CleanUp() ;
	g_pPlayerPool->DelPlayer( client->PlayerID() ) ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_iFDSize>=FD_SETSIZE )
	{//�Ѿ������ܹ������������������
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		return FALSE ;
	}

	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	Assert( fd != INVALID_SOCKET ) ;

	m_MinFD = min(fd , m_MinFD);
	m_MaxFD = max(fd , m_MaxFD);

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_iFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::DelPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	Assert( pServerPlayer ) ;

	SOCKET fd = pServerPlayer->GetSocket()->getSOCKET() ;

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

			ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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

			ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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

	m_iFDSize-- ;
	Assert( m_iFDSize>=0 ) ;

	PlayerManager::RemovePlayer( pPlayer->PlayerID() ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::HeartBeat( )
{
__ENTER_FUNCTION

	BOOL ret ;

	DWORD dwTime = g_pTimeManager->CurrentTime() ;

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

		ret = pPlayer->HeartBeat( dwTime ) ;
		if( !ret )
		{//����߼���������ʧ�ܣ�����Ҫ�Ͽ���ǰ����
			ret = RemovePlayer( pPlayer ) ;
			Assert( ret ) ;
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

void ServerManager::RemoveAllPlayer( )
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

void ServerManager::Loop( )
{
__ENTER_FUNCTION

	while( IsActive() )
	{
		BOOL ret = FALSE ;
		UINT uTime = g_pTimeManager->CurrentTime() ;

		_MY_TRY
		{
			ret = Select( ) ;
			Assert( ret ) ;

			ret = ProcessExceptions( ) ;
			Assert( ret ) ;

			ret = ProcessInputs( ) ;
			Assert( ret ) ;

			ret = ProcessOutputs( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = ProcessCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = HeartBeat( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pOnlineUser->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

        _MY_TRY
        {
            ret = g_pCountryManager->HeartBeat( uTime ) ;
            Assert( ret ) ;
        }
        _MY_CATCH
        {
        }

		_MY_TRY
		{
			ret = g_pTeamList->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pChatCenter->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pGuildManager->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pCityManager->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pMailCenter->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pSceneInfo->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pWorldTimeInfo->HeartBeat( uTime ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}
	};

__LEAVE_FUNCTION
}

ServerPlayer* ServerManager::GetServerPlayer( ID_t ServerID )
{
__ENTER_FUNCTION

	Assert( ServerID>=0 && ServerID<OVER_MAX_SERVER ) ;
	PlayerID_t pid = m_aServerHash[ServerID] ;
	
	if(pid == INVALID_ID)
		return NULL;

	ServerPlayer* pServerPlayer = g_pPlayerPool->GetPlayer( pid ) ;
	Assert( pServerPlayer ) ;
	return pServerPlayer ;

__LEAVE_FUNCTION

	return NULL ;
}

VOID ServerManager::BroadCast( Packet* pPacket )
{
__ENTER_FUNCTION

	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}

		pPlayer->SendPacket( pPacket ) ;
	}

__LEAVE_FUNCTION
}

VOID ServerManager::BroadCastServer(Packet* pPacket)
{
	
	__ENTER_FUNCTION

	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		ServerPlayer* pPlayer = (ServerPlayer*)g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}
		if(pPlayer->GetServerData()->m_Type == SERVER_GAME)
			pPlayer->SendPacket( pPacket ) ;
	}

	return;
	
	__LEAVE_FUNCTION
}


ServerPlayer*	ServerManager::GetLoginPlayer()
{
	__ENTER_FUNCTION

		uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		ServerPlayer* pPlayer = (ServerPlayer*)g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			AssertEx(FALSE, "[ServerManager::GetLoginPlayer]: pPlayer == NULL!!!") ;
			continue ;
		}
		_SERVER_DATA* pServerData = pPlayer->GetServerData();
		if(NULL!=pServerData && SERVER_LOGIN == pServerData->m_Type)
		{
			return pPlayer;
		}
	}

	return NULL;

	__LEAVE_FUNCTION

		return NULL;
}