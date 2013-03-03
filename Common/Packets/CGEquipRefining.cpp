#include "stdafx.h"
#include "CGEquipRefining.h"

CGEquipRefining::CGEquipRefining(VOID)
{
}

CGEquipRefining::~CGEquipRefining(VOID)
{
}

BOOL Packets::CGEquipRefining::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nEquipBagPos), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_nRefiningItemPos), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::CGEquipRefining::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nEquipBagPos), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nRefiningItemPos), sizeof(BYTE) ) ;

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT Packets::CGEquipRefining::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGEquipRefiningHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
		return FALSE ;
}


