#include "stdafx.h"
#include "GCSouXiaDiscard.h"

using namespace Packets;


BOOL GCSouXiaDiscard::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_CurPos), sizeof(SHORT));
	iStream.Read( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCSouXiaDiscard::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_CurPos), sizeof(SHORT));
	oStream.Write( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	GCSouXiaDiscard::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCSouXiaDiscardHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}