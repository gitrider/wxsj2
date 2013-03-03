#include "stdafx.h"
#include "CGUseExtraBag.h"


using namespace Packets;


BOOL CGUseExtraBag::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_BagIndex), sizeof(INT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGUseExtraBag::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_BagIndex), sizeof(INT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGUseExtraBag::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGUseExtraBagHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}