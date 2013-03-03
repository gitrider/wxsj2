#include "stdafx.h"
#include "GCEquipDecompoundResult.h"


BOOL	GCEquipDecompoundResult::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)&m_Result,sizeof(EQUIPDECOMPOUND_RESULT));
	iStream.Read((CHAR*)&m_EquipPoint,sizeof(BYTE));
	
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	GCEquipDecompoundResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write((CHAR*)&m_Result,sizeof(EQUIPDECOMPOUND_RESULT));
	oStream.Write((CHAR*)&m_EquipPoint,sizeof(BYTE));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	GCEquipDecompoundResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipDecompoundResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}