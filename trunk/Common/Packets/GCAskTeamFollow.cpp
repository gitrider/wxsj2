/********************************************************************************
 *	�ļ�����	GCAskTeamFollow.cpp
 *	ȫ·����	d:\Prj\Common\Packets\GCAskTeamFollow.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	16:10
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include "GCAskTeamFollow.h"


BOOL	GCAskTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCAskTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCAskTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAskTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

