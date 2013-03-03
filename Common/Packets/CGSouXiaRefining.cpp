#include "stdafx.h"
#include "CGSouXiaRefining.h"

CGSouXiaRefining::CGSouXiaRefining(VOID)
{
}

CGSouXiaRefining::~CGSouXiaRefining(VOID)
{
}

BOOL Packets::CGSouXiaRefining::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nRefiningSouXia1), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nRefiningSouXia2), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nRefiningItem),	sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Packets::CGSouXiaRefining::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nRefiningSouXia1), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nRefiningSouXia2), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nRefiningItem), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT Packets::CGSouXiaRefining::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGSouXiaRefiningHandler::Execute( this, pPlayer );

	__LEAVE_FUNCTION
		return FALSE ;
}


