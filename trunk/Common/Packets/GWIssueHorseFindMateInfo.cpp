/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "GWIssueHorseFindMateInfo.h"

namespace Packets
{

BOOL GWIssueHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_HorseFindMateInfo), sizeof(m_HorseFindMateInfo) );


	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWIssueHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_HorseFindMateInfo), sizeof(m_HorseFindMateInfo));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWIssueHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWIssueHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
