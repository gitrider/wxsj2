#include "stdafx.h"
#include "CGSouXiaDismantle.h"

CGSouXiaDismantle::CGSouXiaDismantle(VOID)
{
}

CGSouXiaDismantle::~CGSouXiaDismantle(VOID)
{
}

BOOL Packets::CGSouXiaDismantle::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nDismantleSouXia), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_nDismantleItem), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::CGSouXiaDismantle::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nDismantleSouXia), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nDismantleItem), sizeof(BYTE) ) ;

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT Packets::CGSouXiaDismantle::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGSouXiaDismantleHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
		return FALSE ;
}


