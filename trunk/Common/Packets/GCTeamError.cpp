#include "stdafx.h"
#include  "GCTeamError.h"


BOOL		GCTeamError::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Error), sizeof(BYTE) ) ;

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GCTeamError::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Error), sizeof(BYTE) ) ;

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GCTeamError::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GCTeamErrorHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

