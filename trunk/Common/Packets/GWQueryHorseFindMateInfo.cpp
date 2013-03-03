/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWQueryHorseFindMateInfo.h"

namespace Packets
{

BOOL GWQueryHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerGUID, sizeof(m_PlayerGUID) );
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)&m_iMaxPostion, sizeof(m_iMaxPostion) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWQueryHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_PlayerGUID, sizeof(m_PlayerGUID) );
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)&m_iMaxPostion, sizeof(m_iMaxPostion) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWQueryHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWQueryHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}

