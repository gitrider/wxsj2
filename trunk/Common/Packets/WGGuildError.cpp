/********************************************************************************
 *	�ļ�����	WGGuildError.cpp
 *	ȫ·����	d:\Prj\Common\Packets\WGGuildError.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	9:22
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "WGGuildError.h"

BOOL	WGGuildError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildError::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildErrorHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
