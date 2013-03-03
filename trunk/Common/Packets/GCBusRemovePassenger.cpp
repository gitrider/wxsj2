#include "stdafx.h"
#include "GCBusRemovePassenger.h"

BOOL GCBusRemovePassenger::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_nPassengerID), sizeof(m_nPassengerID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBusRemovePassenger::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_nPassengerID), sizeof(m_nPassengerID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBusRemovePassenger::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCBusRemovePassengerHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

