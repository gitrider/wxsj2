

#include "stdafx.h"
#include "GCRetSubTrainSetting.h"



BOOL GCRetSubTrainSetting::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_SubTrainSetting, sizeof(_SUBTRAIN_SETTING) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCRetSubTrainSetting::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_SubTrainSetting, sizeof(_SUBTRAIN_SETTING) );
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCRetSubTrainSetting::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCRetSubTrainSettingHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
