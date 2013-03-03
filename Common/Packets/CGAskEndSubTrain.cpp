

#include "stdafx.h"
#include "CGAskEndSubTrain.h"



BOOL CGAskEndSubTrain::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskEndSubTrain::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGAskEndSubTrain::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskEndSubTrainHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}