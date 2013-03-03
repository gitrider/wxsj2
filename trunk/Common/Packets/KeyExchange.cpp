// KeyExchange.cpp
// 
/////////////////////////////////////////////////////
#include "stdafx.h" 
#include "KeyExchange.h"

namespace Packets
{
	BOOL KeyExchange::Read(SocketInputStream& iStream) 
	{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)(&m_nKeySize), sizeof(m_nKeySize));
		Assert(0<=m_nKeySize&&m_nKeySize<=MAX_BUFFER_SIZE);
		iStream.Read((CHAR*)(&m_szKey), sizeof(UCHAR)*m_nKeySize);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL KeyExchange::Write(SocketOutputStream& oStream)const
	{
	__ENTER_FUNCTION
		Assert(0<=m_nKeySize);
		oStream.Write((CHAR*)(&m_nKeySize), sizeof(m_nKeySize));
		oStream.Write((CHAR*)(&m_szKey), sizeof(UCHAR)*m_nKeySize);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT KeyExchange::Execute(Player* pPlayer)
	{
	__ENTER_FUNCTION

		return KeyExchangeHandler::Execute(this, pPlayer);

	__LEAVE_FUNCTION

		return FALSE ;
	}
	
	VOID KeyExchange::SetKey(UCHAR const* szKey, INT nKeySize)
	{
	__ENTER_FUNCTION
		Assert(MAX_BUFFER_SIZE>=nKeySize && 0<=nKeySize);
		m_nKeySize = nKeySize;
		memcpy(m_szKey, szKey, nKeySize);
	__LEAVE_FUNCTION
	}
}
