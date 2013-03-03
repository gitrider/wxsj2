#include "stdafx.h"
#include  "CGOpenBoxManipulate.h"

BOOL CGOpenBoxManipulate::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nManipulateType), sizeof(INT));	

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;

}

BOOL CGOpenBoxManipulate::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nManipulateType), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGOpenBoxManipulate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGOpenBoxManipulateHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}