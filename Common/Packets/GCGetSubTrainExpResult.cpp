

#include "stdafx.h"
#include "GCGetSubTrainExpResult.h"



BOOL GCGetSubTrainExpResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCGetSubTrainExpResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCGetSubTrainExpResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCGetSubTrainExpResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}