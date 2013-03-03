// [7/8/2010 陈军龙]
#include "stdafx.h"
#include "GCDetailMountList.h"

BOOL GCDetailMountList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_ItemIndexFlags), sizeof(UINT) ) ;
	
	for(INT	i=0; i<MAX_MOUNT_SIZE; i++)
	{
		if( m_ItemIndexFlags & (1<<i) ) 
		{
			m_ItemList[i].ReadItemVarAttr(iStream);

			if((m_ItemList[i].ItemClass()!=ICLASS_MOUNT)&&(!m_ItemList[i].IsNullType()))
			{
				Assert(FALSE);
			}
		}
	}
	
	return TRUE ;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL GCDetailMountList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_ItemIndexFlags), sizeof(UINT) ) ;
	

	for(INT	i=0; i<MAX_MOUNT_SIZE; i++)
	{
		if( m_ItemIndexFlags & (1<<i) ) 
		{
			if((m_ItemList[i].ItemClass()==ICLASS_MOUNT))
			{
				m_ItemList[i].WriteItemVarAttr(oStream);
			}
			else
			{
				Assert(FALSE); //大概是往包里放数据写错了
			}
		}
	}
	

	  return TRUE ;
	__LEAVE_FUNCTION

	return FALSE ;
}

UINT	GCDetailMountList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
	
	return GCDetailMountListHandler::Execute( this, pPlayer ) ;
	
	__LEAVE_FUNCTION
	return FALSE ;
}