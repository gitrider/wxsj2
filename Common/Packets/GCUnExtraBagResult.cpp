#include "stdafx.h"
#include "GCUnExtraBagResult.h"

using namespace Packets;
BOOL GCUnExtraBagResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(INT));
	iStream.Read( (CHAR*)(&m_DestIndex), sizeof(INT));
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCUnExtraBagResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(INT));
	oStream.Write( (CHAR*)(&m_DestIndex), sizeof(INT));
	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCUnExtraBagResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCUnExtraBagResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}