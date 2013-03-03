#include "stdafx.h"
#include  "GCOpenBoxManipulate.h"

BOOL
GCOpenBoxManipulate::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)( &m_nManipulateType ), sizeof(INT));
		iStream.Read( (CHAR*)( &m_nRet ), sizeof(INT) );
		iStream.Read( (CHAR*)( &m_GUID ), sizeof(_ITEM_GUID) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;

}

BOOL
GCOpenBoxManipulate::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	   oStream.Write( (CHAR*)( &m_nManipulateType ), sizeof(INT));
	   oStream.Write( (CHAR*)( &m_nRet ), sizeof(INT) );
	   oStream.Write( (CHAR*)( &m_GUID ), sizeof(_ITEM_GUID) );
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT
GCOpenBoxManipulate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCOpenBoxManipulateHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}