#include "stdafx.h"
#include "GCEquipChangePaiTypeResult.h"

BOOL	GCEquipChangePaiTypeResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPCHANGEPAITTYPE_RESULT));		
	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCEquipChangePaiTypeResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPCHANGEPAITTYPE_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCEquipChangePaiTypeResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipChangePaiTypeResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
