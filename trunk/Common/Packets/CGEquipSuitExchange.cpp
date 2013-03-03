#include "stdafx.h"
#include "CGEquipSuitExchange.h"


using namespace Packets;


BOOL CGEquipSuitExchange::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_EquipSuitNum), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGEquipSuitExchange::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_EquipSuitNum), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGEquipSuitExchange::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGEquipSuitExchangeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}