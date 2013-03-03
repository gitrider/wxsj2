#include "stdafx.h"
#include "GCEquipSuitExchangeResult.h"


BOOL GCEquipSuitExchangeResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_ExchageEquipInfo), sizeof(EXCHANGE_EQUIP_INFO)*HEQUIP_NUMBER);

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCEquipSuitExchangeResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_ExchageEquipInfo), sizeof(EXCHANGE_EQUIP_INFO)*HEQUIP_NUMBER);

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCEquipSuitExchangeResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCEquipSuitExchangeResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}