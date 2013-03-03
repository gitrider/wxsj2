#include "stdafx.h"
#include "CGSouXiaDiscard.h"

BOOL		CGSouXiaDiscard::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_BagIndex,sizeof(BYTE));
		iStream.Read((CHAR*)&m_Opt,sizeof(BYTE));
		iStream.Read((CHAR*)&m_UniqueNPCId,sizeof(UINT));

		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGSouXiaDiscard::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_BagIndex,sizeof(BYTE));
		oStream.Write((CHAR*)&m_Opt,sizeof(BYTE));
		oStream.Write((CHAR*)&m_UniqueNPCId,sizeof(UINT));


		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT	CGSouXiaDiscard::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGSouXiaDiscardHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}