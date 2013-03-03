

#include "stdafx.h"
#include "GCSubTrainModeResult.h"



BOOL GCSubTrainModeResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_nMoneyRate, sizeof(UINT) );
	iStream.Read( (CHAR*)&m_nExpRate, sizeof(UINT) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCSubTrainModeResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_nMoneyRate, sizeof(UINT) );
	oStream.Write( (CHAR*)&m_nExpRate, sizeof(UINT) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSubTrainModeResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSubTrainModeResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}