

#include "stdafx.h"
#include "CGAskGetSubTrainExp.h"



BOOL CGAskGetSubTrainExp::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_nMode, sizeof(BYTE) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskGetSubTrainExp::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_nMode, sizeof(BYTE) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGAskGetSubTrainExp::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskGetSubTrainExpHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}