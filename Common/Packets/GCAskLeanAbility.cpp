// GCAskLeanAbility.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCAskLeanAbility.h"

BOOL GCAskLeanAbility::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	iStream.Read( (CHAR*)(&m_nResult), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAskLeanAbility::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	oStream.Write( (CHAR*)(&m_nResult), sizeof(INT));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAskLeanAbility::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAskLeanAbilityHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

