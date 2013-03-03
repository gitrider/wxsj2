// WGCallOfHuman.cpp

#include "stdafx.h"
#include "WGCallOfHuman.h"

BOOL WGCallOfHuman::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nTargetPlayerID), sizeof(m_nTargetPlayerID) );
	iStream.Read( (CHAR*)(&m_infoCallOf), sizeof(m_infoCallOf) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCallOfHuman::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nTargetPlayerID), sizeof(m_nTargetPlayerID) );
	oStream.Write( (CHAR*)(&m_infoCallOf), sizeof(m_infoCallOf) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCallOfHuman::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCallOfHumanHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


