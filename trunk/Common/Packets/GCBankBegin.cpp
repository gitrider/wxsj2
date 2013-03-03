// GCBankBegin.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCBankBegin.h"

BOOL GCBankBegin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_BankID), sizeof(BYTE));
    iStream.Read( (CHAR*)(&m_NPCID), sizeof(ObjID_t));
    iStream.Read( (CHAR*)(&m_bHavePwd), sizeof(BOOL));

    
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankBegin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BankID), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_NPCID), sizeof(ObjID_t));
    oStream.Write( (CHAR*)(&m_bHavePwd), sizeof(BOOL));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankBegin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankBeginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

