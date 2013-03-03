

#include "stdafx.h"
#include "CGAskBeginSubTrain.h"



BOOL CGAskBeginSubTrain::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)&m_nMode, sizeof(BYTE) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskBeginSubTrain::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_nMode, sizeof(BYTE) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGAskBeginSubTrain::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskBeginSubTrainHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}