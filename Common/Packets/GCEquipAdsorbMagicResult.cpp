#include "stdafx.h"
#include "GCEquipAdsorbMagicResult.h"

BOOL	GCEquipAdsorbMagicResult::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_Result,sizeof(EQUIPADSORMAGIC_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	GCEquipAdsorbMagicResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_Result,sizeof(EQUIPADSORMAGIC_RESULT));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	GCEquipAdsorbMagicResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipAdsorbMagicResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
