#include "stdafx.h"
#include "CGUnExtraBag.h"

using namespace Packets;

BOOL CGUnExtraBag::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_BagPoint), sizeof(INT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGUnExtraBag::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_BagPoint), sizeof(INT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGUnExtraBag::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGUnExtraBagHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}