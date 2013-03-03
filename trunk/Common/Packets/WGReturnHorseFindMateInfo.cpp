/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "WGReturnHorseFindMateInfo.h"

namespace Packets
{

BOOL WGReturnHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_bSearch, sizeof(m_bSearch) );
	iStream.Read( (CHAR*)&m_bContinue, sizeof(m_bContinue) );
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	//iStream.Read( (CHAR*)&m_HorseFindMateResult, sizeof(m_HorseFindMateResult) );
	m_HorseFindMateResult.Read(iStream);
	iStream.Read( (CHAR*)&m_iReturnPostion, sizeof(m_iReturnPostion) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL WGReturnHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_bSearch, sizeof(m_bSearch) );
	oStream.Write( (CHAR*)&m_bContinue, sizeof(m_bContinue) );
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	//oStream.Write( (CHAR*)&m_HorseFindMateResult, sizeof(m_HorseFindMateResult) );
	m_HorseFindMateResult.Write(oStream);
	oStream.Write( (CHAR*)&m_iReturnPostion, sizeof(m_iReturnPostion) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGReturnHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGReturnHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
