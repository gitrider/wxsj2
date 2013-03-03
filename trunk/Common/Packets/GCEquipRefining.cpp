#include "stdafx.h"
#include "GCEquipRefining.h"

GCEquipRefining::GCEquipRefining(VOID)
{
}

GCEquipRefining::~GCEquipRefining(VOID)
{
}

BOOL Packets::GCEquipRefining::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nResult), sizeof(BYTE) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::GCEquipRefining::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nResult), sizeof(BYTE) ) ;

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT Packets::GCEquipRefining::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCEquipRefiningHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
		return FALSE ;
}


