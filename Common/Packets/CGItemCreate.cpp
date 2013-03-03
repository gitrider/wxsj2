#include "stdafx.h"
#include "CGItemCreate.h"


BOOL	CGItemCreate::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	iStream.Read((CHAR*)&m_nItemIndex,sizeof(UINT));
	iStream.Read((CHAR*)&m_ValidNum,sizeof(BYTE));
	iStream.Read((CHAR*)m_nMaterial, m_ValidNum*sizeof(CREATEITEM_MATERIAL_INFO));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGItemCreate::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_nItemIndex,sizeof(UINT));
	oStream.Write((CHAR*)&m_ValidNum,sizeof(BYTE));
	oStream.Write((CHAR*)m_nMaterial, m_ValidNum*sizeof(CREATEITEM_MATERIAL_INFO));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGItemCreate::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGItemCreateHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}