#include "stdafx.h"
#include "GCBusStopMove.h"

BOOL GCBusStopMove::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_posWorld), sizeof(m_posWorld));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBusStopMove::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_posWorld), sizeof(m_posWorld));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBusStopMove::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCBusStopMoveHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

