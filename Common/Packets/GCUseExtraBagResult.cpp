#include "stdafx.h"
#include "GCUseExtraBagResult.h"


using namespace Packets;


BOOL GCUseExtraBagResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_SourIndex), sizeof(INT));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(INT));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCUseExtraBagResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_SourIndex), sizeof(INT));
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(INT));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCUseExtraBagResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCUseExtraBagResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}