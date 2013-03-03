#include "stdafx.h"
#include "CGEquipAddHole.h"


using namespace Packets;


BOOL CGEquipAddHole::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_EquipFromType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_EquipFromPos), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGEquipAddHole::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_EquipFromType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_EquipFromPos), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGEquipAddHole::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGEquipAddHoleHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}