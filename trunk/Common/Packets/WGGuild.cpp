/********************************************************************************
 *	�ļ�����	WGGuild.cpp
 *	ȫ·����	d:\Prj\Common\Packets\WGGuild.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 14 ��	11:58
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGGuild.h"

BOOL WGGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL WGGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
