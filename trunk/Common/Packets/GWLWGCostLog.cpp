#include "stdafx.h" 
#include "GWLWGCostLog.h"

BOOL GWLWGCostLog::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
		
	iStream.Read((CHAR*)&m_nType,sizeof(BYTE));
	if(GW_COST_LOG == m_nType)
	{
		iStream.Read((CHAR*)&m_LogInfo,sizeof(_COST_LOG));
	}
	else if(WL_COST_LOG == m_nType)
	{
		iStream.Read((CHAR*)&m_LogPos,sizeof(ID_t));
		iStream.Read((CHAR*)&m_LogInfo,sizeof(_COST_LOG));
	}
	else if(LW_COST_LOG == m_nType)
	{
		iStream.Read((CHAR*)&m_LogPos,sizeof(ID_t));
		iStream.Read((CHAR*)&m_nResult,sizeof(BYTE));
		iStream.Read((CHAR*)m_SerialKey,MAX_PRIZE_SERIAL_LENGTH);
	}
		
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL GWLWGCostLog::Write( SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION
	oStream.Write((CHAR*)&m_nType,sizeof(BYTE));
	if(GW_COST_LOG == m_nType)
	{
		oStream.Write((CHAR*)&m_LogInfo,sizeof(_COST_LOG));
	}
	else if(WL_COST_LOG == m_nType)
	{
		oStream.Write((CHAR*)&m_LogPos,sizeof(ID_t));
		oStream.Write((CHAR*)&m_LogInfo,sizeof(_COST_LOG));
	}
	else if(LW_COST_LOG == m_nType)
	{
		oStream.Write((CHAR*)&m_LogPos,sizeof(ID_t));
		oStream.Write((CHAR*)&m_nResult,sizeof(BYTE));
		oStream.Write((CHAR*)m_SerialKey,MAX_PRIZE_SERIAL_LENGTH);
	}

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

UINT GWLWGCostLog::Execute( Player* pPlayer )
{
__ENTER_FUNCTION
	return GWLWGCostLogHandler::Execute( this, pPlayer );
__LEAVE_FUNCTION
	return FALSE;
}
