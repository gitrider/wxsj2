
#include "stdafx.h"
#include "GCNotifyGoodBad.h"

BOOL GCNotifyGoodBad::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_uMode), sizeof(m_uMode));
	iStream.Read((CHAR*)(&m_nValue), sizeof(m_nValue));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCNotifyGoodBad::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_uMode), sizeof(m_uMode));
	oStream.Write((CHAR*)(&m_nValue), sizeof(m_nValue));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCNotifyGoodBad::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNotifyGoodBadHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
