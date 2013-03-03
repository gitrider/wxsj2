

#include "stdafx.h"
#include "CGAskSubTrainMode.h"



BOOL CGAskSubTrainMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_nMode, sizeof(BYTE) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskSubTrainMode::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_nMode, sizeof(BYTE) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGAskSubTrainMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskSubTrainModeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}