/********************************************************************************
 *	�ļ�����	CGStopTeamFollow.cpp
 *	ȫ·����	d:\Prj\Common\Packets\CGStopTeamFollow.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	22:48
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include "CGStopTeamFollow.h"


BOOL	CGStopTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGStopTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGStopTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGStopTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

