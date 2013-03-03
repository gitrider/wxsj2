#include "stdafx.h"
#include "WGRetTeamDistribMode.h"


BOOL WGRetTeamDistribMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_nMode), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_TeamID), sizeof(m_TeamID));
	iStream.Read( (CHAR*)(&m_LeaderID), sizeof(PlayerID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL WGRetTeamDistribMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nMode), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_TeamID), sizeof(m_TeamID));
	oStream.Write( (CHAR*)(&m_LeaderID), sizeof(PlayerID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT WGRetTeamDistribMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return WGRetTeamDistribModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}