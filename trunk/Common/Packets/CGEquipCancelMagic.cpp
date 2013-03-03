#include "stdafx.h"
#include "CGEquipCancelMagic.h"

BOOL	CGEquipCancelMagic::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_nEquipPos,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nGemPos,sizeof(BYTE));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGEquipCancelMagic::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_nEquipPos,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nGemPos,sizeof(BYTE));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGEquipCancelMagic::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipCancelMagicHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
