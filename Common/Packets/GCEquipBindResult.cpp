#include "stdafx.h"
#include "GCEquipBindResult.h"

BOOL	GCEquipBindResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPBIND_RESULT));		
	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCEquipBindResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPBIND_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCEquipBindResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipBindResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
