/********************************************************************************
 *	文件名：	GWRegisterTeamRecruit.cpp
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 5 月 14 日	17:57
 *
 *	功能说明：	队长发布征收队员的消息
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include  "GWRegisterTeamRecruit.h"

BOOL GWRegisterTeamRecruit::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Type), sizeof(m_Type) ) ;
		iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;

		if ( m_Type == LEADER_RECRUIT )
		{
			m_LeaderRecruit.Read(iStream);
		}
		else if (m_Type == MEMBER_RECRUIT )
		{
			m_MemberRecruit.Read(iStream);;
		}
		else
		{
			Assert(FALSE);
		}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWRegisterTeamRecruit::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
	
		oStream.Write( (CHAR*)(&m_Type), sizeof(m_Type) ) ;	
		oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;	

		if ( m_Type == LEADER_RECRUIT )
		{
			m_LeaderRecruit.Write(oStream);
		}
		else if (m_Type == MEMBER_RECRUIT )
		{
			m_MemberRecruit.Write(oStream);;
		}
		else
		{
			Assert(FALSE);
		}
		
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

UINT GWRegisterTeamRecruit::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return GWRegisterTeamRecruitHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
