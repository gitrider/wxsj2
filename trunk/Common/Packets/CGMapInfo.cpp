/********************************************************************************
 *	�ļ�����	CGMapInfo.cpp

 *	�����ˣ�	�����
 *	����ʱ�䣺	2008 �� 4 �� 16 ��	
 *
 *	����˵����	�ͻ������������˷��͵������ͼ(������ת)��Ϣ
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "CGMapInfo.h"

BOOL CGMapInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Index, sizeof(m_Index) );	

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGMapInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Index, sizeof(m_Index) );	

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGMapInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMapInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
