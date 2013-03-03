#include "stdafx.h" 
#include "WLChangeName.h"

BOOL WLChangeName::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return m_sPacketUnit.ReadStream( iStream ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WLChangeName::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return m_sPacketUnit.WriteStream( oStream ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WLChangeName::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WLChangeNameHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


