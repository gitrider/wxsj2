#include "stdafx.h"
#include "GCBusRemoveAllPassenger.h"

BOOL GCBusRemoveAllPassenger::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBusRemoveAllPassenger::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBusRemoveAllPassenger::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCBusRemoveAllPassengerHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

