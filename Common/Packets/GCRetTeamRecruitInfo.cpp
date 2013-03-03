
#include "stdafx.h"
#include "GCRetTeamRecruitInfo.h"


BOOL	GCRetTeamRecruitInfo::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

		for( INT i=0; i<m_nMemberCount; ++i )
		{
			m_MemberInfo[i].Read(iStream);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	GCRetTeamRecruitInfo::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
	oStream.Write( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_MemberInfo[i].Write(oStream);
	}
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT GCRetTeamRecruitInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCRetTeamRecruitInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
