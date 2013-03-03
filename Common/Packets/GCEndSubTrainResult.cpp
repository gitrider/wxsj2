

#include "stdafx.h"
#include "GCEndSubTrainResult.h"



BOOL GCEndSubTrainResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)&m_nCountMoney, sizeof(UINT) );
	iStream.Read( (CHAR*)&m_nCountExp, sizeof(UINT) );

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCEndSubTrainResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)&m_nCountMoney, sizeof(UINT) );
	oStream.Write( (CHAR*)&m_nCountExp, sizeof(UINT) );

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCEndSubTrainResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEndSubTrainResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}