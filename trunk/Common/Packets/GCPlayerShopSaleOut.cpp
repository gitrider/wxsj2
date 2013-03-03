// GCPlayerShopSaleOut.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCPlayerShopSaleOut.h"

BOOL GCPlayerShopSaleOut::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bSaleOut), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopSaleOut::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_bSaleOut), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopSaleOut::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopSaleOutHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

