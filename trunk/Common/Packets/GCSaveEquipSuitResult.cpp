#include "stdafx.h"
#include "GCSaveEquipSuitResult.h"


BOOL GCSaveEquipSuitResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_SuitNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_nSuitSetting.m_SuitName), sizeof(CHAR)*MAX_SUIT_NAME_SIZE);
	iStream.Read( (CHAR*)(m_nSuitSetting.m_EquipData), sizeof(_ITEM_GUID)*HEQUIP_NUMBER);
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCSaveEquipSuitResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_SuitNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_nSuitSetting.m_SuitName), sizeof(CHAR)*MAX_SUIT_NAME_SIZE);
	oStream.Write( (CHAR*)(m_nSuitSetting.m_EquipData), sizeof(_ITEM_GUID)*HEQUIP_NUMBER);
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCSaveEquipSuitResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCSaveEquipSuitResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}