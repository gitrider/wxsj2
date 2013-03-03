#include "stdafx.h"
#include "GCNewObjItem.h"

using namespace Packets;

BOOL GCNewObjItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ProtectTime), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nItemIndex), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nItemCount), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nOwnerCount), sizeof(UINT));
	iStream.Read( (CHAR*)(m_idOwner), sizeof(GUID_t)*m_nOwnerCount);
	iStream.Read( (CHAR*)(&m_DropObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_OBJType), sizeof(INT));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCNewObjItem::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_ProtectTime), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nItemIndex), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nItemCount), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nOwnerCount), sizeof(UINT));
	oStream.Write( (CHAR*)(m_idOwner), sizeof(GUID_t)*m_nOwnerCount);
	oStream.Write( (CHAR*)(&m_DropObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_OBJType), sizeof(INT));


	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCNewObjItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCNewObjItemHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}