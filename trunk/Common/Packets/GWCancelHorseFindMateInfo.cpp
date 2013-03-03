/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWCancelHorseFindMateInfo.h"

namespace Packets
{

BOOL GWCancelHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWCancelHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWCancelHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCancelHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}

