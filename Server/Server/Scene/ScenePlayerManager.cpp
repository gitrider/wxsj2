
#include "stdafx.h"
#include "ScenePlayerManager.h"
#include "Scene.h"
#include "Log.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "SceneManager.h"
#include "ServerManager.h"
#include "GWNotifyUser.h"
#include "PacketFactoryManager.h"



ScenePlayerManager::ScenePlayerManager( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_USE] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_SceneID = INVALID_ID ;

__LEAVE_FUNCTION
}

ScenePlayerManager::~ScenePlayerManager( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL ScenePlayerManager::Select( )
{
__ENTER_FUNCTION

//	if( m_nPlayers == 0 )
	{//线程合并后会影响效率
		//MySleep( 100 ) ;
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
			Assert(FALSE) ;
		}
	} 
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据读取
	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		//连接已断开
		if( pPlayer->IsDisconnect() )
			continue;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//连接出现错误
				RemovePlayer( pPlayer ) ;
			}
			else
			{//连接正常
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

BOOL ScenePlayerManager::ProcessOutputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据发送
	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i]==INVALID_ID )
			continue ;

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		Assert( pPlayer ) ;

		if( pPlayer->IsDisconnect( ) )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( pPlayer->GetSocket()->isSockError() )
			{//连接出现错误
				RemovePlayer( pPlayer ) ;
			}
			else
			{//连接正常
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

BOOL ScenePlayerManager::ProcessExceptions( )
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

		//某个玩家断开网络连接

		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
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

BOOL ScenePlayerManager::ProcessCommands( )
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

		if( pPlayer->IsDisconnect() )
			continue ;

		SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
		Assert( s!=INVALID_SOCKET ) ;

		if( pPlayer->GetSocket()->isSockError() )
		{//连接出现错误
			RemovePlayer( pPlayer ) ;
		}
		else
		{//连接正常
			_MY_TRY
			{
				ret = pPlayer->ProcessCommand( FALSE ) ;
				if( !ret )
				{
					RemovePlayer( pPlayer, TRUE ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
				RemovePlayer( pPlayer, TRUE ) ;
			}
		}
	}




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::RemovePlayer( Player* pPlayer, BOOL bReal )
{
__ENTER_FUNCTION

	Assert( pPlayer ) ;

	BOOL ret = FALSE ;

	//第一步：清除Socket信息
	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	if( fd != INVALID_SOCKET )
	{
		DelPlayerSocket( fd ) ;
		pPlayer->Disconnect( ) ;

		Log::SaveLog( SERVER_LOGFILE, "ScenePlayerManager::RemovePlayer Disconnect(SOCKET=%d)...OK", 
			fd ) ;
	}

	if( !bReal )
	{//仅仅断线
		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;

		if( !pGamePlayer->IsDirty( ) )
		{
			Scene* pScene = g_pSceneManager->GetScene( GetSceneID() ) ;
			pScene->OnScenePlayerDisconnect( pGamePlayer ) ;
			
			GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
			pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
			pMsg->SetStatus( GWNotifyUser::NUS_DISCONNECT ) ;
			g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
		}
		return TRUE ;
	}

	//第二步：清除场景中的相关数据
	Scene* pScene;
	_MY_TRY
	{
		pScene = g_pSceneManager->GetScene( GetSceneID() ) ;
		if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
		Obj_Human *pHuman = pGamePlayer->GetHuman() ;

		if( pHuman && pHuman->getScene() && pHuman->IsActiveObj() )
		{
			pScene->OnScenePlayerLeave( pGamePlayer ) ;

			pGamePlayer->GetHuman()->SetActiveFlag( FALSE );
			pScene->ObjectLeaveScene( pHuman ) ;
		}
		Assert( pGamePlayer->GetHuman()->HumanNode()->m_pNext == NULL ) ;
		Assert( pGamePlayer->GetHuman()->HumanNode()->m_pPrev == NULL ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	//第三步：清除PlayerManager中的信息
	if( pPlayer->PlayerManagerID()!=INVALID_ID )
	{
		ret = DelPlayer( pPlayer->PlayerID() ) ;
		Assert( ret ) ;
	}

	//第四步：清除PlayerPool中的信息，注意此步骤必须放在最后，
	//当调用此操作后，当前Player就有可能会被马上分配给新接入玩家
	GUID_t guid = ((GamePlayer*)pPlayer)->GetHuman()->GetGUID() ;
	if( guid!=INVALID_ID )
	{
		((GamePlayer*)pPlayer)->ChooseFreeOwn(pScene->GetRecyclePlayerManager()) ;
	}


	Log::SaveLog( SERVER_LOGFILE, "ScenePlayerManager::Real RemovePlayer(GUID=%X)...OK", 
		guid ) ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION

	if( m_nFDSize>=FD_SETSIZE )
	{//已经超出能够检测的网络句柄最大数；
		return FALSE ;
	}

	BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}


	SOCKET fd = pPlayer->GetSocket()->getSOCKET() ;
	AddPlayerSocket( fd ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::AddPlayerSocket( SOCKET fd )
{
__ENTER_FUNCTION

	Assert(fd!=INVALID_SOCKET) ;
	if( FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]) )
	{
		return FALSE ;
	}

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd , m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd,m_MaxFD));

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::DelPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( pid!=INVALID_ID) ;

//清除PlayerManager中的信息
	PlayerManager::RemovePlayer( pid ) ;




	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::DelPlayerSocket( SOCKET fd )
{
__ENTER_FUNCTION

	if( m_MinFD==INVALID_SOCKET )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR ScenePlayerManager::DelPlayerSocket m_MinFD (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( m_MaxFD==INVALID_SOCKET )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR ScenePlayerManager::DelPlayerSocket m_MaxFD (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( fd == INVALID_SOCKET )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR ScenePlayerManager::DelPlayerSocket fd (SOCKET=%d)", 
			fd ) ;
		return FALSE ;
	}
	if( !FD_ISSET(fd,&m_ReadFDs[SELECT_BAK]) )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR ScenePlayerManager::DelPlayerSocket(SOCKET=%d)", 
			fd ) ;
		return FALSE ;
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
			m_MaxFD = s ;
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



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScenePlayerManager::HeartBeat( UINT uTime )
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

		_MY_TRY
		{
			ret = pPlayer->HeartBeat(uTime ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
			ret = FALSE ;
		}
		if( !ret )
		{//如果逻辑操作返回失败，则需要断开当前连接
			ret = RemovePlayer( pPlayer, TRUE ) ;
			Assert( ret ) ;
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID ScenePlayerManager::RemoveAllPlayer( )
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

		RemovePlayer( pPlayer, TRUE ) ;
	}

__LEAVE_FUNCTION
}
