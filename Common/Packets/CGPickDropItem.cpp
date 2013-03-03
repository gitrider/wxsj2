#include "stdafx.h"
#include "CGPickDropItem.h"

using namespace Packets;


BOOL CGPickDropItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read((CHAR*)&m_DropItemNameSize, sizeof(UCHAR));

	if ( m_DropItemNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DropItemName, sizeof(CHAR) * m_DropItemNameSize);
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPickDropItem::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write((CHAR*)&m_DropItemNameSize, sizeof(UCHAR));

	if ( m_DropItemNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DropItemName, sizeof(CHAR) * m_DropItemNameSize);
	}
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGPickDropItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGPickDropItemHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}