/********************************************************************************
 *	�ļ�����	GWEnterTeamFollow.cpp
 *	ȫ·����	d:\Prj\Common\Packets\GWEnterTeamFollow.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	15:23
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include  "GWEnterTeamFollow.h"


BOOL	GWEnterTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GWEnterTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GWEnterTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWEnterTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

