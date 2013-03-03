// CGShopSpecialRepair.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGShopSpecialRepair.h"

BOOL CGShopSpecialRepair::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nItemNum), sizeof(BYTE));


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGShopSpecialRepair::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nItemNum), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGShopSpecialRepair::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGShopSpecialRepairHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
