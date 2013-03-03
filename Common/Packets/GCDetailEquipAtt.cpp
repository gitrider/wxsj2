#include "stdafx.h"
#include "GCDetailEquipAtt.h"

using namespace Packets;
BOOL GCDetailEquipAtt::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read((CHAR*)&m_wPartFlags,sizeof(UINT));

	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{
			iStream.Read( (CHAR*)(&m_ItemList[i].m_nEquipPoint), sizeof(BYTE) ) ;
			iStream.Read( (CHAR*)(&m_ItemList[i].m_nEquipAtt.m_AttrType), sizeof(INT) ) ;
			iStream.Read( (CHAR*)(&m_ItemList[i].m_nEquipAtt.m_Value), sizeof(SHORT) ) ;
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCDetailEquipAtt::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write((CHAR*)&m_wPartFlags,sizeof(UINT));

	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{
			oStream.Write( (CHAR*)(&m_ItemList[i].m_nEquipPoint), sizeof(BYTE) ) ;
			oStream.Write( (CHAR*)(&m_ItemList[i].m_nEquipAtt.m_AttrType), sizeof(INT) ) ;
			oStream.Write( (CHAR*)(&m_ItemList[i].m_nEquipAtt.m_Value), sizeof(SHORT) ) ;
		}
	}


	return TRUE ;
	__LEAVE_FUNCTION

		return FALSE ;
}

UINT	GCDetailEquipAtt::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCDetailEquipAttHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION
		return FALSE ;
}