#include "stdafx.h"
#include "GCShopSpecialRepairResult.h"

BOOL	GCShopSpecialRepairResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPSPECIALREPAIR_RESULT));		
	iStream.Read( (CHAR*)(&m_aRepairedItem), sizeof(REPAIRED_ITEM));

	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCShopSpecialRepairResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPSPECIALREPAIR_RESULT));
	oStream.Write( (CHAR*)(&m_aRepairedItem), sizeof(REPAIRED_ITEM));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCShopSpecialRepairResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopSpecialRepairResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}
