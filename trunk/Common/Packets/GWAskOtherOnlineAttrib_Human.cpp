// GWAskOtherOnlineAttrib_Human.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GWAskOtherOnlineAttrib_Human.h"

namespace Packets
{

BOOL GWAskOtherOnlineAttrib_Human::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	iStream.Read( (CHAR*)(&m_SourceGuid), sizeof(m_SourceGuid) ) ;
	iStream.Read( (CHAR*)(&m_DestGuid), sizeof(m_DestGuid) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWAskOtherOnlineAttrib_Human::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	oStream.Write( (CHAR*)(&m_SourceGuid), sizeof(m_SourceGuid) ) ;
	oStream.Write( (CHAR*)(&m_DestGuid), sizeof(m_DestGuid) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWAskOtherOnlineAttrib_Human::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWAskOtherOnlineAttrib_HumanHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


}


