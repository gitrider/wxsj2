#include "stdafx.h"
#include "GCEquipAddHoleResult.h"


BOOL GCEquipAddHoleResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCEquipAddHoleResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCEquipAddHoleResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCEquipAddHoleResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}