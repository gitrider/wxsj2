/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "CGQueryHorseFindMateInfo.h"

namespace Packets
{

BOOL CGQueryHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_iMaxPostion, sizeof(m_iMaxPostion) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGQueryHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_iMaxPostion, sizeof(m_iMaxPostion) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGQueryHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGQueryHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
