/********************************************************************************
 *	�ļ�����	CGReturnTeamFollow.cpp
 *	ȫ·����	d:\Prj\Common\Packets\CGReturnTeamFollow.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	17:46
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include "CGReturnTeamFollow.h"


BOOL	CGReturnTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_uRet, sizeof(UCHAR) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGReturnTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_uRet, sizeof(UCHAR) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGReturnTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGReturnTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

