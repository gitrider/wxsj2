// GCShopSoldList.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCShopSoldList.h"

BOOL GCShopSoldList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	for(int i=0; i<m_nMerchadiseNum; ++i)
	{
		m_MerchadiseList[i].Read(iStream);
	}
	

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCShopSoldList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	for(int i=0; i<m_nMerchadiseNum; ++i)
	{
		m_MerchadiseList[i].Write(oStream);
	}
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCShopSoldList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopSoldListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

