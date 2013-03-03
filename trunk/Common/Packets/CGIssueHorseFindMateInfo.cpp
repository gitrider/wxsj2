/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/


#include "stdafx.h"
#include "CGIssueHorseFindMateInfo.h"

namespace Packets
{

BOOL CGIssueHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)(&m_szLeaveWords), sizeof(m_szLeaveWords) );
	
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGIssueHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)(&m_szLeaveWords), sizeof(m_szLeaveWords));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGIssueHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGIssueHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
