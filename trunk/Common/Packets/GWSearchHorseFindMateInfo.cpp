/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#include "stdafx.h"
#include "GWSearchHorseFindMateInfo.h"

namespace Packets
{

BOOL GWSearchHorseFindMateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_szHorseName), sizeof(m_szHorseName) );
	iStream.Read( (CHAR*)(&m_iHorseLevel), sizeof(m_iHorseLevel));
	iStream.Read( (CHAR*)(&m_iHorseSex), sizeof(m_iHorseSex));
	iStream.Read( (CHAR*)(&m_iHorseGrowRate), sizeof(m_iHorseGrowRate));
	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID));
	iStream.Read( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion));

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWSearchHorseFindMateInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_szHorseName), sizeof(m_szHorseName) );
	oStream.Write( (CHAR*)(&m_iHorseLevel), sizeof(m_iHorseLevel));
	oStream.Write( (CHAR*)(&m_iHorseSex), sizeof(m_iHorseSex));
	oStream.Write( (CHAR*)(&m_iHorseGrowRate), sizeof(m_iHorseGrowRate));
	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID));
	oStream.Write( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWSearchHorseFindMateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWSearchHorseFindMateInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

}
