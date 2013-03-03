// GCBankAcquireList.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCBankAcquireList.h"

BOOL GCBankAcquireList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ItemNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_CurBankSize), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Money), sizeof(INT));
	for(INT i = 0; i< m_ItemNum; i++)
	{
		iStream.Read( (CHAR*)(&m_ItemList[i].bankindex), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_ItemList[i].isBlueEquip), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_ItemList[i].byNumber), sizeof(BYTE));
		if(m_ItemList[i].isBlueEquip)
		{
			m_ItemList[i].item_data.ReadItemVarAttr(iStream);
		}
		else
		{
			iStream.Read( (CHAR*)(&m_ItemList[i].item_guid), sizeof(UINT));
		}
	}


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankAcquireList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ItemNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_CurBankSize), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Money), sizeof(INT));

	for(INT i = 0; i< m_ItemNum; i++)
	{
		oStream.Write( (CHAR*)(&m_ItemList[i].bankindex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ItemList[i].isBlueEquip), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ItemList[i].byNumber), sizeof(BYTE));

		if(m_ItemList[i].isBlueEquip)
		{
			m_ItemList[i].item_data.WriteItemVarAttr(oStream);
		}
		else
		{
			oStream.Write( (CHAR*)(&m_ItemList[i].item_guid), sizeof(UINT));
		}
	}


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankAcquireList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankAcquireListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

