// [7/8/2010 ³Â¾üÁú]
#include "stdafx.h"
#include "CGManipulateMount.h"


using namespace Packets;


BOOL CGManipulateMount::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_MountIndex), sizeof(BYTE));
	    iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_MountID), sizeof(INT));
	    iStream.Read( (CHAR*)(&m_MountOperator), sizeof(MANIPULATE_MOUNT_TYPE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGManipulateMount::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_MountIndex), sizeof(BYTE));
	    oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_MountID), sizeof(INT));
	    oStream.Write( (CHAR*)(&m_MountOperator), sizeof(MANIPULATE_MOUNT_TYPE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGManipulateMount::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGManipulateMountHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}