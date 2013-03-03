#include "stdafx.h"
#include "GCSouXiaSew.h"

GCSouXiaSew::GCSouXiaSew(VOID):m_nSewSouXia(0),m_nSewItem(0),m_nRelicItem(0)
{
}

GCSouXiaSew::~GCSouXiaSew(VOID)
{
}

BOOL Packets::GCSouXiaSew::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION
	
	m_SouXiaData.ReadSouXiaVarAttr(iStream);
	iStream.Read( (CHAR*)(&m_nResult), sizeof(BYTE) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::GCSouXiaSew::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	m_SouXiaData.WriteSouXiaVarAttr(oStream);
	oStream.Write( (CHAR*)(&m_nResult), sizeof(BYTE) ) ;

	return TRUE ;
	__LEAVE_FUNCTION
	
	return FALSE ;
}

UINT Packets::GCSouXiaSew::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSouXiaSewHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
		return FALSE ;
}


