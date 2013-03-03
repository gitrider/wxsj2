/********************************************************************************
 *	�ļ�����	CGRegisterTeamRecruit.cpp
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
 *
 *	����˵����	�ӳ��������ն�Ա����Ϣ
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include  "CGRegisterTeamRecruit.h"

BOOL CGRegisterTeamRecruit::Read(SocketInputStream& iStream )
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

BOOL CGRegisterTeamRecruit::Write(SocketOutputStream& oStream )	const
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

UINT CGRegisterTeamRecruit::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGRegisterTeamRecruitHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
