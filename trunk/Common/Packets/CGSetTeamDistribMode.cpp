#include "stdafx.h"
#include "CGSetTeamDistribMode.h"


BOOL CGSetTeamDistribMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_nMode), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGSetTeamDistribMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nMode), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT CGSetTeamDistribMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGSetTeamDistribModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}