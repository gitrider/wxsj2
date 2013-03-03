

#include "stdafx.h"
#include "GCSubTrainResult.h"



BOOL GCSubTrainResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_nResult, sizeof(BYTE) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCSubTrainResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_nResult, sizeof(BYTE) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSubTrainResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSubTrainResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}