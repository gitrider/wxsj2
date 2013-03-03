// CGManipulatePet.cpp

#include "stdafx.h"
#include "CGManipulatePet.h"


BOOL CGManipulatePet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_PetGUID), sizeof(m_PetGUID));
	iStream.Read( (CHAR*)(&m_nType), sizeof(m_nType));
	iStream.Read( (CHAR*)(&m_CharObjID), sizeof(m_CharObjID));
	iStream.Read( (CHAR*)(&m_iTypeData), sizeof(m_iTypeData));
	iStream.Read( (CHAR*)(&m_sNickName), sizeof(m_sNickName));
	iStream.Read( (CHAR*)(&m_iBagIndex), sizeof(m_iBagIndex));
	iStream.Read( (CHAR*)(&m_uItemID), sizeof(m_uItemID));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGManipulatePet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_PetGUID), sizeof(m_PetGUID));
	oStream.Write( (CHAR*)(&m_nType), sizeof(m_nType));
	oStream.Write( (CHAR*)(&m_CharObjID), sizeof(m_CharObjID));
	oStream.Write( (CHAR*)(&m_iTypeData), sizeof(m_iTypeData));
	oStream.Write( (CHAR*)(&m_sNickName), sizeof(m_sNickName));
	oStream.Write( (CHAR*)(&m_iBagIndex), sizeof(m_iBagIndex));
	oStream.Write( (CHAR*)(&m_uItemID), sizeof(m_uItemID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGManipulatePet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGManipulatePetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
