#include "stdafx.h"
#include "CGUseGem.h"


BOOL	CGUseGem::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_nValidCount,sizeof(BYTE));
	iStream.Read((CHAR*)&m_EquipPosType,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPos,sizeof(INT));
	iStream.Read((CHAR*)&m_HoleGemInfo,sizeof(HOLE_GEM_INFO)*m_nValidCount);
	iStream.Read((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGUseGem::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_nValidCount,sizeof(BYTE));
	oStream.Write((CHAR*)&m_EquipPosType,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPos,sizeof(INT));
	oStream.Write((CHAR*)&m_HoleGemInfo,sizeof(HOLE_GEM_INFO)*m_nValidCount);
	oStream.Write((CHAR*)&m_ItemId,sizeof(_ITEM_GUID));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGUseGem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGUseGemHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}


