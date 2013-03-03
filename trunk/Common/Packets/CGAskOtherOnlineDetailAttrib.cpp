// CGAskOtherOnlineDetailAttrib.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGAskOtherOnlineDetailAttrib.h"

BOOL CGAskOtherOnlineDetailAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_Guid), sizeof(GUID_t) ) ;
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskOtherOnlineDetailAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_Guid), sizeof(GUID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskOtherOnlineDetailAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskOtherOnlineDetailAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

