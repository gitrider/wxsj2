#include "stdafx.h"
#include "GCSouXiaRefining.h"

GCSouXiaRefining::GCSouXiaRefining(VOID):m_nRefiningSouXia1(0),m_nRefiningSouXia2(0),m_nRefiningItem(0),m_nType(0)
{
}

GCSouXiaRefining::~GCSouXiaRefining(VOID)
{
}

BOOL GCSouXiaRefining::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nNewItemBagIndex),sizeof(BYTE));
	m_SouXiaData.ReadSouXiaVarAttr(iStream);
	iStream.Read((CHAR*)(&m_nResult),sizeof(BYTE));
	iStream.Read((CHAR*)(&m_nType),sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCSouXiaRefining::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write((CHAR*)(&m_nNewItemBagIndex),sizeof(BYTE));
	m_SouXiaData.WriteSouXiaVarAttr(oStream);
	oStream.Write((CHAR*)(&m_nResult),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_nType),sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSouXiaRefining::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCSouXiaRefiningHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}


