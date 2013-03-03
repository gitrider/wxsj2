#include "stdafx.h"
#include "GCBusMove.h"

BOOL GCBusMove::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_posWorld), sizeof(m_posWorld));
	iStream.Read((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read((CHAR*)(&m_fTargetHeight), sizeof(m_fTargetHeight));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBusMove::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_posWorld), sizeof(m_posWorld));
	oStream.Write((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write((CHAR*)(&m_fTargetHeight), sizeof(m_fTargetHeight));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBusMove::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCBusMoveHandler::Execute(this, pPlayer) ;

__LEAVE_FUNCTION

	return FALSE ;
}

