#include "stdafx.h"
#include  "CGTeamKick.h"


BOOL		CGTeamKick::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_DestGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGTeamKick::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_DestGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGTeamKick::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGTeamKickHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

