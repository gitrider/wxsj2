#include "stdafx.h"
#include "GCWorldTime.h"


BOOL	GCWorldTime::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)(&m_Time),sizeof(WORLD_TIME));
		iStream.Read((CHAR*)(&m_SystemTime),sizeof(m_SystemTime));
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	GCWorldTime::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)(&m_Time),sizeof(WORLD_TIME));
		oStream.Write((CHAR*)(&m_SystemTime),sizeof(m_SystemTime));
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT GCWorldTime::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCWorldTimeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}