#include "stdafx.h"
#include "GCPickDropItemResult.h"

using namespace Packets;


BOOL GCPickDropItemResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCPickDropItemResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCPickDropItemResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCPickDropItemResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}