#include "stdafx.h"
#include "GCExchangeSynchLockItemII.h"

BOOL GCExchangeSynchLockItemII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));

	if( m_Type == EXCHANGE_MSG::POS_BAG )
	{
		iStream.Read( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_bLock), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_Index), sizeof(BYTE));
	}
	else if( m_Type == EXCHANGE_MSG::POS_PET )
	{
		iStream.Read( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_bLock), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_Index), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if( m_Type == EXCHANGE_MSG::POS_FOLLOWPET )
	{
		iStream.Read( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_bLock), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_Index), sizeof(BYTE));

		// 待加 跟随宠物用到关键字读取...
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeSynchLockItemII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));

	if( m_Type == EXCHANGE_MSG::POS_BAG )
	{
		oStream.Write( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_bLock), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_Index), sizeof(BYTE));
	}
	else if( m_Type == EXCHANGE_MSG::POS_PET )
	{
		oStream.Write( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_bLock), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_Index), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if( m_Type == EXCHANGE_MSG::POS_FOLLOWPET )
	{
		oStream.Write( (CHAR*)(&m_IsMyself), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_bLock), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_Index), sizeof(BYTE));

		// 待加 跟随宠物用到关键字读取...
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeSynchLockItemII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeSynchLockItemIIHandler::Execute(  this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

