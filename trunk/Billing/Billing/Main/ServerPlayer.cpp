
#include "stdafx.h"
#include "ServerPlayer.h"
#include "PacketFactoryManager.h"


ServerPlayer::ServerPlayer( )
{
__ENTER_FUNCTION

	m_pServerData = NULL ;
	m_Status = 0 ;

__LEAVE_FUNCTION
}

ServerPlayer::~ServerPlayer( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL ServerPlayer::ProcessCommand( bool Option )
{
__ENTER_FUNCTION

	return BillingPlayer::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return BillingPlayer::ProcessInput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return BillingPlayer::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::SendPacket( BillPacket* pPacket )
{
__ENTER_FUNCTION

	return BillingPlayer::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::SendPacket( Packet* pPacket )
{
	__ENTER_FUNCTION

		return Player::SendPacket( pPacket ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL ServerPlayer::IsValid( )
{
__ENTER_FUNCTION

	if( !BillingPlayer::IsValid( ) )
		return FALSE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

void ServerPlayer::CleanUp( )
{
__ENTER_FUNCTION

	BillingPlayer::CleanUp( ) ;

	m_pServerData = NULL ;	

__LEAVE_FUNCTION
}

BOOL ServerPlayer::HeartBeat( DWORD dwTime )
{
__ENTER_FUNCTION


	return BillingPlayer::HeartBeat( dwTime ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

void ServerPlayer::FreeOwn( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL ServerPlayer::Init( )
{
__ENTER_FUNCTION

	SetDisconnect( FALSE ) ;
	ResetKick( ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

