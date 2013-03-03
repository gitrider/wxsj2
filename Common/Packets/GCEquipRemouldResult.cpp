#include "stdafx.h"
#include "GCEquipRemouldResult.h"


BOOL	GCEquipRemouldResult::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_Result,sizeof(EQUIPREMOULD_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	GCEquipRemouldResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_Result,sizeof(EQUIPREMOULD_RESULT));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	GCEquipRemouldResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipRemouldResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}