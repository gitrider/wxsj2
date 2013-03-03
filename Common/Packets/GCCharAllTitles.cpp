// GCCharAllTitles.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCCharAllTitles.h"

BOOL GCCharAllTitles::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_TitleInfo), sizeof(_TITLE));
	iStream.Read( (CHAR*)(&m_cType), sizeof(BYTE));    

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharAllTitles::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_TitleInfo), sizeof(_TITLE));
	oStream.Write( (CHAR*)(&m_cType), sizeof(BYTE));    

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharAllTitles::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCCharAllTitlesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

