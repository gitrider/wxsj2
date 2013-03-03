#include "stdafx.h" 
#include "LWChangeName.h"

BOOL LWChangeName::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return m_sPacketUnit.ReadStream( iStream ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LWChangeName::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return m_sPacketUnit.WriteStream( oStream ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT LWChangeName::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return LWChangeNameHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


