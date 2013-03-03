

#include "stdafx.h"
#include "CGCleanSubTrainInfo.h"



BOOL CGCleanSubTrainInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGCleanSubTrainInfo::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGCleanSubTrainInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGCleanSubTrainInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
