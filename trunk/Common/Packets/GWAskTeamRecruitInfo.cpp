/********************************************************************************
 *	文件名：	GWAskTeamRecruitInfo.h
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 5 月 14 日	17:57
 *
 *	功能说明：	请求得到组队征兵信息
 *	修改记录：
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
