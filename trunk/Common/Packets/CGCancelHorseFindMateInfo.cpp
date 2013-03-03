/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "CGCancelHorseFindMateInfo.h"

namespace Packets
{

BOOL CGCancelHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGCancelHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGCancelHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCancelHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
