#include "stdafx.h"
#include "GCSouXiaDismantle.h"

GCSouXiaDismantle::GCSouXiaDismantle(VOID):m_nSouXiaRelicCount(0),m_nType(0)
{
}

GCSouXiaDismantle::~GCSouXiaDismantle(VOID)
{
}

BOOL GCSouXiaDismantle::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nSouXiaRelicCount),sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nType),sizeof(BYTE));
	for (int i=0; i<m_nSouXiaRelicCount; ++i)
	{
		iStream.Read((CHAR*)(&m_nCurBagIndex[i]),sizeof(BYTE));
		m_SouXiaData[i].ReadSouXiaVarAttr(iStream);
	}
	iStream.Read((CHAR*)(&m_nResult),sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCSouXiaDismantle::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nSouXiaRelicCount),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_nType),sizeof(BYTE));
	if(m_nSouXiaRelicCount > MAX_SOUXIA_RELIC) 
	{
		Assert(FALSE);
	}
	for (BYTE i=0; i<m_nSouXiaRelicCount; ++i)
	{
		if (m_SouXiaData[i].m_CurPos >= 0 && m_SouXiaData[i].m_SouXiaID > 0)
		{
			oStream.Write((CHAR*)(&m_nCurBagIndex[i]),sizeof(BYTE));
			m_SouXiaData[i].WriteSouXiaVarAttr(oStream);
		}
	}
	oStream.Write((CHAR*)(&m_nResult),sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSouXiaDismantle::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCSouXiaDismantleHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}


