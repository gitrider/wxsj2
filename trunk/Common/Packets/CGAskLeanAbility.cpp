// CGAskLeanAbility.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGAskLeanAbility.h"

BOOL CGAskLeanAbility::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	iStream.Read( (CHAR*)(&m_nNpcID), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskLeanAbility::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	oStream.Write( (CHAR*)(&m_nNpcID),   sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskLeanAbility::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskLeanAbilityHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

