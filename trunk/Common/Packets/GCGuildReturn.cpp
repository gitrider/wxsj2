/********************************************************************************
 *	�ļ�����	GCGuildReturn.cpp
 *	ȫ·����	d:\Prj\Common\Packets\GCGuildReturn.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	17:35
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GCGuildReturn.h"

BOOL	GCGuildReturn::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	m_GuildReturn.Read(iStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GCGuildReturn::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildReturn.Write(oStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCGuildReturn::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildReturnHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
