#include "stdafx.h"
#include "CGSouXiaSew.h"

CGSouXiaSew::CGSouXiaSew(VOID):m_nSewSouXia(0),m_nSewItem(0),m_nRelicItem(0)
{
}

CGSouXiaSew::~CGSouXiaSew(VOID)
{
}

BOOL CGSouXiaSew::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nSewSouXia), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_nSewItem), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_nRelicItem), sizeof(BYTE) ) ;

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGSouXiaSew::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nSewSouXia), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nSewItem), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nRelicItem), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGSouXiaSew::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGSouXiaSewHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}


