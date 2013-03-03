// GCManipulatePetRet.cpp

#include "stdafx.h"
#include "GCManipulatePetRet.h"



BOOL GCManipulatePetRet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID));
	iStream.Read( (CHAR*)(&m_bFighting), sizeof(m_bFighting));
	iStream.Read( (CHAR*)(&m_Ret), sizeof(m_Ret));
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCManipulatePetRet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));
	oStream.Write( (CHAR*)(&m_bFighting), sizeof(m_bFighting));
	oStream.Write( (CHAR*)(&m_Ret), sizeof(m_Ret));
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCManipulatePetRet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCManipulatePetRetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

