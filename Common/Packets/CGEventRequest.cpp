// CGEventRequest.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGEventRequest.h"

using namespace Packets;

BOOL CGEventRequest::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	iStream.Read( (CHAR*)(&m_idScript), sizeof(ScriptID_t) ) ;
	iStream.Read( (CHAR*)(&m_idExIndex), sizeof(INT) ) ;
	iStream.Read( (CHAR*)(&m_idState), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGEventRequest::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;
	oStream.Write( (CHAR*)(&m_idExIndex), sizeof(m_idExIndex) ) ;
	oStream.Write( (CHAR*)(&m_idState), sizeof(m_idState) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGEventRequest::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGEventRequestHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

