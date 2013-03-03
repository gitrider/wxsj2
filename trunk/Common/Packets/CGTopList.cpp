/********************************************************************************
 *	�ļ�����	CGTopList.cpp

 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 4 �� 10 ��	
 *
 *	����˵����	�ͻ������������˷��͵��������а���Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "CGTopList.h"

BOOL CGTopList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGTopList::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGTopList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGTopListHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
