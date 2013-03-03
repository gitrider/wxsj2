#include "stdafx.h"
#include "GCEquipExChangePaiTypeResult.h"

BOOL	GCEquipExChangePaiTypeResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPEXCHANGEPAITTYPE_RESULT));		
	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCEquipExChangePaiTypeResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPEXCHANGEPAITTYPE_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCEquipExChangePaiTypeResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipExChangePaiTypeResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
