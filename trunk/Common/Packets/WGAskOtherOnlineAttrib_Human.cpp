// WGAskOtherOnlineAttrib_Human.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "WGAskOtherOnlineAttrib_Human.h"

namespace Packets
{

BOOL WGAskOtherOnlineAttrib_Human::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SourcePlayerID), sizeof(m_SourcePlayerID) ) ;
	iStream.Read( (CHAR*)(&m_DestPlayerID), sizeof(m_DestPlayerID) ) ;
	iStream.Read( (CHAR*)(&m_Guid), sizeof(m_Guid) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGAskOtherOnlineAttrib_Human::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SourcePlayerID), sizeof(m_SourcePlayerID) ) ;
	oStream.Write( (CHAR*)(&m_DestPlayerID), sizeof(m_DestPlayerID) ) ;
	oStream.Write( (CHAR*)(&m_Guid), sizeof(m_Guid) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGAskOtherOnlineAttrib_Human::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGAskOtherOnlineAttrib_HumanHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


}


