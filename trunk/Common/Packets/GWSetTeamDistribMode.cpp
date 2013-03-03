#include "stdafx.h"
#include "GWSetTeamDistribMode.h"


BOOL GWSetTeamDistribMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_nMode), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_TeamID), sizeof(m_TeamID));
	iStream.Read( (CHAR*)(&m_LeaderGuid), sizeof(GUID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GWSetTeamDistribMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nMode), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_TeamID), sizeof(m_TeamID));
	oStream.Write( (CHAR*)(&m_LeaderGuid), sizeof(GUID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GWSetTeamDistribMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GWSetTeamDistribModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}