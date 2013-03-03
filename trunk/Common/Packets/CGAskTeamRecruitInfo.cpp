/********************************************************************************
 *	�ļ�����	CGAskTeamRecruitInfo.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
 *
 *	����˵����	����õ����������Ϣ
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include  "CGAskTeamRecruitInfo.h"

BOOL CGAskTeamRecruitInfo::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Type), sizeof(m_Type) ) ;
		iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL CGAskTeamRecruitInfo::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Type), sizeof(m_Type) ) ;	
		oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;	

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

UINT CGAskTeamRecruitInfo::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGAskTeamRecruitInfoHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
