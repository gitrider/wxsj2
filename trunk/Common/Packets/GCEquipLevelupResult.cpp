#include "stdafx.h"
#include "GCEquipLevelupResult.h"

BOOL	GCEquipLevelupResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPLEVEUP_RESULT));		

	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCEquipLevelupResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPLEVEUP_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCEquipLevelupResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipLevelupResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
