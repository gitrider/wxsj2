#include "stdafx.h"
#include "CGAskTeamDistribMode.h"


BOOL CGAskTeamDistribMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGAskTeamDistribMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT CGAskTeamDistribMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGAskTeamDistribModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}