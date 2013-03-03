/********************************************************************************
 *	�ļ�����	GWAskTeamRecruitInfo.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 5 �� 14 ��	17:57
 *
 *	����˵����	����õ����������Ϣ
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include  "GWAskTeamRecruitInfo.h"

BOOL GWAskTeamRecruitInfo::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Type), sizeof(m_Type) ) ;
		iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWAskTeamRecruitInfo::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Type), sizeof(m_Type) ) ;	
		oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;	

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

UINT GWAskTeamRecruitInfo::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return GWAskTeamRecruitInfoHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
