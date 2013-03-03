

#include "stdafx.h"
#include "GCRetSetting.h"



BOOL GCRetSetting::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)m_aSetting, sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER );
	iStream.Read( (CHAR*)m_SuitSetting, sizeof(_SUIT_SETTING)*MAX_SUIT_SETTING );
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRetSetting::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)m_aSetting, sizeof(_OWN_SETTING)*SETTING_TYPE_NUMBER );
	oStream.Write( (CHAR*)m_SuitSetting, sizeof(_SUIT_SETTING)*MAX_SUIT_SETTING );
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRetSetting::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCRetSettingHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


