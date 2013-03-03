/********************************************************************************
 *	�����ˣ�	���ı�
 *	����ʱ�䣺	2008 �� 5 �� 20 ��	
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "GCReturnHorseFindMateInfo.h"

namespace Packets
{

BOOL GCReturnHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_bSearch, sizeof(m_bSearch) );
	iStream.Read( (CHAR*)&m_bContinue, sizeof(m_bContinue) );
	//iStream.Read( (CHAR*)&m_HorseFindMateResult, sizeof(m_HorseFindMateResult) );
	m_HorseFindMateResult.Read(iStream);
	iStream.Read( (CHAR*)&m_iReturnPostion, sizeof(m_iReturnPostion) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GCReturnHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_bSearch, sizeof(m_bSearch) );
	oStream.Write( (CHAR*)&m_bContinue, sizeof(m_bContinue) );
	//oStream.Write( (CHAR*)&m_HorseFindMateResult, sizeof(m_HorseFindMateResult) );
	m_HorseFindMateResult.Write(oStream);
	oStream.Write( (CHAR*)&m_iReturnPostion, sizeof(m_iReturnPostion) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCReturnHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCReturnHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
