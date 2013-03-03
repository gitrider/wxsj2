#ifndef _OBJ_ITEM_INL_
#define _OBJ_ITEM_INL_


VOID				Obj_Item::SetObj_ItemID( ObjID_t id )
{
	m_Obj_ItemId = id ; 
} 
ObjID_t				Obj_Item::GetObj_ItemID( )
{
	return m_Obj_ItemId ; 
} 

//VOID				Obj_Item::SaveObj_Item(Item* pItem)			//设置物品数据
//{
//	Assert(pItem);
//	memcpy(&m_Obj_Item, pItem, sizeof(Item));
//}
//
//Item*				Obj_Item::GetObj_Item()						//获取物品数据
//{
//	return &m_Obj_Item;
//}

GUID_t		Obj_Item::GetOwner(INT nPos)
{
	Assert(nPos<=MAX_TEAM_MEMBER);
	return m_OwnerId[nPos];
}

VOID				Obj_Item::SetOwner(GUID_t owner)
{
	Assert(m_OwnerCount<MAX_TEAM_MEMBER);

	m_OwnerId[m_OwnerCount]  = owner; 
	++m_OwnerCount;							//设置所有者
}
/*
void			Obj_Item::SetTeamOwner(TeamID_t	id)
{
m_OwnerTeam	= id;	
}
TeamID_t		Obj_Item::GetTeamOwner()
{
return m_OwnerTeam;
}
void			Obj_Item::SetItemPickRuler(IPC	ipc,UCHAR	uIndex)
{
Assert(uIndex<MAX_BOXITEM_NUMBER);

m_IPCList[uIndex] = ipc;
}
IPC			Obj_Item::GetItemPickRuler(UCHAR uIndex)
{
Assert(uIndex<MAX_BOXITEM_NUMBER);

return m_IPCList[uIndex];
}*/



INT					Obj_Item::GetType()	const
{
	return m_Obj_ItemType;
};
VOID				Obj_Item::SetType(INT	Type)
{
	m_Obj_ItemType = Type;
};

INT					Obj_Item::GetRecycleTimes()
{
	return m_RecycleTimes;
}
VOID				Obj_Item::SetRecycleTimes(INT RecycleTimes)
{
	m_RecycleTimes = RecycleTimes;
}


//UINT				Obj_Item::GetMaxGrowTime()
//{
//	return m_MaxGrowTime;
//}

inline VOID			Obj_Item::SetPoolPos(uint	iPos)
{
	m_nPoolPos	= iPos;
}
#endif
//----------------------------------------------------------------------------------------------------