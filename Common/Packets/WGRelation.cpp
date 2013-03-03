

#include "stdafx.h"
#include "WGRelation.h"



BOOL WGRelation::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Relation.Read( iStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGRelation::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Relation.Write( oStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGRelation::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGRelationHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


