#include "stdafx.h"
#include "GCOpenBox.h"

BOOL GCOpenBox::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)( &m_nRet ), sizeof(INT) );
		
		for ( INT i=0; i<OPENBOX_ITEM_SIZE; ++i )
		{
		m_Item[i].ReadItemVarAttr(iStream);
		}

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCOpenBox::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)( &m_nRet ), sizeof(INT) );

		for ( INT i=0; i<OPENBOX_ITEM_SIZE; ++i )
		{
			m_Item[i].WriteItemVarAttr(oStream);
		}


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}


UINT
GCOpenBox::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCOpenBoxHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}