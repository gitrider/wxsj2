#include "stdafx.h" 
#include "ServerManager.h"
#include "Log.h"
#include "LBConnect.h"
#include "PacketFactoryManager.h"
#include "TurnPlayerQueue.h"
#include "LWAskCharLogin.h"
#include "PlayerPool.h"
#include "SSConnect.h"
#include "TimeManager.h"
#include "LBKickAll.h"


ServerManager* g_pServerManager = NULL ;

ServerManager::ServerManager( )
{
__ENTER_FUNCTION

	//m_pServerSocket = NULL ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;
	m_KickALL	= FALSE;

	CleanUp( ) ;

__LEAVE_FUNCTION
}

ServerManager::~ServerManager( )
{
__ENTER_FUNCTION

	//SAFE_DELETE( m_pServerSocket ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;

__LEAVE_FUNCTION
}

VOID ServerManager::CleanUp( )
{
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

	m_Head = 0 ;
	m_Tail = 0 ;

	m_WorldPlayer.SetPlayerID(WORLD_PLAYER_ID) ;
	m_BillingPlayer.SetPlayerID(BILLING_PLAYER_ID);

	m_pCurServerInfo = NULL ;

	memset( m_aScenePlayerCount, 0, MAX_SCENE*sizeof(INT) ) ;
	memset( m_aServerPlayerPoolSize, 0, MAX_SERVER*sizeof(INT) ) ;

	m_CurrentTime = 0;
	m_LogTurnTime = 0;
}

ID_t ServerManager::GetLoginID( )
{
__ENTER_FUNCTION

	return g_Config.m_LoginInfo.m_LoginID ;
	
__LEAVE_FUNCTION

	return INVALID_ID ;
}

_SERVER_DATA* ServerManager::FindServerInfo( ID_t ServerID )
{
__ENTER_FUNCTION

	Assert( ServerID<OVER_MAX_SERVER && ServerID>=0 ) ;

	INT iIndex = g_Config.m_ServerInfo.m_HashServer[ServerID] ;
	Assert( iIndex>=0 && iIndex<MAX_SERVER ) ;

	return &(g_Config.m_ServerInfo.m_pServer[iIndex]) ;

__LEAVE_FUNCTION

	return NULL ;
}

_SERVER_DATA* ServerManager::GetCurrentServerInfo( )
{
__ENTER_FUNCTION

	if( m_pCurServerInfo==NULL )
	{
		INT iIndex = g_Config.m_ServerInfo.m_HashServer[g_Config.m_LoginInfo.m_LoginID] ;

		Assert( iIndex>=0 && iIndex<MAX_SERVER ) ;
		m_pCurServerInfo = &(g_Config.m_ServerInfo.m_pServer[iIndex]) ;

	}
	Assert( m_pCurServerInfo ) ;

	return m_pCurServerInfo ;

__LEAVE_FUNCTION

	return NULL ;
}

BOOL ServerManager::Init( )
{
__ENTER_FUNCTION

	//m_Timeout[SELECT_BAK].tv_sec = 0;
	//m_Timeout[SELECT_BAK].tv_usec = 100;
	m_LogTurnTime = g_Config.m_ConfigInfo.m_DisconnectTime;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Tick( )
{
__ENTER_FUNCTION

	BOOL ret ;

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

	ret = ProcessCommands( ) ;
	Assert( ret ) ;

	ret = ProcessCacheCommands( ) ;
	Assert( ret ) ;


	ret = HeartBeat( ) ;
	Assert( ret ) ;

	ret = ResetQueuePos();
	Assert(ret);
		
	ret = SendQueuePlayerToWorld();
	Assert(ret);

	//排队玩家数量定时输出到日志
	_MY_TRY
	{
		m_LogTurnTime -= (uTime-m_CurrentTime);
		if(m_LogTurnTime < 0)
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ServerManager::Tick WorldTurnPlayer Number:%u ValidNum:%d InvalidNum:%d",
				g_pWorldPlayerQueue->GetCount(),g_pWorldPlayerQueue->GetValidCount(),g_pWorldPlayerQueue->GetInvalidCount());
			m_LogTurnTime = g_Config.m_ConfigInfo.m_DisconnectTime;
		}
		m_CurrentTime = uTime;
	}
	_MY_CATCH
	{

	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Select( )
{
__ENTER_FUNCTION

	MySleep(100) ;
	if( m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET )
	{
		return TRUE ;
	}

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	_MY_TRY 
	{
		INT iRet = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		if( iRet==SOCKET_ERROR )
		{
			AssertEx( FALSE,MySocketError()) ;
		}
	} 
	_MY_CATCH
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ServerManager::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;

}



BOOL ServerManager::RemoveServer(PlayerID_t id )
{
__ENTER_FUNCTION
	
	BOOL bRet = FALSE;

	switch(id) 
	{
	case WORLD_PLAYER_ID:
		{
			SOCKET s = m_WorldPlayer.GetSocket()->getSOCKET() ;
			Assert( s!=INVALID_SOCKET ) ;

			bRet = DelServer( s ) ;
			Assert( bRet ) ;

			m_WorldPlayer.CleanUp( ) ;

			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: RemoveServer( )...World" ) ;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			SOCKET s = m_BillingPlayer.GetSocket()->getSOCKET() ;
			Assert( s!=INVALID_SOCKET ) ;
			bRet = DelServer( s ) ;
			Assert( bRet ) ;
			m_BillingPlayer.CleanUp( ) ;
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: RemoveServer( )...Billing" ) ;
		}
		break;
	default:
		{
			return FALSE;
		}
	}
	

	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

Player*	ServerManager::GetServerPlayer(PlayerID_t id)
{
	__ENTER_FUNCTION



	switch(id) 
	{
	case WORLD_PLAYER_ID:
		{
			return &m_WorldPlayer;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			return &m_BillingPlayer;
		}
		break;
	default:
		{
			return NULL;
		}
	}
	__LEAVE_FUNCTION

	return NULL;
}

BOOL ServerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据读取
	do
	{
		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			Player*	pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if( pPlayer->IsValid())
			{
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
				
				if( pPlayer->GetSocket()->isSockError() )
				{
					RemoveServer(i) ;
				}
				else if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
				{
					_MY_TRY
					{
						//处理服务器发送过来的消息
						ret = pPlayer->ProcessInput( ) ;
						if( !ret )
						{
							RemoveServer(i) ;
						}
					}
					_MY_CATCH
					{
						RemoveServer(i) ;
					}
				}
			}
		}

		

	}while(FALSE) ;


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

	//数据发送
	do
	{

		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			Player* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if(pPlayer->IsValid())
			{
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
				
				if( pPlayer->GetSocket()->isSockError() )
				{
					RemoveServer( i ) ;
				}
				else if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
				{
					_MY_TRY
					{
						//发送数据
						ret = pPlayer->ProcessOutput( ) ;
						if( !ret )
						{
							RemoveServer( i ) ;
						}
					}
					_MY_CATCH
					{
						RemoveServer( i ) ;
					}
				}	
			}
		}
		

	}while(FALSE) ;


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

	do
	{
		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			Player* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if(pPlayer->IsValid())
			{	
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;

				if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
				{
					g_pLog->FastSaveLog(LOG_FILE_0, "ProcessExceptions( ) ..." ) ;
					RemoveServer( i ) ;
				}
			}
		}
		
	}while(FALSE) ;

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

	do
	{

		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			Player* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if( pPlayer->IsValid() )
			{
				if( pPlayer->GetSocket()->isSockError() )
				{//连接出现错误
					RemoveServer( i ) ;
				}
				else
				{//连接正常
					_MY_TRY
					{
						ret = pPlayer->ProcessCommand( FALSE ) ;
						if( !ret )
						{
							RemoveServer( i ) ;
						}
					}
					_MY_CATCH
					{
						RemoveServer( i ) ;
					}
				}
			}

		}

	

	}while(FALSE) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::AddServer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( fd!=INVALID_SOCKET ) ;

	if( m_nFDSize>=MAX_PLAYER )
	{//已经超出能够检测的网络句柄最大数；
		Assert(FALSE) ;
		return FALSE ;
	}

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd , m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd,  m_MaxFD));

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::DelServer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	Assert( fd!=INVALID_SOCKET ) ;
	
	if( fd == m_WorldPlayer.GetSocket()->getSOCKET() ) 
	{
		m_MinFD = m_MaxFD = m_BillingPlayer.GetSocket()->getSOCKET();
	} 
	else if( fd == m_BillingPlayer.GetSocket()->getSOCKET() ) 
	{
		m_MinFD = m_MaxFD = m_WorldPlayer.GetSocket()->getSOCKET();
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

BOOL ServerManager::HeartBeat( )
{
__ENTER_FUNCTION

	BOOL ret ;
	
	UINT uTime = g_pTimeManager->CurrentTime();
	
	if(!IsWorldServerActive())
	{
		//连接TestServerID的内网端口
		ret = ConnectWorldServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
		}
	}
	if(!IsBillingServerActive())
	{
		//连接TestServerID的内网端口
		ret = ConnectBillingServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
		}

		// 测试连多个BillingServer不同的代码[3/12/2007]
		//else
		//{
		//	RemoveServer(BILLING_PLAYER_ID);
		//}
	}
	else
	{
		ret = m_BillingPlayer.HeartBeat(uTime);
		if(FALSE == ret)
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: HeartBeat( ) m_BillingPlayer ...HeartBeat Faile!!");
			RemoveServer(BILLING_PLAYER_ID);
		}
	}

	

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::IsWorldServerActive( )
{
__ENTER_FUNCTION

	if( m_WorldPlayer.IsValid() )
		return TRUE ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	ServerManager::IsBillingServerActive( )
{
	__ENTER_FUNCTION

		return m_BillingPlayer.IsValid();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ServerManager::ConnectWorldServer( )
{

INT	step = 0;

__ENTER_FUNCTION

	BOOL ret ;
	uint i = 0 ;
	SSConnect* pMsg=NULL ;
	
	Socket* pSocket = m_WorldPlayer.GetSocket() ;
	_SERVER_WORLD* pWorld = &(g_Config.m_ServerInfo.m_World) ;

	_MY_TRY
	{
		ret = pSocket->create() ;
		if( !ret )
		{
			step = 1;
			Assert( FALSE ) ;
		
		}

		ret = pSocket->connect(	pWorld->m_IP, pWorld->m_Port ) ;							
		if( !ret )
		{
			step = 2;
			printf("exeption 2");
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking( ) ;
		if( !ret )
		{
			step = 3;
			printf("exeption 3");
			Assert( FALSE ) ;
			
		}

		ret = pSocket->setLinger( 0 ) ;
		if( !ret )
		{
			step = 4;
			printf("exeption 4");
			Assert( FALSE ) ;
			
		}

		g_pLog->FastSaveLog(LOG_FILE_0, "ConnectServer( ) IP:%s Port:%d ...OK ",
			pWorld->m_IP, pWorld->m_Port ) ;
	}
	_MY_CATCH
	{
		step = 5;
		Assert( FALSE ) ;
		
	}

	ret = AddServer( pSocket->getSOCKET() ) ;
	if( !ret )
	{
		step = 6;
		Assert( FALSE ) ;
		
	}

	pMsg = new SSConnect ;
	pMsg->SetServerID( g_Config.m_LoginInfo.m_LoginID) ;
	pMsg->SetZoneID(g_Config.m_WorldInfo.m_ZoneID);
	pMsg->SetWorldID(g_Config.m_WorldInfo.m_WorldID);
	ret = m_WorldPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		step = 7;
		Assert( FALSE ) ;
		
	}

	g_pLog->FastSaveLog(LOG_FILE_0, "ConnectWorldServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_LoginInfo.m_LoginID ) ;

	return TRUE ;

EXCEPTION:

	g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ConnectWorldServer( ) IP:%s Port:%d ServerID:%d,Step: %d ...Faile ",
		pWorld->m_IP, pWorld->m_Port, g_Config.m_LoginInfo.m_LoginID,step) ;
	
	m_WorldPlayer.CleanUp( ) ;	

__LEAVE_FUNCTION
	
	g_pLog->FastSaveLog(LOG_FILE_0, "Exception: ConnectWorldServer() step : %d",step) ;
	
	return FALSE ;
}

BOOL ServerManager::ConnectBillingServer( )
{
	__ENTER_FUNCTION

	BOOL ret ;
	uint i = 0 ;
	LBConnect* pMsg=NULL ;

	Socket* pSocket = m_BillingPlayer.GetSocket() ;
	_BILLING_INFO* pBilling = &(g_Config.m_BillingInfo) ;

	if(FALSE == pBilling->IsUse())
	{
		g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ConnectBillingServer( ) Current BillingInfo is not READY!! ");
		return FALSE;
	}

	_BILLING_DATA* pBillingData = pBilling->Next();

	_MY_TRY
	{		
		ret = pSocket->create() ;
		if( !ret )
		{
			Assert( FALSE ) ;
		}

		ret = pSocket->connect(	pBillingData->m_IP, pBillingData->m_Port ) ;							
		if( !ret )
		{
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking( ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			
		}

		ret = pSocket->setLinger( 0 ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			
		}

		g_pLog->FastSaveLog(LOG_FILE_0, "ConnectServer( ) Pos:%d IP:%s Port:%d ...OK ",
			pBillingData->m_ContainerPos,pBillingData->m_IP, pBillingData->m_Port ) ;
	}
	_MY_CATCH
	{
		Assert( FALSE ) ;
		
	}

	ret = AddServer( pSocket->getSOCKET() ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		
	}

	pMsg = new LBConnect ;
	pMsg->SetZoneID(g_Config.m_WorldInfo.m_ZoneID);
	pMsg->SetWorldID(g_Config.m_WorldInfo.m_WorldID);
	pMsg->SetIPAddr((g_pServerManager->GetCurrentServerInfo())->m_IP1);
	ret = m_BillingPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		
	}
	m_BillingPlayer.SetPlayerStatus(PS_BILLING_EMPTY);



	g_pLog->FastSaveLog(LOG_FILE_0, "ConnectBillingServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_LoginInfo.m_LoginID ) ;

	return TRUE ;

EXCEPTION:

	g_pLog->FastSaveLog(LOG_FILE_0, "ERROR: ConnectBillingServer( ) Pos:%d IP:%s Port:%d ServerID:%d ...Faile ",
		pBillingData->m_ContainerPos,pBillingData->m_IP, pBillingData->m_Port, g_Config.m_LoginInfo.m_LoginID ) ;

	m_BillingPlayer.CleanUp( ) ;

	__LEAVE_FUNCTION

	g_pLog->FastSaveLog(LOG_FILE_0, "Exception: ConnectBillingServer") ;

	return FALSE ;
}

BOOL ServerManager::SendQueuePlayerToWorld()
{
	__ENTER_FUNCTION
		
#define	POS_PLAYER_COUNT 100 //World至少拥有100个空闲玩家后，才继续放人进去

		INT nRequireCount = g_WorldPlayerCounter.GetRequirePlayerCount();
		if(nRequireCount>(INT)(POS_PLAYER_COUNT+g_pWorldPlayerQueue->GetSendCount()))
		{//如果队列里面的人数低于阀值

			INT nQueueCount = nRequireCount-POS_PLAYER_COUNT;
			(nQueueCount>(INT)g_pWorldPlayerQueue->GetCount())?nQueueCount=(INT)g_pWorldPlayerQueue->GetCount():nQueueCount=nQueueCount;
			for( INT i=0; i<nQueueCount; i++ )
			{
				UINT QueuePos ;
				if( g_pWorldPlayerQueue->FindHeadPlayer(QueuePos) )
				{//从队列里取出第一个玩家
					WORLD_PLAYER_INFO& PlayerInfo = g_pWorldPlayerQueue->GetPlayer(QueuePos);
					LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(PlayerInfo.PlayerID);
					Assert(pLoginPlayer);

					if( pLoginPlayer->IsGUIDOwner(PlayerInfo.Guid) &&
						pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WORLD_PROCESS_TURN )
					{
						SceneID_t TarSceneID = pLoginPlayer->GetChooseSceneID() ;
						ID_t TarServerID = INVALID_ID ;
						INT nSceneCount =0 ;
						INT nServerCount = 0 ; 
						if( TarSceneID>=0 && TarSceneID<(INT)(g_Config.m_SceneInfo.m_SceneCount) )
						{
							TarServerID = g_Config.m_SceneInfo.m_pScene[TarSceneID].m_ServerID ;
							nSceneCount = g_pServerManager->GetScenePlayerCount(TarSceneID) ;
							nServerCount = g_pServerManager->GetServerPlayerPoolSize(TarServerID) ;
						}

						if( nSceneCount >= MAX_PLAYER-128 ||
							nServerCount >= MAX_POOL_SIZE-128 )
						{//如果目标场景人数或者目标服务器人数过多，则继续排队
							WORLD_PLAYER_INFO pi = PlayerInfo ;
							BOOL bClean = g_pWorldPlayerQueue->GetOutPlayer(QueuePos);
							if(!bClean)
							{
								Assert(FALSE);
							}
							UINT QPos ;
							g_pWorldPlayerQueue->AddInPlayer(	pi.PlayerID,
																pi.PlayerName,
																pi.Guid,
																QPos ) ;

							g_pLog->FastSaveLog(LOG_FILE_0, "SendQueuePlayerToWorld...Server Full Or Scene Full..GUID=%d Scene=%d nSceneCount=%d nServerCount=%d",
								pi.Guid, TarSceneID, nSceneCount, nServerCount);
							continue ;
						}

						LWAskCharLogin	Msg;
						Msg.SetAccount(PlayerInfo.PlayerName);
						Msg.SetPlayerID(PlayerInfo.PlayerID);
						Msg.SetPlayerGUID(PlayerInfo.Guid);
						Msg.SetAskStatus(ALS_ASKSTATUS);
						Msg.SetUserKey(pLoginPlayer->GetUserKey());
						Msg.GetUserData()->m_Human.m_nCountry = 1;

						m_WorldPlayer.SendPacket(&Msg);

						pLoginPlayer->SetPlayerStatus(PS_LOGIN_NORMAL);
						pLoginPlayer->ResetKick();
                        pLoginPlayer->SetQueuePos(0);
						g_pWorldPlayerQueue->AddSendCount();

						g_pLog->FastSaveLog(LOG_FILE_0, "SendQueuePlayerToWorld...Send Success..GUID=%d Scene=%d",
							PlayerInfo.Guid, TarSceneID);
					}
					else
					{
						//错误操作
					}

					BOOL bCleanPlayer = g_pWorldPlayerQueue->GetOutPlayer(QueuePos);
					if(!bCleanPlayer)
					{
						Assert(FALSE);
					}
				}

			}
		}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ServerManager::ResetQueuePos()
{
__ENTER_FUNCTION
	g_pWorldPlayerQueue->SortQueuePos();
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL ServerManager::SendPacket( Packet* pPacket, ID_t ServerID, uint Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = ServerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//缓冲区中没有消息
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


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
	
BOOL ServerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( uint i=0; i<MAX_CACHE_SIZE; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;
			
		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = GetServerPlayer(PlayerID);
				Assert(pPlayer);

				if(pPlayer->IsValid())
				{
					uint uret = pPacket->Execute(pPlayer) ;
					if( uret == PACKET_EXE_ERROR )
					{
						RemoveServer( PlayerID ) ;
				
					}
					else if( uret == PACKET_EXE_BREAK )
					{
					}
					else if( uret == PACKET_EXE_CONTINUE )
					{
					}
					else if( uret == PACKET_EXE_NOTREMOVE )
					{

					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						RemoveServer( PlayerID ) ;
						
					}
				}
				else
				{
					//用户已经不在了这个消息不用处理
					//但是必须删除
				}
				
				
			}
			_MY_CATCH
			{
			}
		}
		//服务器之间的消息永远都回收
		g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL  ServerManager::ResizeCache( )
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

INT ServerManager::GetServerPlayerPoolSize( ID_t ServerID )
{
__ENTER_FUNCTION

	Assert( ServerID>=0 && ServerID<MAX_SERVER ) ;
	return m_aServerPlayerPoolSize[ServerID] ;

__LEAVE_FUNCTION
	return 0 ;
}

VOID ServerManager::SetServerPlayerPoolSize( ID_t ServerID, INT nPlayerPoolSize )
{
__ENTER_FUNCTION

	Assert( ServerID>=0 && ServerID<MAX_SERVER ) ;
	m_aServerPlayerPoolSize[ServerID] = nPlayerPoolSize ;

__LEAVE_FUNCTION
}

