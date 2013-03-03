/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWHorseChangeState.h"

namespace Packets
{

BOOL GWHorseChangeState::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_iStateChangeType, sizeof(m_iStateChangeType) );
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWHorseChangeState::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_iStateChangeType, sizeof(m_iStateChangeType) );
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWHorseChangeState::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWHorseChangeStateHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
