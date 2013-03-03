
#include "stdafx.h"
#include "GCTopList.h"


BOOL	GCTopList::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		m_TopList.Read( iStream ) ;
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	GCTopList::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
		m_TopList.Write( oStream ) ;
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT GCTopList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCTopListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
