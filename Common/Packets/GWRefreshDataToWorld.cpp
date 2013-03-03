

#include "stdafx.h"
#include "GWRefreshDataToWorld.h"



BOOL GWRefreshDataToWorld::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags) );
	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	if(m_Flags.isSetBit(REFRESH_COUNTRY))
		iStream.Read( (CHAR*)(&m_iCountry), sizeof(m_iCountry));
	if(m_Flags.isSetBit(REFRESH_MENPAI))
		iStream.Read( (CHAR*)(&m_iMenPai), sizeof(m_iMenPai));
	if(m_Flags.isSetBit(REFRESH_LEVEL))
		iStream.Read( (CHAR*)(&m_iLevel), sizeof(m_iLevel));
	if(m_Flags.isSetBit(REFRESH_GUILD))
		iStream.Read( (CHAR*)(&m_GuildID), sizeof(m_GuildID));
	if(m_Flags.isSetBit(REFRESH_POSTCODE))
		iStream.Read( (CHAR*)(&m_iPostCode), sizeof(m_iPostCode));
	if(m_Flags.isSetBit(REFRESH_MONEY))
		iStream.Read( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if(m_Flags.isSetBit(REFRESH_BANK_MONEY))
		iStream.Read( (CHAR*)(&m_uBankMoney), sizeof(m_uBankMoney));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWRefreshDataToWorld::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags) );
	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );

	if(m_Flags.isSetBit(REFRESH_COUNTRY))
		oStream.Write( (CHAR*)(&m_iCountry), sizeof(m_iCountry));
	if(m_Flags.isSetBit(REFRESH_MENPAI))
		oStream.Write( (CHAR*)(&m_iMenPai), sizeof(m_iMenPai));
	if(m_Flags.isSetBit(REFRESH_LEVEL))
		oStream.Write( (CHAR*)(&m_iLevel), sizeof(m_iLevel));
	if(m_Flags.isSetBit(REFRESH_GUILD))
		oStream.Write( (CHAR*)(&m_GuildID), sizeof(m_GuildID));
	if(m_Flags.isSetBit(REFRESH_POSTCODE))
		oStream.Write( (CHAR*)(&m_iPostCode), sizeof(m_iPostCode));
	if(m_Flags.isSetBit(REFRESH_MONEY))
		oStream.Write( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if(m_Flags.isSetBit(REFRESH_BANK_MONEY))
		oStream.Write( (CHAR*)(&m_uBankMoney), sizeof(m_uBankMoney));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWRefreshDataToWorld::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWRefreshDataToWorldHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


