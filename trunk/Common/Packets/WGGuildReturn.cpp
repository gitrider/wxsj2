/********************************************************************************
 *	�ļ�����	WGGuildReturn.cpp
 *	ȫ·����	d:\Prj\Common\Packets\WGGuildReturn.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	12:11
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGGuildReturn.h"

BOOL	WGGuildReturn::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Read(iStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildReturn::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Write(oStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildReturn::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildReturnHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
