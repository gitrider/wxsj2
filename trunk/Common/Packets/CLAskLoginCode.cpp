#include "stdafx.h" 
#include "CLAskLoginCode.h"

BOOL CLAskLoginCode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_AskType), sizeof(BYTE));
	if(LOGINCODE_SEND==m_AskType)
	{
		iStream.Read((CHAR*)(&m_Answer), sizeof(CreateCode));
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLAskLoginCode::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_AskType), sizeof(BYTE));
	if(LOGINCODE_SEND==m_AskType)
	{
		oStream.Write((CHAR*)(&m_Answer), sizeof(CreateCode));
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CLAskLoginCode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLAskLoginCodeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}
