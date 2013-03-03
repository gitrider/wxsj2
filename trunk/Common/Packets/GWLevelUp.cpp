/********************************************************************************
 *	�ļ�����	GWLevelUp.cpp
 *	ȫ·����	d:\Prj\Common\Packets\GWLevelUp.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 9 ��	14:31
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include "GWLevelUp.h"

using namespace Packets;

BOOL GWLevelUp::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	iStream.Read( (CHAR*)(&m_Level), sizeof(INT) );
	iStream.Read( (CHAR*)(&m_m_Exp), sizeof(UINT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWLevelUp::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	oStream.Write( (CHAR*)(&m_Level), sizeof(INT) );
	oStream.Write( (CHAR*)(&m_m_Exp), sizeof(UINT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWLevelUp::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWLevelUpHandler::Execute( this, pPlayer );
	
__LEAVE_FUNCTION

	return FALSE;
}