// GCMerchandiseList.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCMerchandiseList.h"

BOOL GCMerchandiseList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_MerchadiseList), sizeof(_MERCHANDISE_ITEM)*m_nMerchadiseNum);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMerchandiseList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_MerchadiseList), sizeof(_MERCHANDISE_ITEM)*m_nMerchadiseNum);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMerchandiseList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMerchandiseListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

