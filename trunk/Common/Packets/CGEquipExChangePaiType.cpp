#include "stdafx.h"
#include "CGEquipExChangePaiType.h"


BOOL	CGEquipExChangePaiType::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_Equip1PosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos1,sizeof(BYTE));
	iStream.Read((CHAR*)&m_Item1Id,sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)&m_Equip2PosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos2,sizeof(BYTE));
	iStream.Read((CHAR*)&m_Item2Id,sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)&m_nGemBagPos,sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipExChangePaiType::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_Equip1PosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos1,sizeof(BYTE));
	oStream.Write((CHAR*)&m_Item1Id,sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)&m_Equip2PosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos2,sizeof(BYTE));
	oStream.Write((CHAR*)&m_Item2Id,sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)&m_nGemBagPos,sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipExChangePaiType::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipExChangePaiTypeHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}