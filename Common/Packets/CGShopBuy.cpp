// CGShopBuy.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGShopBuy.h"

BOOL CGShopBuy::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_uCount), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_uVersion), sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGShopBuy::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_uCount), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_uVersion), sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGShopBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGShopBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

