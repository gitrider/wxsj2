#include "stdafx.h"
#include "GCSouXiaList.h"

using namespace Packets;

BOOL GCSouXiaList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nSouXiaCount), sizeof(BYTE));
	
	for(INT i=0; i<m_nSouXiaCount; i++)
	{
		m_SouXiaData[i].ReadSouXiaVarAttr(iStream);
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCSouXiaList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nSouXiaCount),sizeof(BYTE));
	
	if(m_nSouXiaCount>MAX_SOUXIA_CONTAINER) 
	{
		Assert(FALSE);
	}

	for(INT i=0; i<m_nSouXiaCount; i++)
	{
		if (m_SouXiaData[i].m_CurPos >= 0 && m_SouXiaData[i].m_SouXiaID > 0)
		{
			m_SouXiaData[i].WriteSouXiaVarAttr(oStream);
		}
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSouXiaList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSouXiaListHandler::Execute( this, pPlayer );  //¿Í»§¶ËÐÞ¸Ä
	__LEAVE_FUNCTION

		return FALSE ;
}
