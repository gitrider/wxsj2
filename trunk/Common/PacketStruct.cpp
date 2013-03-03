#include "stdafx.h" 
#include "PacketStruct.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

using namespace Packets ;

//////////////////////////////////////////////////
// About Change Name Packet
SChgNamePacketUnit::SChgNamePacketUnit()
{
	m_opType		= CHGNAME_OP_INVALID ;
	m_UserGuid		= INVALID_GUID ;
	memset( m_UserOldName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX ) ;
	memset( m_UserNewName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX ) ;
	m_reType		= CHGNAME_RE_OK ;
}

VOID SChgNamePacketUnit::SetUserOldName( INT OpType, const CHAR* pName )
{
__ENTER_FUNCTION

	if( pName == NULL )
		return ;

	m_opType		= OpType ;
	memset( m_UserOldName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX ) ;

	switch( OpType )
	{
	case CHGNAME_OP_USER :
        strncpy( m_UserOldName, pName, MAX_CHARACTER_NAME-1 ) ;
		break ;
	case CHGNAME_OP_GUILD :
        strncpy( m_UserOldName, pName, MAX_GUILD_NAME_SIZE-1 ) ;
		break ;
	default :
		strncpy( m_UserOldName, pName, CHANGE_NAME_MAX-1 ) ;
		break ;
	}

__LEAVE_FUNCTION
}

VOID SChgNamePacketUnit::SetUserNewName( INT OpType, const CHAR* pName )
{
__ENTER_FUNCTION

	if( pName == NULL )
		return ;

	m_opType		= OpType ;
	memset( m_UserNewName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX ) ;

	switch( OpType )
	{
	case CHGNAME_OP_USER :
        strncpy( m_UserNewName, pName, MAX_CHARACTER_NAME-1 ) ;
		break ;
	case CHGNAME_OP_GUILD :
        strncpy( m_UserNewName, pName, MAX_GUILD_NAME_SIZE-1 ) ;
		break ;
	default :
		strncpy( m_UserNewName, pName, CHANGE_NAME_MAX-1 ) ;
		break ;
	}

__LEAVE_FUNCTION
}

UINT SChgNamePacketUnit::GetPacketSize()
{
	return sizeof(INT) +
		sizeof(GUID_t) +
		sizeof(CHAR) * (CHANGE_NAME_MAX-1) +
		sizeof(CHAR) * (CHANGE_NAME_MAX-1) +
		sizeof(INT) ;
}

BOOL SChgNamePacketUnit::ReadStream( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_opType), sizeof(INT) ) ;
	iStream.Read( (CHAR*)(&m_UserGuid), sizeof(GUID_t) ) ;
	memset( m_UserOldName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX );
	iStream.Read( (CHAR*)(&m_UserOldName), sizeof(CHAR) * (CHANGE_NAME_MAX-1) ) ;
	memset( m_UserNewName, 0, sizeof( CHAR ) * CHANGE_NAME_MAX );
	iStream.Read( (CHAR*)(&m_UserNewName), sizeof(CHAR) * (CHANGE_NAME_MAX-1) ) ;
	iStream.Read( (CHAR*)(&m_reType), sizeof(INT) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SChgNamePacketUnit::WriteStream( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_opType), sizeof(INT) ) ;
	oStream.Write( (CHAR*)(&m_UserGuid), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_UserOldName), sizeof(CHAR) * (CHANGE_NAME_MAX-1) ) ;
	oStream.Write( (CHAR*)(&m_UserNewName), sizeof(CHAR) * (CHANGE_NAME_MAX-1) ) ;
	oStream.Write( (CHAR*)(&m_reType), sizeof(INT) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

//////////////////////////////////////////////////
// About Other Packet
