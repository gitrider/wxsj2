#include "stdafx.h"
#include "WGRetTeamRecruitInfo.h"

BOOL	WGRetTeamRecruitInfo::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_MemberInfo[i].Read(iStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGRetTeamRecruitInfo::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_MemberInfo[i].Write(oStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT	WGRetTeamRecruitInfo::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGRetTeamRecruitInfoHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

