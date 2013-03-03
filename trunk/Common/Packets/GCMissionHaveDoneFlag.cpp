// GCMissionHaveDoneFlag.cpp
// 
/////////////////////////////////////////////////////
#include "stdafx.h"
#include "GCMissionHaveDoneFlag.h"

using namespace Packets;

BOOL GCMissionHaveDoneFlag::Read(SocketInputStream& iStream) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nMissionID), sizeof(m_nMissionID));
	iStream.Read((CHAR*)(&m_bHaveDown), sizeof(m_bHaveDown));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCMissionHaveDoneFlag::Write(SocketOutputStream& oStream)const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nMissionID), sizeof(m_nMissionID));
	oStream.Write((CHAR*)(&m_bHaveDown), sizeof(m_bHaveDown));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCMissionHaveDoneFlag::Execute(Player* pPlayer)
{
__ENTER_FUNCTION

	return GCMissionHaveDoneFlagHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

