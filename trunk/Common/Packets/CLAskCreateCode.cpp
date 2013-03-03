#include "stdafx.h" 
#include "CLAskCreateCode.h"

BOOL CLAskCreateCode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_AskType), sizeof(BYTE));
	if(CREATECODE_SEND==m_AskType)
	{
		iStream.Read((CHAR*)(&m_Answer), sizeof(CreateCode));
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLAskCreateCode::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_AskType), sizeof(BYTE));
	if(CREATECODE_SEND==m_AskType)
	{
		oStream.Write((CHAR*)(&m_Answer), sizeof(CreateCode));
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLAskCreateCode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLAskCreateCodeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}
