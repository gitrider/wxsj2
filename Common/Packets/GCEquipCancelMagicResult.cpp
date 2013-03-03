#include "stdafx.h"
#include "GCEquipCancelMagicResult.h"

BOOL	GCEquipCancelMagicResult::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_Result,sizeof(EQUIPCANCELMAGIC_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	GCEquipCancelMagicResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_Result,sizeof(EQUIPCANCELMAGIC_RESULT));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	GCEquipCancelMagicResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipCancelMagicResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
