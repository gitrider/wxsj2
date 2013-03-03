#include "stdafx.h"
#include "CGEquipAdsorbMagic.h"

BOOL	CGEquipAdsorbMagic::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_Money,sizeof(INT));
	iStream.Read((CHAR*)&m_EquipPosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nEquipPos,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nBeadPos,sizeof(BYTE));
	//iStream.Read((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	//iStream.Read((CHAR*)&m_BeadId,sizeof(_ITEM_GUID));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipAdsorbMagic::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_Money,sizeof(INT));
	oStream.Write((CHAR*)&m_EquipPosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nEquipPos,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nBeadPos,sizeof(BYTE));
	//oStream.Write((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	//oStream.Write((CHAR*)&m_BeadId,sizeof(_ITEM_GUID));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipAdsorbMagic::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipAdsorbMagicHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
