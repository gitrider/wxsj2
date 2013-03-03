

#include "CGDisclosureToGM.h"



BOOL CGDisclosureToGM::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		m_disMsg.Read( iStream ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGDisclosureToGM::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		m_disMsg.Write( oStream ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGDisclosureToGM::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGDisclosureToGMHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}