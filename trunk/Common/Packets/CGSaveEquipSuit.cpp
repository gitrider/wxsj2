#include "stdafx.h"
#include "CGSaveEquipSuit.h"


using namespace Packets;


BOOL CGSaveEquipSuit::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_SuitNum), sizeof(BYTE));	
	iStream.Read( (CHAR*)(&m_SuitSetting), sizeof(_SUIT_SETTING));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGSaveEquipSuit::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_SuitNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_SuitSetting), sizeof(_SUIT_SETTING));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGSaveEquipSuit::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGSaveEquipSuitHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}