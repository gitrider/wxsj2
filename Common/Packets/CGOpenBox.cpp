#include "stdafx.h"
#include  "CGOpenBox.h"

BOOL
CGOpenBox::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_BoxID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;

}

BOOL
CGOpenBox::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

      oStream.Write( (CHAR*)(&m_BoxID), sizeof(UINT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT
CGOpenBox::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGOpenBoxHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}