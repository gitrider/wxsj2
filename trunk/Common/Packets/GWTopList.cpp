/********************************************************************************
 *	�ļ�����	GWTopList.h
 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 4 �� 10 ��
 *
 *	����˵����	���������� World ���͵����а���صĲ�������
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GWTopList.h"

BOOL GWTopList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
	iStream.Read((CHAR*)&m_Type, sizeof(m_Type));
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWTopList::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
	oStream.Write((CHAR*)&m_Type, sizeof(m_Type));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWTopList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTopListHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
