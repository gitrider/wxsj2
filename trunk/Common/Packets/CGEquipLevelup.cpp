#include "stdafx.h"
#include "CGEquipLevelup.h"


BOOL	CGEquipLevelup::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_EquipPosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nGemNum,sizeof(BYTE));
	iStream.Read((CHAR*)&m_GemId,sizeof(UINT));
	iStream.Read((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipLevelup::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_EquipPosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nGemNum,sizeof(BYTE));
	oStream.Write((CHAR*)&m_GemId,sizeof(UINT));
	oStream.Write((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipLevelup::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipLevelupHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
