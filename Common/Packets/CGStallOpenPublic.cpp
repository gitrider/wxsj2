// CGStallOpenPublic.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGStallOpenPublic.h"

BOOL CGStallOpenPublic::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_bOpenPublic), sizeof(BOOL));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallOpenPublic::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_bOpenPublic), sizeof(BOOL));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallOpenPublic::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallOpenPublicHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

