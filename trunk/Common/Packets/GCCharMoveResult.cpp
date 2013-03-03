// GCCharMoveResult.cpp

#include "stdafx.h"
#include "GCCharMoveResult.h"

BOOL GCCharMoveResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_nResult), sizeof(m_nResult));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharMoveResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_nResult), sizeof(m_nResult));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharMoveResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharMoveResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

