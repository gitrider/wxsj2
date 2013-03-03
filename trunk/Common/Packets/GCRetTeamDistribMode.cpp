#include "stdafx.h"
#include "GCRetTeamDistribMode.h"


BOOL GCRetTeamDistribMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_nMode), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCRetTeamDistribMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nMode), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCRetTeamDistribMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCRetTeamDistribModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}