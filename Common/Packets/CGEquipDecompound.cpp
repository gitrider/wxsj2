#include "stdafx.h"
#include "CGEquipDecompound.h"


BOOL	CGEquipDecompound::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)&m_EquipPosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos,sizeof(BYTE));
	iStream.Read((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipDecompound::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write((CHAR*)&m_EquipPosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos,sizeof(BYTE));
	oStream.Write((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipDecompound::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipDecompoundHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}