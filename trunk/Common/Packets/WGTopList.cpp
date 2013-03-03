

#include "stdafx.h"
#include "WGTopList.h"


BOOL WGTopList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_TopList.Read( iStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGTopList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_TopList.Write( oStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGTopList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTopListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


