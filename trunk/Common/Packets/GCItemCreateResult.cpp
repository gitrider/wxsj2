#include "stdafx.h"
#include "GCItemCreateResult.h"

BOOL	GCItemCreateResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(ITEMCREATE_RESULT));		

	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCItemCreateResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(ITEMCREATE_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCItemCreateResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCItemCreateResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
