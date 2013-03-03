

#include "stdafx.h"
#include "CGAskSubTrainSetting.h"



BOOL CGAskSubTrainSetting::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskSubTrainSetting::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGAskSubTrainSetting::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskSubTrainSettingHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
