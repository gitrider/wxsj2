#include "stdafx.h"
#include "GCNotifyEquip.h"


using namespace Packets;


BOOL GCNotifyEquip::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(WORD));
	m_Item.ReadItemVarAttr(iStream);
    iStream.Read( (CHAR*)&m_OpType, sizeof(INT) );

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNotifyEquip::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(WORD));
	m_Item.WriteItemVarAttr(oStream);
    oStream.Write( (CHAR*)&m_OpType, sizeof(INT) );
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNotifyEquip::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCNotifyEquipHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}