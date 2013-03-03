/********************************************************************************
 *	�ļ�����	GCGuildError.cpp
 *	ȫ·����	d:\Prj\Common\Packets\GCGuildError.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	17:37
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GCGuildError.h"

BOOL	GCGuildError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GCGuildError::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCGuildError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildErrorHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
