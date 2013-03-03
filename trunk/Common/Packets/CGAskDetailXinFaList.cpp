// CGAskDetailXinFaList.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGAskDetailXinFaList.h"

BOOL CGAskDetailXinFaList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskDetailXinFaList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskDetailXinFaList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskDetailXinFaListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

