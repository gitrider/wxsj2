
#include "stdafx.h"
#include  "GWTeamLeaderDie.h"


BOOL		GWTeamLeaderDie::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GWTeamLeaderDie::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GWTeamLeaderDie::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GWTeamLeaderDieHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

