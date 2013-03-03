// [7/8/2010 ³Â¾üÁú]
#include "stdafx.h"
#include "GCManipulateMountResult.h"


using namespace Packets;


BOOL GCManipulateMountResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Result), sizeof(BOOL));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_MountIndex), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_ItemID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_ObtainDate),sizeof(UINT));
	iStream.Read((CHAR*)(&m_RemainTime),sizeof(UINT));
	iStream.Read((CHAR*)(&m_MountGUID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_IsBinded),sizeof(BOOL));
	iStream.Read( (CHAR*)(&m_MountOperator), sizeof(MANIPULATE_MOUNT_TYPE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCManipulateMountResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write( (CHAR*)(&m_Result), sizeof(BOOL));
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_MountIndex), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_ItemID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_ObtainDate),sizeof(UINT));
	oStream.Write((CHAR*)(&m_RemainTime),sizeof(UINT));
	oStream.Write((CHAR*)(&m_MountGUID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_IsBinded),sizeof(BOOL));
	oStream.Write( (CHAR*)(&m_MountOperator), sizeof(MANIPULATE_MOUNT_TYPE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCManipulateMountResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCManipulateMountResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}