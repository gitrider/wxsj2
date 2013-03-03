#include "stdafx.h"
#include "GCBusAddPassenger.h"

BOOL GCBusAddPassenger::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_nIndex), sizeof(m_nIndex));
	iStream.Read((CHAR*)(&m_nPassengerID), sizeof(m_nPassengerID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBusAddPassenger::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_nIndex), sizeof(m_nIndex));
	oStream.Write((CHAR*)(&m_nPassengerID), sizeof(m_nPassengerID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBusAddPassenger::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCBusAddPassengerHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

