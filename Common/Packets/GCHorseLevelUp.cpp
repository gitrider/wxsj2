#include "stdafx.h"
#include "GCHorseLevelUp.h"

using namespace Packets;


BOOL GCHorseLevelUp::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_OnwerObjID), sizeof(ObjID_t));
	iStream.Read((CHAR*)(&m_HorseLevel),sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCHorseLevelUp::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_OnwerObjID), sizeof(ObjID_t));
	oStream.Write((CHAR*)(&m_HorseLevel),sizeof(UINT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCHorseLevelUp::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCHorseLevelUpHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}