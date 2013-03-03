#include "stdafx.h"
#include "GCNewBus_Move.h"

BOOL GCNewBus_Move::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_nDataID), sizeof(m_nDataID));
	iStream.Read((CHAR*)(&m_posWorld), sizeof(m_posWorld));
	iStream.Read((CHAR*)(&m_fDir), sizeof(m_fDir));
	iStream.Read((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read((CHAR*)(&m_fTargetHeight), sizeof(m_fTargetHeight));
	iStream.Read((CHAR*)(&m_nPassengerCount), sizeof(m_nPassengerCount));
	if( m_nPassengerCount > 0 && m_nPassengerCount <= DEF_BUS_MAX_PASSENGER_COUNT )
	{
		iStream.Read((CHAR*)(m_anPassengerIDs), sizeof(ObjID_t) * m_nPassengerCount);
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewBus_Move::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_nDataID), sizeof(m_nDataID));
	oStream.Write((CHAR*)(&m_posWorld), sizeof(m_posWorld));
	oStream.Write((CHAR*)(&m_fDir), sizeof(m_fDir));
	oStream.Write((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write((CHAR*)(&m_fTargetHeight), sizeof(m_fTargetHeight));
	oStream.Write((CHAR*)(&m_nPassengerCount), sizeof(m_nPassengerCount));
	if( m_nPassengerCount > 0 && m_nPassengerCount <= DEF_BUS_MAX_PASSENGER_COUNT )
	{
		oStream.Write((CHAR*)(m_anPassengerIDs), sizeof(ObjID_t) * m_nPassengerCount);
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewBus_Move::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCNewBus_MoveHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

