#include "stdafx.h" 
#include "WLAskPrize.h"

BOOL WLAskPrize::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_PrizeUserID,sizeof(ID_t));
	iStream.Read((CHAR*)&m_AccName,sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)&m_CharName,sizeof(CHAR)*MAX_CHARACTER_NAME);
	iStream.Read((CHAR*)&m_Host,sizeof(CHAR)*IP_SIZE);
	iStream.Read((CHAR*)&m_GUID,sizeof(GUID_t));
	iStream.Read((CHAR*)&m_askType, sizeof(BYTE));
	if(ASK_BUY == m_askType)
	{
		iStream.Read((CHAR*)&m_GoodsType, sizeof(UINT));
		iStream.Read((CHAR*)&m_GoodsNumber, sizeof(USHORT));
		iStream.Read((CHAR*)&m_CostPoint, sizeof(UINT));
	}
	else if(ASK_PRIZE == m_askType)
	{
		iStream.Read((CHAR*)&m_GoodsType, sizeof(UINT));
	}
	else if(ASK_NEWUSER_CARD == m_askType)
	{
		iStream.Read((CHAR*)m_NewUserCard, sizeof(CHAR)*MAX_NEWUSER_CARD_SIZE);
	}
	
	return TRUE ;

__LEAVE_FUNCTION
	return FALSE ;
}

BOOL WLAskPrize::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_PrizeUserID,sizeof(ID_t));
	oStream.Write((CHAR*)&m_AccName,sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)&m_CharName,sizeof(CHAR)*MAX_CHARACTER_NAME);
	oStream.Write((CHAR*)&m_Host,sizeof(CHAR)*IP_SIZE);
	oStream.Write((CHAR*)&m_GUID,sizeof(GUID_t));
	oStream.Write((CHAR*)&m_askType, sizeof(BYTE) );
	if(ASK_BUY == m_askType)
	{
		oStream.Write((CHAR*)&m_GoodsType, sizeof(UINT));
		oStream.Write((CHAR*)&m_GoodsNumber, sizeof(USHORT));
		oStream.Write((CHAR*)&m_CostPoint, sizeof(UINT));
	}
	else if(ASK_PRIZE == m_askType)
	{
		oStream.Write((CHAR*)&m_GoodsType, sizeof(UINT));
	}
	else if(ASK_NEWUSER_CARD == m_askType)
	{
		oStream.Write((CHAR*)m_NewUserCard, sizeof(CHAR)*MAX_NEWUSER_CARD_SIZE);
	}

	return TRUE ;

__LEAVE_FUNCTION
	return FALSE ;
}

UINT WLAskPrize::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return	WLAskPrizeHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION
	return FALSE ;
}


