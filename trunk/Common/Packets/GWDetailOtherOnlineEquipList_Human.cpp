#include "stdafx.h"
#include "GWDetailOtherOnlineEquipList_Human.h"

BOOL GWDetailOtherOnlineEquipList_Human::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_PlayerGUID), sizeof(m_PlayerGUID) ) ;
		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read((CHAR*)&m_Mode,sizeof(ASK_EQUIP_MODE));
	iStream.Read((CHAR*)&m_wPartFlags,sizeof(UINT));

	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{
			m_ItemList[i].ReadItemVarAttr(iStream);

			if((m_ItemList[i].ItemClass()!=ICLASS_EQUIP)&&(!m_ItemList[i].IsNullType()))
			{
				Assert(FALSE);
			}
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GWDetailOtherOnlineEquipList_Human::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_PlayerGUID), sizeof(m_PlayerGUID) ) ;
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write((CHAR*)&m_Mode,sizeof(ASK_EQUIP_MODE));
	oStream.Write((CHAR*)&m_wPartFlags,sizeof(UINT));



	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{

			if((m_ItemList[i].ItemClass()==ICLASS_EQUIP))
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

UINT	GWDetailOtherOnlineEquipList_Human::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWDetailOtherOnlineEquipList_HumanHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION
		return FALSE ;
}

