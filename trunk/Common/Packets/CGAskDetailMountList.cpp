// [7/8/2010 �¾���]
#include "stdafx.h"
#include "CGAskDetailMountList.h"

BOOL CGAskDetailMountList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskDetailMountList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT	CGAskDetailMountList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskDetailMountListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}