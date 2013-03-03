#include "stdafx.h"
#include "CGEquipRemould.h"


BOOL	CGEquipRemould::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_EquipPosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos,sizeof(BYTE));
	iStream.Read((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)&m_ValidNum,sizeof(BYTE));
	iStream.Read((CHAR*)&m_MaterialInfo,sizeof(EQUIPREMOULD_MATERIAL_INFO)*m_ValidNum);
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipRemould::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_EquipPosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos,sizeof(BYTE));
	oStream.Write((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)&m_ValidNum,sizeof(BYTE));
	oStream.Write((CHAR*)&m_MaterialInfo,sizeof(EQUIPREMOULD_MATERIAL_INFO)*m_ValidNum);
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipRemould::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipRemouldHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}