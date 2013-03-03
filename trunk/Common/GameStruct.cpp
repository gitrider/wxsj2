#include "stdafx.h"
 #include "Type.h"
#include "GameStruct.h"

#include "SocketInputStream.h"
#include "SocketOutputStream.h"

//-------------------------------------------------------------------------------------
BYTE		GetSerialClass(UINT Serial)
{
	return (BYTE)(Serial/1000000);
}

BYTE		GetSerialType(UINT Serial)
{
	return (BYTE)((Serial%1000000)/10000);
}

UINT		GetSerialIndex(UINT Serial)
{
	return (Serial%10000);
}

BOOL        ISCommonEquip(UINT Serial)
{
	return (Serial%10000)<1000 ? TRUE:FALSE;
}

UINT        GenMaterialIndex(UINT nMaterialType, UINT nMaterialLevel )
{
	Assert(nMaterialType<100);
	Assert(nMaterialLevel<=MAX_MATERIAL_LEVEL);
	return (ICLASS_MATERIAL*1000000)+(nMaterialType*10000)+nMaterialLevel;
}
//-------------------------------------------------------------------------------------

_ITEM_TYPE	ConvertSerial2ItemType(UINT Serial)
{
	_ITEM_TYPE it;
	it.m_Class		=	GetSerialClass(Serial);
	it.m_Type		=	GetSerialType(Serial);
	it.m_Index		=	GetSerialIndex(Serial);

	return it;
}

//获得物品有效数据大小
//-----------------------------------------------------------------------------------------
UINT	GetNewItemVarSize(const _ITEM& it)
{
	/*if(it.m_ItemIndex == 0)
		return 0;*/
	if(it.ItemClass()==ICLASS_EQUIP)
	{

		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			//////////////////////////////////////////////////////////////////////////
			sizeof(INT)+
			sizeof(BYTE);
	}
	else if(it.ItemClass()==ICLASS_GEM)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else if(it.ItemClass()==ICLASS_COMITEM || it.ItemClass()==ICLASS_IDENT)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else if(it.ItemClass()==ICLASS_MATERIAL)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else if(it.ItemClass()==ICLASS_TASKITEM)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else if(it.ItemClass()==ICLASS_STOREMAP)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else if(it.ItemClass()==ICLASS_SOUL_BEAD)// add by gh 2010/06/29
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT);
	}
	else
		return	sizeof(_ITEM_GUID)	+
		sizeof(UINT)		+
		sizeof(CHAR)		+
		sizeof(INT)*MAX_ITEM_PARAM  +
		sizeof(CHAR);
}

UINT	GetItemVarSize(const _ITEM& it)
{
	//if(it.m_ItemIndex == 0)
	//	return 0;
	INT CreatorSize = it.GetCreatorVar()?(sizeof(CHAR)*MAX_ITEM_CREATOR_NAME):0;
	if(it.ItemClass()==ICLASS_EQUIP)
	{

		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			//////////////////////////////////////////////////////////////////////////
			sizeof(BYTE)		+
			sizeof(BYTE)		+
			sizeof(UINT)		+
			//sizeof(INT)		+

			sizeof(BYTE)		+
			sizeof(BYTE)		+
			sizeof(BYTE)		+

			sizeof(INT)			+
			sizeof(INT)			+

			sizeof(USHORT)		+

			sizeof(BYTE)		+
			sizeof(BYTE)		+	

			sizeof(_ITEM_GEMINFO)*MAX_ITEM_GEM	+
			sizeof(BYTE)		+
			(sizeof(INT)+sizeof(SHORT))*it.GetEquipData()->m_AttrCount+
			CreatorSize			+
			sizeof(BYTE)		+//add by gh 2010/06/30 for Soul_bead
			sizeof(BYTE)		+
			(sizeof(INT)+sizeof(SHORT))*it.GetEquipData()->m_SoulBeadAttrCount;
		

	}
	else if(it.ItemClass()==ICLASS_GEM)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM	+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_COMITEM || it.ItemClass()==ICLASS_IDENT)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			sizeof(BYTE)		+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_MATERIAL)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			sizeof(BYTE)		+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_TASKITEM)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM+
			sizeof(BYTE)		+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_STOREMAP)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM		+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_EXTRABAG)
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM		+
			sizeof(INT)			+
			CreatorSize;
	}
	else if(it.ItemClass()==ICLASS_SOUL_BEAD)//add by gh
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM	+
			CreatorSize			+
			sizeof(UINT)		+
			sizeof(BYTE)		+
			sizeof(BYTE)		+
			(sizeof(INT)+sizeof(SHORT))*it.GetSoulBeadData()->m_nAttrCount;
	}
	else if(it.ItemClass()==ICLASS_MOUNT)  //  [7/16/2010 陈军龙]
	{
		return	sizeof(_ITEM_GUID)	+
			sizeof(UINT)		+
			sizeof(CHAR)		+
			sizeof(CHAR)		+
			sizeof(INT)*MAX_ITEM_PARAM	+
			CreatorSize			+
			sizeof(UINT)		+
			sizeof(INT)	    	+
			sizeof(INT)	    	+
			sizeof(INT)	    	+
			sizeof(INT)			+
			sizeof(BOOL)		+
			sizeof(UINT)	    +
			sizeof(UINT);
	}
	else
		return	sizeof(_ITEM_GUID)	+
		sizeof(UINT)		+
		sizeof(CHAR)		+
		sizeof(INT)*MAX_ITEM_PARAM+
		CreatorSize			+
		sizeof(CHAR);
}
//----------------------------------------------------------------------------

VOID	SwapItem(_ITEM* pItemA,_ITEM* pItemB)
{
	Assert(pItemA);
	Assert(pItemB);

	_ITEM tempItem;
	memcpy(&tempItem,pItemA,sizeof(_ITEM));
	memcpy(pItemA,pItemB,sizeof(_ITEM));
	memcpy(pItemB,&tempItem,sizeof(_ITEM));
}

VOID	OverWriteItem(_ITEM* pItemA,_ITEM* pItemB)
{
	Assert(pItemA);
	Assert(pItemB);

	memcpy(pItemB,pItemA,sizeof(_ITEM));
	memset(pItemA,0,sizeof(_ITEM));
}


BOOL	_ITEM::IsFixAttr(BYTE attrType)
{
	switch(attrType) 
	{
		case IATTRIBUTE_BASE_ATTACK_NEAR:
		case IATTRIBUTE_BASE_ATTACK_FAR:

		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
		
		case IATTRIBUTE_BASE_DEFENCE_NEAR:
		case IATTRIBUTE_BASE_DEFENCE_FAR:

		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:
		{
			return TRUE;
		}
		break;
	default:
		return FALSE;
	}
}

//VOID	_ITEM::ReadNewItemVarAttr(SocketInputStream& iStream)
//{
//	/*if(m_ItemIndex == 0)
//		return;*/
//	iStream.Read((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
//	iStream.Read((CHAR*)(&m_ItemIndex),sizeof(UINT));
//
//	if(ItemClass()==ICLASS_EQUIP)
//	{
//		iStream.Read((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
//		iStream.Read((CHAR*)(&GetEquipData()->m_nAttSet),sizeof(INT));
//	}
//	else if(ItemClass()==ICLASS_GEM)
//	{
//	}
//	else if(ItemClass()==ICLASS_COMITEM)
//	{
//	}
//	else if(ItemClass()==ICLASS_MATERIAL)
//	{
//	}
//	else if(ItemClass()==ICLASS_TASKITEM)
//	{
//	}
//	else if(ItemClass()==ICLASS_STOREMAP)
//	{
//	}
//}
//
//VOID	_ITEM::WriteNewItemVarAttr(SocketOutputStream& oStream) const
//{
//	/*if(m_ItemIndex == 0)
//		return;*/
//	oStream.Write((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
//	oStream.Write((CHAR*)(&m_ItemIndex),sizeof(UINT));
//
//	BYTE bClass = ItemClass();
//
//	if(bClass==ICLASS_EQUIP)
//	{
//		oStream.Write((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
//		oStream.Write((CHAR*)(&GetEquipData()->m_nAttSet),sizeof(INT));
//	}
//	else if(bClass==ICLASS_GEM)
//	{	
//	}
//	else if(bClass	== ICLASS_COMITEM)
//	{
//	}
//	else if(bClass == ICLASS_MATERIAL)
//	{
//	}
//	else if(bClass == ICLASS_TASKITEM)
//	{
//	}
//	else if(bClass	== ICLASS_STOREMAP)
//	{
//	}
//}

VOID	_ITEM::ReadItemVarAttr(SocketInputStream& iStream)
{
	/*if (m_ItemIndex == 0)
	{
		return;
	}*/
	iStream.Read((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_ItemIndex),sizeof(UINT));
	iStream.Read((CHAR*)(&m_RulerID),sizeof(CHAR));
	iStream.Read((CHAR*)(&m_nsBind),sizeof(CHAR));
	iStream.Read((CHAR*)(m_Param),sizeof(INT)*MAX_ITEM_PARAM);

	if(GetCreatorVar())
	{
		iStream.Read(m_Creator,sizeof(CHAR)*MAX_ITEM_CREATOR_NAME);
	}
	if(ItemClass()==ICLASS_EQUIP)
	{
		//iStream.Read((CHAR*)(&GetEquipData()->m_nAttSet),sizeof(INT));
		iStream.Read((CHAR*)(&GetEquipData()->m_Bind),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_PaiType),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_EquipScore),sizeof(UINT));

		iStream.Read((CHAR*)(&GetEquipData()->m_EquipType),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_Level),sizeof(BYTE));

		iStream.Read((CHAR*)(&GetEquipData()->m_CurMaxDur),sizeof(INT));
		iStream.Read((CHAR*)(&GetEquipData()->m_CurDurPoint),sizeof(INT));
		iStream.Read((CHAR*)(&GetEquipData()->m_CurDamagePoint),sizeof(USHORT));
		iStream.Read((CHAR*)(&GetEquipData()->m_CurGemHole),sizeof(BYTE));

		iStream.Read((CHAR*)(&GetEquipData()->m_StoneCount),sizeof(BYTE));
		if(GetEquipData()->m_StoneCount>MAX_ITEM_GEM)		
			GetEquipData()->m_StoneCount	= MAX_ITEM_GEM;
		
		iStream.Read((CHAR*)(&GetEquipData()->m_pGemInfo),sizeof(_ITEM_GEMINFO)*MAX_ITEM_GEM);


		iStream.Read((CHAR*)(&GetEquipData()->m_AttrCount),sizeof(BYTE));
		for (UINT i=0; i<GetEquipData()->m_AttrCount; ++i)
		{
			iStream.Read((CHAR*)(&GetEquipData()->m_pAttr[i].m_AttrType),sizeof(INT));
			iStream.Read((CHAR*)(&GetEquipData()->m_pAttr[i].m_Value.m_Value),sizeof(SHORT));
		}

		//add by gh 2010/06/60
		iStream.Read((CHAR*)(&GetEquipData()->m_SoulBeadType),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetEquipData()->m_SoulBeadAttrCount),sizeof(BYTE));
		if (SOUL_BEAD_WU == GetEquipData()->m_SoulBeadType)
		{
			for (UINT i=0; i<GetEquipData()->m_SoulBeadAttrCount; ++i)
			{
				iStream.Read((CHAR*)(&GetEquipData()->m_pWuSoulBeadAttr[i].m_AttrType),sizeof(INT));
				iStream.Read((CHAR*)(&GetEquipData()->m_pWuSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
		else if(SOUL_BEAD_QI == GetEquipData()->m_SoulBeadType)
		{
			for (UINT i=0; i<GetEquipData()->m_SoulBeadAttrCount; ++i)
			{
				iStream.Read((CHAR*)(&GetEquipData()->m_pQiSoulBeadAttr[i].m_AttrType),sizeof(INT));
				iStream.Read((CHAR*)(&GetEquipData()->m_pQiSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
	}
	else if(ItemClass()==ICLASS_GEM)
	{
	}
	else if(ItemClass()==ICLASS_COMITEM || ItemClass()==ICLASS_IDENT)
	{
		//当前数量
		iStream.Read((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if(ItemClass()==ICLASS_MATERIAL)
	{
		//当前数量
		iStream.Read((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if(ItemClass()==ICLASS_TASKITEM)
	{
		//当前数量
		iStream.Read((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if (ItemClass()==ICLASS_EXTRABAG)
	{
		iStream.Read((CHAR*)(&GetExtraBagData()->m_nBeginTime),sizeof(UINT));
	}
	else if(ItemClass()==ICLASS_STOREMAP)
	{
	}
	else if (ItemClass() == ICLASS_SOUL_BEAD)//add by gh 2010/06/29
	{
		iStream.Read((CHAR*)(&GetSoulBeadData()->m_nSellPrice),sizeof(UINT));
		iStream.Read((CHAR*)(&GetSoulBeadData()->m_nNeedLevel),sizeof(BYTE));
		iStream.Read((CHAR*)(&GetSoulBeadData()->m_nAttrCount),sizeof(BYTE));
		if( SOUL_BEAD_WU == ItemType())
		{
			for (UINT i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
			{
				iStream.Read((CHAR*)(&GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_AttrType),sizeof(INT));
				iStream.Read((CHAR*)(&GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		} 
		else if(SOUL_BEAD_QI == ItemType())
		{
			for (UINT i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
			{
				iStream.Read((CHAR*)(&GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_AttrType),sizeof(INT));
				iStream.Read((CHAR*)(&GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
	}
	else if (ItemClass() == ICLASS_MOUNT)  // [7/16/2010 陈军龙]
	{
		iStream.Read((CHAR*)(&GetMountData()->m_nNeedLevel),sizeof(UINT));
		iStream.Read((CHAR*)(&GetMountData()->m_nAddtionSpeed),sizeof(INT));
		iStream.Read((CHAR*)(&GetMountData()->m_nLife),sizeof(INT));
		iStream.Read((CHAR*)(&GetMountData()->m_nBindType),sizeof(INT));
		iStream.Read((CHAR*)(&GetMountData()->m_nPrice),sizeof(INT));
		iStream.Read((CHAR*)(&GetMountData()->m_bIsBinded),sizeof(BOOL));
		iStream.Read((CHAR*)(&GetMountData()->m_nObtainDate),sizeof(UINT));
		iStream.Read((CHAR*)(&GetMountData()->m_nRemainTime),sizeof(UINT));
	}
}

VOID	_ITEM::WriteItemVarAttr(SocketOutputStream& oStream) const
{
	/*if (m_ItemIndex == 0)
	{
		return;
	}*/
	oStream.Write((CHAR*)(&m_ItemGUID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_ItemIndex),sizeof(UINT));
	oStream.Write((CHAR*)(&m_RulerID),sizeof(CHAR));	
	oStream.Write((CHAR*)(&m_nsBind),sizeof(CHAR));
	oStream.Write((CHAR*)(m_Param),sizeof(INT)*MAX_ITEM_PARAM);

	if(GetCreatorVar())
	{
		oStream.Write(m_Creator,sizeof(CHAR)*MAX_ITEM_CREATOR_NAME);
	}

	BYTE bClass = ItemClass();

	if(bClass==ICLASS_EQUIP)
	{
		//oStream.Write((CHAR*)(&GetEquipData()->m_nAttSet),sizeof(INT));
		oStream.Write((CHAR*)(&GetEquipData()->m_Bind),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_PaiType),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_EquipScore),sizeof(UINT));

		oStream.Write((CHAR*)(&GetEquipData()->m_EquipType),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_NeedLevel),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_Level),sizeof(BYTE));

		oStream.Write((CHAR*)(&GetEquipData()->m_CurMaxDur),sizeof(INT));
		oStream.Write((CHAR*)(&GetEquipData()->m_CurDurPoint),sizeof(INT));
		oStream.Write((CHAR*)(&GetEquipData()->m_CurDamagePoint),sizeof(USHORT));
		oStream.Write((CHAR*)(&GetEquipData()->m_CurGemHole),sizeof(BYTE));

		oStream.Write((CHAR*)(&GetEquipData()->m_StoneCount),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_pGemInfo),sizeof(_ITEM_GEMINFO)*MAX_ITEM_GEM);
		
		oStream.Write((CHAR*)(&GetEquipData()->m_AttrCount),sizeof(BYTE));
		for (UINT i=0; i<GetEquipData()->m_AttrCount; ++i)
		{
			oStream.Write((CHAR*)(&GetEquipData()->m_pAttr[i].m_AttrType),sizeof(INT));
			oStream.Write((CHAR*)(&GetEquipData()->m_pAttr[i].m_Value.m_Value),sizeof(SHORT));
		}
		//add by gh 2010/06/60
		oStream.Write((CHAR*)(&GetEquipData()->m_SoulBeadType),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetEquipData()->m_SoulBeadAttrCount),sizeof(BYTE));
		
		if (SOUL_BEAD_WU == GetEquipData()->m_SoulBeadType)
		{
			for (UINT i=0; i<GetEquipData()->m_SoulBeadAttrCount; ++i)
			{
				oStream.Write((CHAR*)(&GetEquipData()->m_pWuSoulBeadAttr[i].m_AttrType),sizeof(INT));
				oStream.Write((CHAR*)(&GetEquipData()->m_pWuSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
		else if(SOUL_BEAD_QI == GetEquipData()->m_SoulBeadType)
		{
			for (UINT i=0; i<GetEquipData()->m_SoulBeadAttrCount; ++i)
			{
				oStream.Write((CHAR*)(&GetEquipData()->m_pQiSoulBeadAttr[i].m_AttrType),sizeof(INT));
				oStream.Write((CHAR*)(&GetEquipData()->m_pQiSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
	}
	else if(bClass==ICLASS_GEM)
	{	
	}
	else if(bClass	== ICLASS_COMITEM || bClass==ICLASS_IDENT)
	{
		//当前数量
		oStream.Write((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if(bClass == ICLASS_MATERIAL)
	{
		//当前数量
		oStream.Write((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if(bClass == ICLASS_TASKITEM)
	{
		//当前数量
		oStream.Write((CHAR*)(&GetMedicData()->m_nCount),sizeof(BYTE));
	}
	else if(bClass == ICLASS_EXTRABAG)
	{
		//当前数量
		oStream.Write((CHAR*)(&GetExtraBagData()->m_nBeginTime),sizeof(UINT));
	}
	else if(bClass	== ICLASS_STOREMAP)
	{
	}
	else if (bClass == ICLASS_SOUL_BEAD)//add by gh 2010/06/29
	{
		oStream.Write((CHAR*)(&GetSoulBeadData()->m_nSellPrice),sizeof(UINT));
		oStream.Write((CHAR*)(&GetSoulBeadData()->m_nNeedLevel),sizeof(BYTE));
		oStream.Write((CHAR*)(&GetSoulBeadData()->m_nAttrCount),sizeof(BYTE));
		if( SOUL_BEAD_WU == ItemType())
		{
			for (UINT i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
			{
				oStream.Write((CHAR*)(&GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_AttrType),sizeof(INT));
				oStream.Write((CHAR*)(&GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		} 
		else if(SOUL_BEAD_QI == ItemType())
		{
			for (UINT i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
			{
				oStream.Write((CHAR*)(&GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_AttrType),sizeof(INT));
				oStream.Write((CHAR*)(&GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_Value.m_Value),sizeof(SHORT));
			}
		}
	}
	else if (bClass == ICLASS_MOUNT)  // [7/16/2010 陈军龙]
	{
		oStream.Write((CHAR*)(&GetMountData()->m_nNeedLevel),sizeof(UINT));
		oStream.Write((CHAR*)(&GetMountData()->m_nAddtionSpeed),sizeof(INT));
		oStream.Write((CHAR*)(&GetMountData()->m_nLife),sizeof(INT));
		oStream.Write((CHAR*)(&GetMountData()->m_nBindType),sizeof(INT));
		oStream.Write((CHAR*)(&GetMountData()->m_nPrice),sizeof(INT));
		oStream.Write((CHAR*)(&GetMountData()->m_bIsBinded),sizeof(BOOL));
		oStream.Write((CHAR*)(&GetMountData()->m_nObtainDate),sizeof(UINT));
		oStream.Write((CHAR*)(&GetMountData()->m_nRemainTime),sizeof(UINT));
	}
}

//---------------------------------------------------------------------------------
BYTE	_ITEM::GetFixAttrCount()
{
	 return  GetEquipData()->m_AttrCount;
}

BOOL	_ITEM::GetItemBind() const
{
	if( m_nsBind & IEI_BIND_INFO)
		return TRUE;
	return FALSE;

	
}
VOID	_ITEM::SetItemBind(BOOL bBind)
{
	if(bBind)
		m_nsBind |= IEI_BIND_INFO;
	else
		m_nsBind &= (~(IEI_BIND_INFO));//这个操作一般不会执行
}

BOOL	_ITEM::GetItemIdent() const
{
	if( m_nsBind & IEI_IDEN_INFO)
		return TRUE;
	return FALSE;
}
VOID	_ITEM::SetItemIdent(BOOL bIdent)
{
	if(bIdent)
		m_nsBind |= IEI_IDEN_INFO;
	else
		m_nsBind &= (~(IEI_IDEN_INFO));//这个操作一般不会执行
}

BOOL	_ITEM::GetCreatorVar()	const
{
	if( m_nsBind & IEL_CREATOR)
		return TRUE;
	return FALSE;
}

VOID	_ITEM::SetCreatorVar(BOOL bCreator)	
{
	if(bCreator)
		m_nsBind |= IEL_CREATOR;
	else
		m_nsBind &= (~(IEL_CREATOR));//这个操作一般不会执行
}

BOOL	_ITEM::GetItemPLock() const
{
	if( m_nsBind & IEI_PLOCK_INFO)
		return TRUE;
	return FALSE;
}
VOID	_ITEM::SetItemPLock(BOOL bLock)
{
	if(bLock)
		m_nsBind |= IEI_PLOCK_INFO;
	else
		m_nsBind &= (~(IEI_PLOCK_INFO));//这个操作一般不会执行
}

INT		_ITEM::GetItemParamValue(UINT Start,ItemParamValue& ipv)
{
	INT TotalSize = sizeof(INT)*MAX_ITEM_PARAM;
	INT size;
	switch(ipv)
	{
	case IPV_CHAR:
		{
			size = 1;
			if(Start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			return *((CHAR*)m_Param+Start);
		}
		break;
	case IPV_SHORT:
		{
			size = 2;
			if(Start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			return *((SHORT*)((CHAR*)m_Param+Start));
		}
		break;
	case IPV_INT:
		{
			size = 4;
			if(Start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			return *((INT*)((CHAR*)m_Param+Start));
		}
		break;
	default:
		Assert(FALSE);
	}

	return -1;
}

VOID	_ITEM::SetItemParam(UINT start,ItemParamValue& ipv,INT value)
{
	INT TotalSize = sizeof(INT)*MAX_ITEM_PARAM;
	INT size;

	switch(ipv)
	{
	case IPV_CHAR:
		{
			size = 1;
			if(start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			*((CHAR*)m_Param+start) = (CHAR)value;
		}
		break;
	case IPV_SHORT:
		{
			size = 2;
			if(start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			*((SHORT*)((CHAR*)m_Param+start)) = (SHORT)value;
		}
		break;
	case IPV_INT:
		{
			size = 4;
			if(start>(UINT)((TotalSize-size)))
			{
				Assert(FALSE);
			}
			*((INT*)((CHAR*)m_Param+start)) = (INT)value;
		}
		break;
	default:
		Assert(FALSE);
	}
}



INT	_ITEM::WriteFixAttr(CHAR* pIn,INT BuffLength) //50
{
	Assert(pIn);
	
	BYTE bClass = ItemClass();
	
	CHAR*	pBuff = pIn;
	

	m_RulerID	=		*((BYTE*)pBuff);
	pBuff	   +=		sizeof(BYTE);

	m_nsBind	=		*((BYTE*)pBuff);
	pBuff	   +=		sizeof(BYTE);


	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			GetEquipData()->m_RulerID     =  *((BYTE*)pBuff)  ;				//规则
			pBuff		+=	sizeof(BYTE);
			GetEquipData()->m_BasePrice   =  *((UINT*)pBuff);				//售出价格
			pBuff		+=	sizeof(UINT);
			GetEquipData()->m_MaxDurPoint  =  *((INT*)pBuff);				//最大耐久值
			pBuff		+=	sizeof(INT);

			GetEquipData()->m_BaseGemHole = *((BYTE*)pBuff);				//天然宝石孔
			pBuff		+=	sizeof(BYTE);
			
			GetEquipData()->m_AttrCount = *((BYTE*)pBuff);					//属性数
			pBuff		+=	sizeof(BYTE);
			//固定属性在前面
			for(INT i = 0 ; i<GetEquipData()->m_AttrCount;i++)
			{
				GetEquipData()->m_pAttr[i].m_AttrType = *((INT*)pBuff);
				pBuff	+= sizeof(INT);

				GetEquipData()->m_pAttr[i].m_Value.m_Value	= *((SHORT*)pBuff);
				pBuff	+= sizeof(SHORT);
			}


		}
		break;
	case ICLASS_GEM:
		{
			GetGemData()->m_nPrice	=	*((INT*)pBuff);
			pBuff				+=	sizeof(INT);
			
			GetGemData()->m_nSellPrice	=	*((INT*)pBuff);
			pBuff				+=	sizeof(INT);

			GetGemData()->m_bBroadCast	=	*((BOOL*)pBuff);
			pBuff				+=	sizeof(BOOL);
			
			for (int i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			{
				GetGemData()->m_EquipType[i]	=	*((UINT*)pBuff);
				pBuff				+=	sizeof(UINT);
			}
			
			GetGemData()->m_AttCount = *((UINT*)pBuff);					//属性数
			pBuff		+=	sizeof(UINT);
			for(UINT i = 0 ; i<GetGemData()->m_AttCount;i++)
			{
				GetGemData()->m_GenAttr[i].m_AttrType = *((INT*)pBuff);
				pBuff	+= sizeof(INT);

				GetGemData()->m_GenAttr[i].m_Value.m_Value	= *((SHORT*)pBuff);
				pBuff	+= sizeof(SHORT);
			}

			GetGemData()->m_GemReqCount = *((UINT*)pBuff);					//属性数
			pBuff		+=	sizeof(UINT);
			for(UINT i = 0 ; i<GetGemData()->m_GemReqCount;i++)
			{
				GetGemData()->m_GemReqAttr[i].m_GemType = *((BYTE*)pBuff);
				pBuff	+= sizeof(BYTE);

				GetGemData()->m_GemReqAttr[i].m_Value.m_Value	= *((SHORT*)pBuff);
				pBuff	+= sizeof(SHORT);
			}
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			GetMedicData()->m_nBasePrice =		*((UINT*)pBuff);
			pBuff					    +=		sizeof(UINT);
			GetMedicData()->m_nSellPrice=		*((UINT*)pBuff);
			pBuff					    +=		sizeof(UINT);
			GetMedicData()->m_nLayedNum =		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nLevel	=		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nReqSkill	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_nReqSkillLevel	= *((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_nScriptID	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_nSkillID	=		*((INT*)pBuff);
			pBuff						+=		sizeof(INT);
			GetMedicData()->m_TargetType =		*((BYTE*)pBuff);
			pBuff						+=		sizeof(BYTE);
			GetMedicData()->m_bBroadcast =		*((BOOL*)pBuff);
			pBuff						+=		sizeof(BOOL);
		}
		break;
	case ICLASS_STOREMAP:
		{
			GetStoreMapData()->m_GrowType	= *((INT*)pBuff);	
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_nLevel	= *((INT*)pBuff);
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_SceneID= *((INT*)pBuff);
			pBuff						+= sizeof(INT);
			GetStoreMapData()->m_xPos	= *((FLOAT*)pBuff);
			pBuff						+= sizeof(FLOAT);
			GetStoreMapData()->m_zPos	= *((FLOAT*)pBuff);
			pBuff						+= sizeof(FLOAT);
		}
		break;
	case ICLASS_SOUL_BEAD://add by gh 2010/06/29
		{
		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	Assert(BuffLength>=(pBuff-pIn));

	return (INT)(pBuff-pIn);
}

BOOL	_ITEM::ReadFixAttr(CHAR* pOut,INT& OutLength,INT BuffLength)
{
	Assert(pOut);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pOut;

	BYTE	FixAttrCount = GetFixAttrCount();


	*((BYTE*)pBuff)  = m_RulerID;										//规则
	pBuff		+=	sizeof(BYTE);

	*((BYTE*)pBuff)  = m_nsBind;										//绑定
	pBuff		+=	sizeof(BYTE);

	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
	
			*((BYTE*)pBuff)  = GetEquipData()->m_RulerID;				 //规则
			pBuff		+=	sizeof(BYTE);
		
			*((UINT*)pBuff) = GetEquipData()->m_BasePrice;				//售出价格
			pBuff		+=	sizeof(UINT);
			*((INT*)pBuff)  = GetEquipData()->m_MaxDurPoint;				//最大耐久值
			pBuff		+=	sizeof(INT);
			*((BYTE*)pBuff)  = GetEquipData()->m_BaseGemHole;				//天然宝石孔
			pBuff		+=	sizeof(BYTE);
			*((BYTE*)pBuff)  = GetEquipData()->m_AttrCount;				//属性数
			pBuff		+=	sizeof(BYTE);
			

			//固定属性在前面
			for(INT i = 0 ; i<FixAttrCount;i++)
			{
				*((INT*)pBuff)	=	GetEquipData()->m_pAttr[i].m_AttrType;
				pBuff	+= sizeof(INT);

				*((SHORT*)pBuff)	=	GetEquipData()->m_pAttr[i].m_Value.m_Value;
				pBuff	+= sizeof(SHORT);
			}
		}
		break;
	case ICLASS_GEM:
		{
			/**((INT*)pBuff)   =  GetGemData()->m_nPrice	;
			pBuff				+=	sizeof(INT);
			*((BYTE*)pBuff)	 =  GetGemData()->m_Attr.m_AttrType;	
			pBuff				+= sizeof(BYTE);
			*((SHORT*)pBuff) =	GetGemData()->m_Attr.m_Value.m_Value ;
			pBuff				+= sizeof(SHORT);*/

			*((INT*)pBuff)		=	GetGemData()->m_nPrice;	
			pBuff				+=	sizeof(INT);

			*((INT*)pBuff)		=	GetGemData()->m_nSellPrice;
			pBuff				+=	sizeof(INT);

			*((BOOL*)pBuff)		=	GetGemData()->m_bBroadCast;
			pBuff				+=	sizeof(BOOL);

			for (int i=0; i<MAX_BASE_EQUIP_COUNT; ++i)
			{
				*((UINT*)pBuff)		=	GetGemData()->m_EquipType[i];
				pBuff				+=	sizeof(UINT);
			}

			*((UINT*)pBuff)		=	GetGemData()->m_AttCount;					//属性数
			pBuff		+=	sizeof(UINT);
			for(UINT i = 0 ; i<GetGemData()->m_AttCount;i++)
			{
				*((INT*)pBuff)		=	GetGemData()->m_GenAttr[i].m_AttrType;
				pBuff	+= sizeof(INT);

				*((SHORT*)pBuff)	=	GetGemData()->m_GenAttr[i].m_Value.m_Value;
				pBuff	+= sizeof(SHORT);
			}

			*((UINT*)pBuff)		=	GetGemData()->m_GemReqCount;					//属性数
			pBuff		+=	sizeof(UINT);
			for(UINT i = 0 ; i<GetGemData()->m_GemReqCount;i++)
			{
				*((BYTE*)pBuff)		=	GetGemData()->m_GemReqAttr[i].m_GemType;
				pBuff	+= sizeof(BYTE);

				*((SHORT*)pBuff)	=	GetGemData()->m_GemReqAttr[i].m_Value.m_Value;
				pBuff	+= sizeof(SHORT);
			}

		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			*((UINT*)pBuff) =	GetMedicData()->m_nBasePrice;
			pBuff					    +=	sizeof(UINT);
			*((UINT*)pBuff) =	GetMedicData()->m_nSellPrice;
			pBuff					    +=	sizeof(UINT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nLayedNum;
			pBuff						+=  sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nLevel;
			pBuff						+=  sizeof(BYTE);
			*((INT*)pBuff)	=	GetMedicData()->m_nReqSkill;
			pBuff						+=  sizeof(INT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_nReqSkillLevel;
			pBuff						+=  sizeof(BYTE);
			*((INT*)pBuff)	=	GetMedicData()->m_nScriptID;
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetMedicData()->m_nSkillID;
			pBuff						+= sizeof(INT);
			*((BYTE*)pBuff)	=	GetMedicData()->m_TargetType;
			pBuff						+= sizeof(BYTE);
			*((BOOL*)pBuff)   = GetMedicData()->m_bBroadcast;
			pBuff					    +=	sizeof(BOOL);
		}
		break;
	case ICLASS_STOREMAP:
		{
			*((INT*)pBuff)	=	GetStoreMapData()->m_GrowType;	
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetStoreMapData()->m_nLevel;
			pBuff						+= sizeof(INT);
			*((INT*)pBuff)	=	GetStoreMapData()->m_SceneID;
			pBuff						+= sizeof(INT);
			*((FLOAT*)pBuff)	=	GetStoreMapData()->m_xPos;
			pBuff						+= sizeof(FLOAT);
			*((FLOAT*)pBuff)	=	GetStoreMapData()->m_zPos;
			pBuff						+= sizeof(FLOAT);
		}
		break;
	case ICLASS_SOUL_BEAD://add by gh 2010/06/29
		{

		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}
	OutLength = (INT)(pBuff-pOut);
	Assert(BuffLength>=OutLength);

	return TRUE;
}

//一定要先序列化固定属性
//CHAR* pIn 实际上是整数序列
INT	_ITEM::WriteVarAttr(CHAR* pIn,INT BuffLength)
{
	Assert(pIn);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pIn;
	m_nsBind	=	*pBuff;
	pBuff++;
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			GetEquipData()->m_NeedLevel		=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);
			GetEquipData()->m_Bind		=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);
			GetEquipData()->m_PaiType		=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);
			GetEquipData()->m_EquipScore		=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(UINT);
			/*GetEquipData()->m_nAttSet		=	*((INT*)pBuff);
			pBuff	+= sizeof(INT);*/
			GetEquipData()->m_Level			=	*((BYTE*)pBuff);
			pBuff	+= sizeof(BYTE);
			GetEquipData()->m_BaseGemHole  =  *((BYTE*)pBuff);				//天然宝石孔
			pBuff		+=	sizeof(BYTE);
			GetEquipData()->m_CurMaxDur  =  *((INT*)pBuff);				//天然宝石孔
			pBuff		+=	sizeof(INT);
			GetEquipData()->m_CurDurPoint	=	*((INT*)pBuff);
			pBuff	+= sizeof(INT);
			GetEquipData()->m_CurDamagePoint	=	*((USHORT*)pBuff);		//损伤度一个SHORT
			pBuff	+= sizeof(USHORT);

			GetEquipData()->m_CurGemHole	=	*((BYTE*)pBuff);		//当前宝石孔
			pBuff	+= sizeof(BYTE);

			GetEquipData()->m_StoneCount	=	(BYTE)(*((BYTE*)pBuff));	//宝石个数一个SHORT
			pBuff	+= sizeof(BYTE);
			Assert(GetEquipData()->m_StoneCount<=MAX_ITEM_GEM);
			
			//每个宝石类型一个INT
			for( int i = 0;i<MAX_ITEM_GEM;i++)
			{
				GetEquipData()->m_pGemInfo[i].m_GemIndex	=	*((UINT*)pBuff);
				pBuff += sizeof(UINT);
			}
			// add by gh for 附魔
			GetEquipData()->m_SoulBeadType	=	*((BYTE*)pBuff);		//当前魂珠类型
			pBuff	+= sizeof(BYTE);
			
			GetEquipData()->m_SoulBeadAttrCount	=	*((BYTE*)pBuff);//当前魂珠附魔属性个数
			pBuff	+= sizeof(BYTE);
			//附魔属性
			if (SOUL_BEAD_WU == GetEquipData()->m_SoulBeadType)
			{
				for(INT i = 0 ; i<GetEquipData()->m_SoulBeadAttrCount;i++)
				{
					GetEquipData()->m_pWuSoulBeadAttr[i].m_AttrType = *((INT*)pBuff);
					pBuff	+= sizeof(INT);

					GetEquipData()->m_pWuSoulBeadAttr[i].m_Value.m_Value = *((SHORT*)pBuff);
					pBuff	+= sizeof(SHORT);
				}
			} 
			else if (SOUL_BEAD_QI == GetEquipData()->m_SoulBeadType)
			{
				for(INT i = 0 ; i<GetEquipData()->m_SoulBeadAttrCount;i++)
				{
					GetEquipData()->m_pQiSoulBeadAttr[i].m_AttrType = *((INT*)pBuff);
					pBuff	+= sizeof(INT);

					GetEquipData()->m_pQiSoulBeadAttr[i].m_Value.m_Value = *((SHORT*)pBuff);
					pBuff	+= sizeof(SHORT);
				}
			}
			
		}
		break;
	case ICLASS_GEM:
		{
			
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			GetMedicData()->m_nCount = *((BYTE*)pBuff);
			pBuff	+= sizeof(BYTE);
		}
		break;
	case ICLASS_STOREMAP:
		{

		}
		break;
	case ICLASS_SOUL_BEAD://add by gh 
		{
			GetSoulBeadData()->m_nSellPrice	=	*((UINT*)pBuff);
			pBuff	+=	sizeof(UINT);
			GetSoulBeadData()->m_nNeedLevel	=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);

			GetSoulBeadData()->m_nAttrCount	=	*((BYTE*)pBuff);
			pBuff	+=	sizeof(BYTE);

			if (SOUL_BEAD_WU == ItemType())
			{
				for (BYTE i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
				{
					GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_AttrType = *((INT*)pBuff);
					pBuff	+=	sizeof(INT);

					GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_Value.m_Value	=	*((SHORT*)pBuff);
					pBuff	+=	sizeof(SHORT);
				}		
			} 
			else if(SOUL_BEAD_QI == ItemType()) 
			{
				for (BYTE i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
				{
					GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_AttrType = *((INT*)pBuff);
					pBuff	+=	sizeof(INT);

					GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_Value.m_Value	=	*((SHORT*)pBuff);
					pBuff	+=	sizeof(SHORT);
				}		
			}// end of add

		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	Assert(BuffLength>=(pBuff-pIn));

	return (INT)(pBuff-pIn);
}

BOOL	_ITEM::ReadVarAttr(CHAR* pOut,INT& OutLength,INT BuffLength)
{
	Assert(pOut);

	BYTE bClass = ItemClass();

	CHAR*	pBuff = pOut;
	*pBuff	=	m_nsBind;
	pBuff++;
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			
			*((BYTE*)pBuff)	=	GetEquipData()->m_NeedLevel;
			pBuff	+= sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetEquipData()->m_Bind;;
			pBuff	+=	sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetEquipData()->m_PaiType;;
			pBuff	+=	sizeof(BYTE);
			*((UINT*)pBuff)	=	GetEquipData()->m_EquipScore;;
			pBuff	+=	sizeof(UINT);
			/**((INT*)pBuff)	=	GetEquipData()->m_nAttSet;;
			pBuff	+= sizeof(INT);*/
			*((BYTE*)pBuff)	=	GetEquipData()->m_Level;
			pBuff	+= sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetEquipData()->m_BaseGemHole;
			pBuff	+= sizeof(BYTE);
			*((INT*)pBuff)	=	GetEquipData()->m_CurMaxDur;
			pBuff	+= sizeof(INT);
			*((INT*)pBuff)	=	GetEquipData()->m_CurDurPoint;
			pBuff	+= sizeof(INT);
			*((USHORT*)pBuff)	=	GetEquipData()->m_CurDamagePoint;
			pBuff	+= sizeof(USHORT);
			*((BYTE*)pBuff)	=	GetEquipData()->m_CurGemHole;		
			pBuff	+= sizeof(BYTE);
			*((BYTE*)pBuff)	=	GetEquipData()->m_StoneCount;
			pBuff	+= sizeof(BYTE);
			Assert(GetEquipData()->m_StoneCount<=MAX_ITEM_GEM);
			for( int i = 0;i<MAX_ITEM_GEM;i++)
			{
				*((UINT*)pBuff)	=	GetEquipData()->m_pGemInfo[i].m_GemIndex;
				pBuff += sizeof(UINT);
			}

			// add by gh for 附魔
			*((BYTE*)pBuff) = GetEquipData()->m_SoulBeadType;		//当前魂珠类型
			pBuff	+= sizeof(BYTE);

			*((BYTE*)pBuff)  = GetEquipData()->m_SoulBeadAttrCount;//当前魂珠附魔属性个数
			pBuff	+= sizeof(BYTE);
			//附魔属性
			if (SOUL_BEAD_WU == GetEquipData()->m_SoulBeadType)
			{
				for(INT i = 0 ; i<GetEquipData()->m_SoulBeadAttrCount;i++)
				{
					*((INT*)pBuff)	=	GetEquipData()->m_pWuSoulBeadAttr[i].m_AttrType;
					pBuff	+= sizeof(INT);

					*((SHORT*)pBuff)	=	GetEquipData()->m_pWuSoulBeadAttr[i].m_Value.m_Value;
					pBuff	+= sizeof(SHORT);
				}
			} 
			else if (SOUL_BEAD_QI == GetEquipData()->m_SoulBeadType)
			{
				for(INT i = 0 ; i<GetEquipData()->m_SoulBeadAttrCount;i++)
				{
					*((INT*)pBuff)	=	GetEquipData()->m_pQiSoulBeadAttr[i].m_AttrType;
					pBuff	+= sizeof(INT);

					*((SHORT*)pBuff)	=	GetEquipData()->m_pQiSoulBeadAttr[i].m_Value.m_Value;
					pBuff	+= sizeof(SHORT);
				}
			}
		}
		break;
	case ICLASS_GEM:
		{

		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			*((BYTE*)pBuff)	=	GetMedicData()->m_nCount;
			pBuff	+= sizeof(BYTE);
		}
		break;
	case ICLASS_STOREMAP:
		{

		}
		break;
	case ICLASS_SOUL_BEAD://add by gh 
		{
			*((UINT*)pBuff)	= GetSoulBeadData()->m_nSellPrice;
			pBuff	+=	sizeof(UINT);

			*((BYTE*)pBuff) = GetSoulBeadData()->m_nNeedLevel;
			pBuff	+=	sizeof(BYTE);

			*((BYTE*)pBuff) = GetSoulBeadData()->m_nAttrCount;
			pBuff	+=	sizeof(BYTE);

			if (SOUL_BEAD_WU == ItemType())
			{
				for (BYTE i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
				{
					*((INT*)pBuff) = GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_AttrType;
					pBuff	+=	sizeof(INT);

					*((SHORT*)pBuff) = GetSoulBeadData()->m_pWuSoulBeadAttr[i].m_Value.m_Value;
					pBuff	+=	sizeof(SHORT);
				}		
			} 
			else if(SOUL_BEAD_QI == ItemType()) 
			{
				for (BYTE i=0; i<GetSoulBeadData()->m_nAttrCount; ++i)
				{
					*((INT*)pBuff) = GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_AttrType;
					pBuff	+=	sizeof(INT);

					*((SHORT*)pBuff) = GetSoulBeadData()->m_pQiSoulBeadAttr[i].m_Value.m_Value;
						pBuff	+=	sizeof(SHORT);
				}		
			}// end of add

		}
		break;
	default:
		{
			Assert(FALSE);
		}
	}

	OutLength = (INT)(pBuff-pOut);
	Assert(BuffLength>=OutLength);
	return TRUE;
}

BOOL	_ITEM::DBReadAttr(CHAR* pOut,INT& OutLength,INT BuffLength)
{
	INT nFixOutLength = 0;
	ReadFixAttr(pOut,nFixOutLength,BuffLength);
	INT nVarOutLength = 0;
	ReadVarAttr(pOut+nFixOutLength,nVarOutLength,BuffLength-nFixOutLength);
	OutLength = nFixOutLength+nVarOutLength;

	//物品参数
	CHAR*	pBuff = pOut+OutLength;
	for(INT i = 0; i < MAX_ITEM_PARAM; ++i)
	{
		*((INT*)pBuff)	=	m_Param[i];
		pBuff	+= sizeof(INT);
	}

	OutLength += (INT)(pBuff-pOut-OutLength);
	return (BuffLength>=OutLength)?TRUE:FALSE;
}

BOOL	_ITEM::DBWriteAttr(CHAR* pIn,INT BuffLength)
{
	INT nUsedLength = WriteFixAttr(pIn,BuffLength);
	nUsedLength += WriteVarAttr(pIn+nUsedLength,BuffLength-nUsedLength);

	//物品参数
	CHAR*	pBuff = pIn+nUsedLength;
	for(INT i = 0; i < MAX_ITEM_PARAM; ++i)
	{
		m_Param[i]	=	*((INT*)pBuff);
		pBuff	+= sizeof(INT);
	}

	nUsedLength += (INT)(pBuff-pIn-nUsedLength);
	return (BuffLength>=nUsedLength)?TRUE:FALSE;
}


BYTE	_ITEM::GetItemCount() const
{
	BYTE bClass = ItemClass();

	if(bClass==ICLASS_EQUIP)
	{
		return 1;

	}
	else if(bClass==ICLASS_GEM)
	{
		return 1;
	}
	else if(bClass==ICLASS_EXTRABAG)
	{
		return 1;
	}
	else if(bClass==ICLASS_COMITEM || bClass == ICLASS_IDENT)
	{

		return GetMedicData()->m_nCount;	
	}
	else if(bClass==ICLASS_MATERIAL)
	{

		return GetMedicData()->m_nCount;	
	}
	else if(bClass == ICLASS_TASKITEM)
	{
		return GetMedicData()->m_nCount;	
	}
	else if (bClass == ICLASS_SOUL_BEAD)
	{
		return 1;
	}
	else if(bClass == ICLASS_MOUNT)  // [7/8/2010 陈军龙]
	{
		return 1;
	}
	else
		return 0;
}

VOID	_ITEM::SetItemCount(INT nCount)	
{
	UCHAR bClass = ItemClass();

	if(bClass==ICLASS_COMITEM|| bClass == ICLASS_IDENT)
	{
		GetMedicData()->m_nCount	=	nCount;	
	}
	else if(bClass==ICLASS_MATERIAL)
	{

		GetMedicData()->m_nCount	=	nCount;	
	}
	else if(bClass==ICLASS_TASKITEM)
	{
		GetMedicData()->m_nCount	=	nCount;	
	}
	else
	{
		Assert(FALSE);
		return;
	}
}

BYTE	_ITEM::GetItemTileMax() const
{
	BYTE	bClass = ItemClass();


	switch(ItemClass()) 
	{
	case ICLASS_EQUIP:
		return 1;
		break;
	case ICLASS_GEM:
		return 1;
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			return GetMedicData()->m_nLayedNum;
		}
		break;
	case ICLASS_STOREMAP:
		{
			return 1;
		}
	case ICLASS_EXTRABAG:
		{
			return 1;
		}
	case  ICLASS_SOUL_BEAD:
		{
			return 1;
		}
	case  ICLASS_MOUNT:  // [7/8/2010 陈军龙]
		{
		    return 1;
		}
		break;
	default:
		{
			AssertEx(FALSE,"not implement class");
			return 0;
		}
	}
}

//是否达到单叠最大个数
BOOL	_ITEM::isFullTile() const
{
	return GetItemCount()>=GetItemTileMax();
}

//增加个数
BOOL	_ITEM::IncCount(UINT nCount /* =1 */)
{
	
	const	BYTE	bClass = ItemClass();

	switch(bClass) {
	case ICLASS_EQUIP:
		{
			return FALSE;
		}
		break;
	case ICLASS_GEM:
		{
			return FALSE;
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			Assert(!isFullTile());
			GetMedicData()->m_nCount+=nCount;	
			return TRUE;
		}
		break;
	case ICLASS_SOUL_BEAD:
		{
			return FALSE;
		}
		break;
	case ICLASS_MOUNT:  // [7/8/2010 陈军龙]
		{
		    return TRUE;
		}
		break;
	default:
		return FALSE;
		break;
	}
	
	
}

//减少个数
BOOL	_ITEM::DecCount(UINT nCount /* =1 */)
{
	const	BYTE	bClass = ItemClass();
	switch(bClass) 
	{
	case ICLASS_EQUIP:
		{
			return FALSE;
		}
		break;
	case ICLASS_GEM:
		{
			return FALSE;
		}
		break;
	case ICLASS_TASKITEM:
	case ICLASS_MATERIAL:
	case ICLASS_COMITEM:
	case ICLASS_IDENT:
		{
			Assert(GetItemCount()>0);
			GetMedicData()->m_nCount-=nCount;	
			return TRUE;
         }
		break;
	case ICLASS_SOUL_BEAD:
		{
			return FALSE;
		}
		break;
	default:
		return FALSE;
		break;
	}
}

BOOL Item2String(_ITEM* pItem,CHAR* pOut,INT& OutLength,INT BuffLength)
{

		Assert(pItem);
		Assert(pOut);
		
		CHAR*	pBuff   = pOut;
		*((UINT*)pBuff)	= pItem->m_ItemIndex;

		pBuff +=sizeof(UINT);
		pItem->ReadVarAttr( pBuff, OutLength, BuffLength - sizeof(UINT) );		
		OutLength += sizeof(UINT);

		CHAR	p[MAX_ITEM_STRING_LENGTH];
		memset( p, 0, MAX_ITEM_STRING_LENGTH );
		Binary2String( pOut, OutLength, p );

		//保存转化的结果	20100706 BLL
		CHAR strLen[16] = {0};
		tsnprintf( strLen, 15, "%03di", strlen(p)+1 );

		strcpy( pOut, strLen );
		strcpy(pOut+strlen(pOut),p);
		OutLength = (INT)strlen(pOut);

		return TRUE;
}

BOOL String2Item(const CHAR* pIn,INT BuffLength,_ITEM* pItem)
{
	Assert(pItem);
	Assert(pIn);
	
	
	CHAR	p[255];
	memset(p,0,255);
	UINT	iOut;
	DBStr2Binary(pIn,BuffLength,p,255,iOut);
	CHAR*	pBuff = p;
	pItem->m_ItemIndex =*((UINT*)pBuff);
	pBuff+=	sizeof(UINT);
	pItem->WriteVarAttr(pBuff,iOut);
	return TRUE;
}


VOID GUILD_CGW_ASKLIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_SortType), sizeof(m_SortType));
	iStream.Read((CHAR*)(&m_uStart), sizeof(m_uStart));
}

VOID GUILD_CGW_ASKLIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_SortType), sizeof(m_SortType));
	oStream.Write((CHAR*)(&m_uStart), sizeof(m_uStart));
}

VOID GUILD_CGW_CREATE::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_NameSize), sizeof(m_NameSize));

	if ( m_NameSize > 0 )
	{
		iStream.Read((CHAR*)m_szGuildName, m_NameSize);
	}

	iStream.Read((CHAR*)(&m_DescSize), sizeof(m_DescSize));

	if ( m_DescSize > 0 )
	{
		iStream.Read((CHAR*)m_szGuildDesc, m_DescSize);
	}
}

VOID GUILD_CGW_CREATE::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_NameSize), sizeof(m_NameSize));

	if ( m_NameSize > 0 )
	{
		oStream.Write((CHAR*)m_szGuildName, m_NameSize);
	}

	oStream.Write((CHAR*)(&m_DescSize), sizeof(m_DescSize));

	if ( m_DescSize > 0 )
	{
		oStream.Write((CHAR*)m_szGuildDesc, m_DescSize);
	}
}

// 修改帮会宗旨
VOID GUILD_CGW_MODIFY_DESC::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_DescSize), sizeof(m_DescSize));

    if ( m_DescSize > 0 )
    {
        iStream.Read((CHAR*)m_szGuildDesc, m_DescSize);
    }
}

VOID GUILD_CGW_MODIFY_DESC::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_DescSize), sizeof(m_DescSize));

    if ( m_DescSize > 0 )
    {
        oStream.Write((CHAR*)m_szGuildDesc, m_DescSize);
    }
}


// 修改万家帮会称号名字
VOID GUILD_CGW_MODIFY_TITLE_NAME::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_NameSize), sizeof(m_NameSize));

    if ( m_NameSize > 0 )
    {
        iStream.Read((CHAR*)m_szTitleName, m_NameSize);
    }
}

VOID GUILD_CGW_MODIFY_TITLE_NAME::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_NameSize), sizeof(m_NameSize));

    if ( m_NameSize > 0 )
    {
        oStream.Write((CHAR*)m_szTitleName, m_NameSize);
    }
}

VOID GUILD_CGW_JOIN::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    iStream.Read((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}

VOID GUILD_CGW_JOIN::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    oStream.Write((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}

VOID GUILD_CGW_FAMILY_DISMISS::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    iStream.Read((CHAR*)(&m_nFamilyID), sizeof(m_nFamilyID));
}

VOID GUILD_CGW_FAMILY_DISMISS::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    oStream.Write((CHAR*)(&m_nFamilyID), sizeof(m_nFamilyID));
}

VOID GUILD_CGW_ASKINFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_Type), sizeof(m_Type));
	iStream.Read((CHAR*)(&m_cPwdSize), sizeof(m_cPwdSize));
    if (m_cPwdSize>0)
    {
	    iStream.Read((CHAR*)(m_Password), m_cPwdSize * sizeof(CHAR));
    }
}

VOID GUILD_CGW_ASKINFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_Type), sizeof(m_Type));
    oStream.Write((CHAR*)(&m_cPwdSize), sizeof(m_cPwdSize));
    if (m_cPwdSize>0)
    {
        oStream.Write((CHAR*)(m_Password), m_cPwdSize*sizeof(CHAR));
    }
}

VOID GUILD_CGW_APPOINT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	iStream.Read((CHAR*)(&m_NewPosition), sizeof(m_NewPosition));
}

VOID GUILD_CGW_APPOINT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	oStream.Write((CHAR*)(&m_NewPosition), sizeof(m_NewPosition));
}

VOID GUILD_CGW_ADJUSTAUTH::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	iStream.Read((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	iStream.Read((CHAR*)(&m_NewAuthority), sizeof(m_NewAuthority));
}

VOID GUILD_CGW_ADJUSTAUTH::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
	oStream.Write((CHAR*)(&m_CandidateGUID), sizeof(m_CandidateGUID));
	oStream.Write((CHAR*)(&m_NewAuthority), sizeof(m_NewAuthority));
}

VOID GUILD_CGW_LEVELUP::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    iStream.Read((CHAR*)(&m_ChiefGUID), sizeof(m_ChiefGUID));
    iStream.Read((CHAR*)(&m_GuildLevel), sizeof(m_GuildLevel));
    iStream.Read((CHAR*)(&m_GP), sizeof(m_GP));
}

VOID GUILD_CGW_LEVELUP::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    oStream.Write((CHAR*)(&m_ChiefGUID), sizeof(m_ChiefGUID));
    oStream.Write((CHAR*)(&m_GuildLevel), sizeof(m_GuildLevel));
    oStream.Write((CHAR*)(&m_GP), sizeof(m_GP));
}

VOID GUILD_CGW_GP::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    iStream.Read((CHAR*)(&m_GP), sizeof(m_GP));
}

VOID GUILD_CGW_GP::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_GuildGUID), sizeof(m_GuildGUID));
    oStream.Write((CHAR*)(&m_GP), sizeof(m_GP));
}

VOID GUILD_CGW_LEVELINFO::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_MaxUser), sizeof(m_MaxUser));
    iStream.Read((CHAR*)(&m_MaxFamily), sizeof(m_MaxFamily));
    iStream.Read((CHAR*)(&m_MaxGoldFamilyUser), sizeof(m_MaxGoldFamilyUser));
}

VOID GUILD_CGW_LEVELINFO::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_MaxUser), sizeof(m_MaxUser));
    oStream.Write((CHAR*)(&m_MaxFamily), sizeof(m_MaxFamily));
    oStream.Write((CHAR*)(&m_MaxGoldFamilyUser), sizeof(m_MaxGoldFamilyUser));
}


VOID GUILD_CGW_RECRUIT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_ProposerGUID), sizeof(m_ProposerGUID));
}

VOID GUILD_CGW_RECRUIT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_ProposerGUID), sizeof(m_ProposerGUID));
}

VOID GUILD_CGW_EXPEL::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildUserGUID), sizeof(m_GuildUserGUID));
}

VOID GUILD_CGW_EXPEL::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildUserGUID), sizeof(m_GuildUserGUID));
}

VOID GUILD_CGW_WITHDRAW::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_WITHDRAW::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_DEPOSIT::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_DEPOSIT::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_MoneyAmount), sizeof(m_MoneyAmount));
}

VOID GUILD_CGW_CHANG_DESC::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildDescLength), sizeof(BYTE));
	if(m_GuildDescLength>0)
	{
		iStream.Read((CHAR*)(m_GuildDesc), m_GuildDescLength*sizeof(CHAR));
	}
}

VOID GUILD_CGW_CHANG_DESC::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildDescLength), sizeof(BYTE));
	if(m_GuildDescLength>0)
	{
		oStream.Write((CHAR*)(m_GuildDesc), m_GuildDescLength*sizeof(CHAR));
	}
}


VOID GUILD_CGW_INVITE::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_NameSize), sizeof(m_NameSize));

    if(m_NameSize>0)
    {
        iStream.Read((CHAR*)(m_szTargetName), m_NameSize*sizeof(CHAR));
    }
}
VOID GUILD_CGW_INVITE::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_NameSize), sizeof(m_NameSize));
    if (m_NameSize>0)
    {
        oStream.Write((CHAR*)(&m_szTargetName), m_NameSize*sizeof(CHAR));
    }
}

VOID GUILD_CGW_INVITE_CONFIRM::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildId), sizeof(GuildID_t));
	iStream.Read((CHAR*)(&m_AgreeInvite), sizeof(BOOL));
	iStream.Read((CHAR*)(&m_InvitorNameSize), sizeof(m_InvitorNameSize));
    if (m_InvitorNameSize>0)
    {
	    iStream.Read((CHAR*)(&m_InvitorName), m_InvitorNameSize);
    }
}
VOID GUILD_CGW_INVITE_CONFIRM::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildId), sizeof(GuildID_t));
	oStream.Write((CHAR*)(&m_AgreeInvite), sizeof(BOOL));
    oStream.Write((CHAR*)(&m_InvitorNameSize), sizeof(m_InvitorNameSize));
    if (m_InvitorNameSize>0)
    {
        oStream.Write((CHAR*)(&m_InvitorName), m_InvitorNameSize);
    }
}

// 创建家族
VOID GUILD_CGW_CREATE_FAMILY::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_GuildGUID), sizeof(GuildID_t));
    iStream.Read((CHAR*)(&m_ShaikhIndex), sizeof(m_ShaikhIndex));
    iStream.Read((CHAR*)(&m_ShaikhGuid), sizeof(m_ShaikhGuid));
        
	iStream.Read((CHAR*)(&m_cDescSize), sizeof(m_cDescSize));
    if (m_cDescSize>0)
    {
        iStream.Read((CHAR*)(&m_szFamilyDesc), m_cDescSize);
    }
   
	iStream.Read((CHAR*)(&m_cNameSize), sizeof(m_cNameSize));
    if (m_cNameSize>0)
    {
        iStream.Read((CHAR*)(&m_szFamilyName), m_cNameSize);
    }
}
VOID GUILD_CGW_CREATE_FAMILY::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_GuildGUID), sizeof(GuildID_t));
    oStream.Write((CHAR*)(&m_ShaikhIndex), sizeof(m_ShaikhIndex));
    oStream.Write((CHAR*)(&m_ShaikhGuid), sizeof(m_ShaikhGuid));
    
	oStream.Write((CHAR*)(&m_cDescSize), sizeof(m_cDescSize));
    if (m_cDescSize>0)
    {
        oStream.Write((CHAR*)(&m_szFamilyDesc), m_cDescSize);
    }

    oStream.Write((CHAR*)(&m_cNameSize), sizeof(m_cNameSize));
    if (m_cNameSize>0)
    {
        oStream.Write((CHAR*)(&m_szFamilyName), m_cNameSize);
    }
}

// 添加家族成员
VOID GUILD_CGW_ADD_FAMILY_USER::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_cFamilyIndex), sizeof(m_cFamilyIndex));
    iStream.Read((CHAR*)(&m_iGuildIndex), sizeof(m_iGuildIndex));
    iStream.Read((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}
VOID GUILD_CGW_ADD_FAMILY_USER::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_cFamilyIndex), sizeof(m_cFamilyIndex));
    oStream.Write((CHAR*)(&m_iGuildIndex), sizeof(m_iGuildIndex));
    oStream.Write((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}

// 删除家族成员
VOID GUILD_CGW_REMOVE_FAMILY_USER::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_cFamilyIndex), sizeof(m_cFamilyIndex));
    iStream.Read((CHAR*)(&m_iGuildIndex), sizeof(m_iGuildIndex));
    iStream.Read((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}
VOID GUILD_CGW_REMOVE_FAMILY_USER::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_cFamilyIndex), sizeof(m_cFamilyIndex));
    oStream.Write((CHAR*)(&m_iGuildIndex), sizeof(m_iGuildIndex));
    oStream.Write((CHAR*)(&m_TargetGuid), sizeof(m_TargetGuid));
}

VOID GUILD_WGC_APPOINT_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&iPosNum), sizeof(iPosNum));
	iStream.Read((CHAR*)(m_PosList), sizeof(s)*iPosNum);
}

VOID GUILD_WGC_APPOINT_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&iPosNum), sizeof(iPosNum));
	oStream.Write((CHAR*)(m_PosList), sizeof(s)*iPosNum);
}

VOID GUILD_WGC_GUILD_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	iStream.Read((CHAR*)(m_GuildCreator), MAX_CHARACTER_NAME);
	iStream.Read((CHAR*)(m_GuildChairMan), MAX_CHARACTER_NAME);
	iStream.Read((CHAR*)(m_CityName), MAX_CITY_NAME_SIZE);
	iStream.Read((CHAR*)(&m_nLevel), sizeof(m_nLevel));
	iStream.Read((CHAR*)(&m_nPortSceneID), sizeof(m_nPortSceneID));
	iStream.Read((CHAR*)(&m_MemNum), sizeof(m_MemNum));
	iStream.Read((CHAR*)(&m_Longevity), sizeof(m_Longevity));
	iStream.Read((CHAR*)(&m_Contribute), sizeof(m_Contribute));
	iStream.Read((CHAR*)(&m_Honor), sizeof(m_Honor));
	iStream.Read((CHAR*)(&m_FoundedMoney), sizeof(m_FoundedMoney));
	iStream.Read((CHAR*)(&m_nIndustryLevel), sizeof(m_nIndustryLevel));
	iStream.Read((CHAR*)(&m_nAgrLevel), sizeof(m_nAgrLevel));
	iStream.Read((CHAR*)(&m_nComLevel), sizeof(m_nComLevel));
	iStream.Read((CHAR*)(&m_nDefLevel), sizeof(m_nDefLevel));
	iStream.Read((CHAR*)(&m_nTechLevel), sizeof(m_nTechLevel));
	iStream.Read((CHAR*)(&m_nAmbiLevel), sizeof(m_nAmbiLevel));
}

VOID GUILD_WGC_GUILD_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	oStream.Write((CHAR*)(m_GuildCreator), MAX_CHARACTER_NAME);
	oStream.Write((CHAR*)(m_GuildChairMan), MAX_CHARACTER_NAME);
	oStream.Write((CHAR*)(m_CityName), MAX_CITY_NAME_SIZE);
	oStream.Write((CHAR*)(&m_nLevel), sizeof(m_nLevel));
	oStream.Write((CHAR*)(&m_nPortSceneID), sizeof(m_nPortSceneID));
	oStream.Write((CHAR*)(&m_MemNum), sizeof(m_MemNum));
	oStream.Write((CHAR*)(&m_Longevity), sizeof(m_Longevity));
	oStream.Write((CHAR*)(&m_Contribute), sizeof(m_Contribute));
	oStream.Write((CHAR*)(&m_Honor), sizeof(m_Honor));
	oStream.Write((CHAR*)(&m_FoundedMoney), sizeof(m_FoundedMoney));
	oStream.Write((CHAR*)(&m_nIndustryLevel), sizeof(m_nIndustryLevel));
	oStream.Write((CHAR*)(&m_nAgrLevel), sizeof(m_nAgrLevel));
	oStream.Write((CHAR*)(&m_nComLevel), sizeof(m_nComLevel));
	oStream.Write((CHAR*)(&m_nDefLevel), sizeof(m_nDefLevel));
	oStream.Write((CHAR*)(&m_nTechLevel), sizeof(m_nTechLevel));
	oStream.Write((CHAR*)(&m_nAmbiLevel), sizeof(m_nAmbiLevel));
}

VOID GUILD_WGC_MEMBER_LIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_uValidMemberCount), sizeof(m_uValidMemberCount));
	iStream.Read((CHAR*)(&m_uMemberCount), sizeof(m_uMemberCount));
	iStream.Read((CHAR*)(&m_uMemberMax), sizeof(m_uMemberMax));
    iStream.Read((CHAR*) (&m_uFamilyCount), sizeof(m_uFamilyCount));
	iStream.Read((CHAR*)(&m_bPosition), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_bAccess), sizeof(BYTE));
	iStream.Read((CHAR*)(m_GuildDesc), MAX_GUILD_DESC_SIZE);
	iStream.Read((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);
	if( m_uMemberCount > 0 )
	{
		iStream.Read((CHAR*)m_GuildMemberData, sizeof(s) * m_uMemberCount);
	}
    if( m_uFamilyCount > 0 )
    {
        iStream.Read((CHAR*)m_GuildFamilyData, sizeof(f) * m_uFamilyCount);
    }
}

VOID GUILD_WGC_MEMBER_LIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_uValidMemberCount), sizeof(m_uValidMemberCount));
	oStream.Write((CHAR*)(&m_uMemberCount), sizeof(m_uMemberCount));
	oStream.Write((CHAR*)(&m_uMemberMax), sizeof(m_uMemberMax));
    oStream.Write((CHAR*) (&m_uFamilyCount), sizeof(m_uFamilyCount));
	oStream.Write((CHAR*)(&m_bPosition), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_bAccess), sizeof(BYTE));
	oStream.Write((CHAR*)(m_GuildDesc), MAX_GUILD_DESC_SIZE);
	oStream.Write((CHAR*)(m_GuildName), MAX_GUILD_NAME_SIZE);

	if( m_uMemberCount > 0 )
	{
		oStream.Write((CHAR*)m_GuildMemberData, sizeof(s) * m_uMemberCount);
	}
    if( m_uFamilyCount > 0 )
    {
        oStream.Write((CHAR*)m_GuildFamilyData, sizeof(f) * m_uFamilyCount);
    }
}

VOID GUILD_WGC_LIST::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_uStartIndex), sizeof(m_uStartIndex));
	iStream.Read((CHAR*)(&m_uGuildCount), sizeof(m_uGuildCount));
	iStream.Read((CHAR*)(&m_uGuildListCount), sizeof(m_uGuildListCount));

	if( m_uGuildListCount > 0 )
	{
		iStream.Read((CHAR*)m_uGuild, sizeof(s) * m_uGuildListCount);
	}
}

VOID GUILD_WGC_LIST::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_uStartIndex), sizeof(m_uStartIndex));
	oStream.Write((CHAR*)(&m_uGuildCount), sizeof(m_uGuildCount));
	oStream.Write((CHAR*)(&m_uGuildListCount), sizeof(m_uGuildListCount));

	if( m_uGuildListCount > 0 )
	{
		oStream.Write((CHAR*)m_uGuild, sizeof(s) * m_uGuildListCount);
	}
}

// 转发邀请加入帮会的请求
VOID GUILD_WGC_INVITE::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildId), sizeof(m_GuildId));

    iStream.Read((CHAR*)(&m_GuildNameSize), sizeof(m_GuildNameSize));
    if (m_GuildNameSize>0)
    {
        iStream.Read((CHAR*)(&m_GuildName), m_GuildNameSize);
    }

    iStream.Read((CHAR*)(&m_InvitorNameSize), sizeof(m_InvitorNameSize));
    if (m_InvitorNameSize>0)
    {
        iStream.Read((CHAR*)&m_InvitorName, m_InvitorNameSize);
    }
}

VOID GUILD_WGC_INVITE::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildId), sizeof(m_GuildId));

    oStream.Write((CHAR*)(&m_GuildNameSize), sizeof(m_GuildNameSize));
    if (m_GuildNameSize>0)
    {
        oStream.Write((CHAR*)(&m_GuildName), m_GuildNameSize);
    }

    oStream.Write((CHAR*)(&m_InvitorNameSize), sizeof(m_InvitorNameSize));
    if (m_InvitorNameSize>0)
    {
        oStream.Write((CHAR*)&m_InvitorName, m_InvitorNameSize);
    }
}

VOID GUILD_WGC_SELF_GUILD_INFO::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_GuildNameSize), sizeof(BYTE));
	
	if( m_GuildNameSize > 0 )
	{
		iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
	}
}

VOID GUILD_WGC_SELF_GUILD_INFO::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_GuildNameSize), sizeof(BYTE));

	if( m_GuildNameSize > 0 )
	{
		oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
	}
}

//修改帮派称号名字
VOID GUILD_WGC_MODIFY_TITLE_NAME::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_Guid), sizeof(GUID_t));
    iStream.Read((CHAR*)(&m_uTitleNameSize), sizeof(BYTE));

    if( m_uTitleNameSize > 0 )
    {
        iStream.Read((CHAR*)m_szTitleName, m_uTitleNameSize);
    }
}

VOID GUILD_WGC_MODIFY_TITLE_NAME::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_Guid), sizeof(GUID_t));
    oStream.Write((CHAR*)(&m_uTitleNameSize), sizeof(BYTE));

    if( m_uTitleNameSize > 0 )
    {
        oStream.Write((CHAR*)m_szTitleName, m_uTitleNameSize);
    }
}


//邀请加入帮派
VOID GUILD_WGC_INVITE_INFO::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)&m_GuildId, sizeof(m_GuildId));
    iStream.Read((CHAR*)&m_TargetGuid, sizeof(m_TargetGuid));
    iStream.Read((CHAR*)(&m_TargetNameSize), sizeof(BYTE));
    if( m_TargetNameSize > 0 )
    {
        iStream.Read((CHAR*)m_TargetName, m_TargetNameSize);
    }
}

VOID GUILD_WGC_INVITE_INFO::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)&m_GuildId, sizeof(m_GuildId));
    oStream.Write((CHAR*)&m_TargetGuid, sizeof(m_TargetGuid));
    oStream.Write((CHAR*)(&m_TargetNameSize), sizeof(BYTE));

    if( m_TargetNameSize > 0 )
    {
        oStream.Write((CHAR*)m_TargetName, m_TargetNameSize);
    }
}

VOID _GUILD_PACKET::Read(SocketInputStream& iStream)
{
	GUILD_PACKET*		pGuildPacket;

	iStream.Read( (CHAR*)(&m_uPacketType), sizeof(m_uPacketType) );

	pGuildPacket = GetPacket(m_uPacketType);
	if( pGuildPacket == NULL )
	{
		Assert( FALSE );
		return;
	}

	pGuildPacket->Read(iStream);
}

VOID _GUILD_PACKET::Write(SocketOutputStream& oStream) const
{
	GUILD_PACKET*		pGuildPacket;

	oStream.Write( (CHAR*)(&m_uPacketType), sizeof(m_uPacketType) );

	pGuildPacket = GetPacket(m_uPacketType);
	if( pGuildPacket == NULL )
	{
		Assert( FALSE );
		return;
	}

	pGuildPacket->Write(oStream);
}

VOID _GUILD_RETURN::Read(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

	switch( m_ReturnType )
	{
	case GUILD_RETURN_CREATE:
	case GUILD_RETURN_JOIN:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}
		}
		break;
	case GUILD_RETURN_RESPONSE:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}
		}
		break;
	case GUILD_RETURN_EXPEL:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_REJECT:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_RECRUIT:
		{
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}

		}
		break;
	case GUILD_RETURN_LEAVE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

		}
		break;
	case GUILD_RETURN_FOUND:
	case GUILD_RETURN_DISMISS:
    case GUILD_RETURN_ALREADY_IN:
    case GUILD_RETURN_ERASE_ITEM_FAIL:
		{
		}
		break;
	case GUILD_RETURN_PROMOTE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_PosID, sizeof(m_PosID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_DEMOTE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_PosID, sizeof(m_PosID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_AUTHORIZE:
	case GUILD_RETURN_DEPRIVE_AUTHORITY:
		{
		}
		break;
	case GUILD_RETURN_DEMISE:
		{
			iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
			iStream.Read((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
			iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			iStream.Read((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			iStream.Read((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));
			iStream.Read((CHAR*)&m_ChangedPositionNameSize, sizeof(m_ChangedPositionName));
			
			if( m_SourNameSize > 0 )
			{
				iStream.Read((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				iStream.Read((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_PositionName, m_PositionNameSize);
			}

			if( m_ChangedPositionNameSize > 0 )
			{
				iStream.Read((CHAR*)m_ChangedPositionName, m_ChangedPositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_WITHDRAW:
	case GUILD_RETURN_DEPOSIT:
		{
		}
		break;
	case GUILD_RETURN_CREATE_FAILED:
		{

		}
		break;
    case GUILD_RETURN_INVIT:
        {
            iStream.Read((CHAR*)&m_GuildID, sizeof(m_GuildID));
            iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));

            iStream.Read((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
            if( m_GuildNameSize > 0 )
            {
                iStream.Read((CHAR*)m_GuildName, m_GuildNameSize);
            }
        }
        break;
    case GUILD_RETURN_CREATE_FAMILY:
        {
            iStream.Read((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            iStream.Read((CHAR*)&m_FamilyNameSize, sizeof(m_FamilyNameSize));
            if (m_FamilyNameSize>0)
            {
                iStream.Read((CHAR*)&m_FamilyName, m_FamilyNameSize);
            }
            iStream.Read((CHAR*)&m_FamilyDescSize, sizeof(m_FamilyDescSize));
            if (m_FamilyDescSize>0)
            {
                iStream.Read((CHAR*)&m_FamilyDesc, m_FamilyDescSize);
            }
        }
        break;

    case GUILD_RETURN_FAMILY_DISMISS:
        {
            iStream.Read((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
        }
        break;

    case GUILD_RETURN_LEVEL_UP:
        {
            iStream.Read((CHAR*)&m_GuildLevel, sizeof(m_GuildLevel));
            iStream.Read((CHAR*)&m_bScuess, sizeof(m_bScuess));
        }
        break;

    case GUILD_RETURN_ADD_FAMILY_USER:
        {
            iStream.Read((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                iStream.Read((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;

    case GUILD_RETURN_BEADD_FAMILY:
        {
            iStream.Read((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
        }
        break;

    case GUILD_RETURN_REMOVE_FAMILY_USER:
        {
            iStream.Read((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                iStream.Read((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;
    case GUILD_RETURN_MODIFY_DESC:
        {
            iStream.Read((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                iStream.Read((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID _GUILD_RETURN::Write(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

	switch( m_ReturnType )
	{
	case GUILD_RETURN_CREATE:
	case GUILD_RETURN_JOIN:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}
		}
		break;
	case GUILD_RETURN_RESPONSE:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}
		}
		break;
	case GUILD_RETURN_EXPEL:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_REJECT:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_RECRUIT:
		{
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}

		}
		break;
	case GUILD_RETURN_LEAVE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}
		}
		break;
	case GUILD_RETURN_FOUND:
	case GUILD_RETURN_DISMISS:
    case GUILD_RETURN_ALREADY_IN:
    case GUILD_RETURN_ERASE_ITEM_FAIL:
		{
		}
		break;
	case GUILD_RETURN_PROMOTE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_PosID, sizeof(m_PosID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_DEMOTE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_PosID, sizeof(m_PosID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_AUTHORIZE:
	case GUILD_RETURN_DEPRIVE_AUTHORITY:
		{
		}
		break;
	case GUILD_RETURN_DEMISE:
		{
			oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
			oStream.Write((CHAR*)&m_GUIDChanged, sizeof(m_GUIDChanged));
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
			oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
			oStream.Write((CHAR*)&m_DestNameSize, sizeof(m_DestNameSize));
			oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
			oStream.Write((CHAR*)&m_PositionNameSize, sizeof(m_PositionNameSize));
			oStream.Write((CHAR*)&m_ChangedPositionNameSize, sizeof(m_ChangedPositionName));

			if( m_SourNameSize > 0 )
			{
				oStream.Write((CHAR*)m_SourName, m_SourNameSize);
			}

			if( m_DestNameSize > 0 )
			{
				oStream.Write((CHAR*)m_DestName, m_DestNameSize);
			}

			if( m_GuildNameSize > 0 )
			{
				oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
			}

			if( m_PositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_PositionName, m_PositionNameSize);
			}

			if( m_ChangedPositionNameSize > 0 )
			{
				oStream.Write((CHAR*)m_ChangedPositionName, m_ChangedPositionNameSize);
			}
		}
		break;
	case GUILD_RETURN_WITHDRAW:
	case GUILD_RETURN_DEPOSIT:
		{
		}
		break;
	case GUILD_RETURN_CREATE_FAILED:
		{

		}
		break;
    case GUILD_RETURN_INVIT:
        {
			oStream.Write((CHAR*)&m_GuildID, sizeof(m_GuildID));
            oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));

            oStream.Write((CHAR*)&m_GuildNameSize, sizeof(m_GuildNameSize));
            if( m_GuildNameSize > 0 )
            {
                oStream.Write((CHAR*)m_GuildName, m_GuildNameSize);
            }

        }
        break;
    case GUILD_RETURN_CREATE_FAMILY:
        {
            oStream.Write((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            oStream.Write((CHAR*)&m_FamilyNameSize, sizeof(m_FamilyNameSize));
            if (m_FamilyNameSize>0)
            {
                oStream.Write((CHAR*)&m_FamilyName, m_FamilyNameSize);
            }
            oStream.Write((CHAR*)&m_FamilyDescSize, sizeof(m_FamilyDescSize));
            if (m_FamilyDescSize>0)
            {
                oStream.Write((CHAR*)&m_FamilyDesc, m_FamilyDescSize);
            }
        }
        break;

    case GUILD_RETURN_FAMILY_DISMISS:
        {
            oStream.Write((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
        }
        break;

    case GUILD_RETURN_LEVEL_UP:
        {
            oStream.Write((CHAR*)&m_GuildLevel, sizeof(m_GuildLevel));
            oStream.Write((CHAR*)&m_bScuess, sizeof(m_bScuess));
        }
        break;

    case GUILD_RETURN_ADD_FAMILY_USER:
        {
            oStream.Write((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                oStream.Write((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;

    case GUILD_RETURN_BEADD_FAMILY:
        {
            oStream.Write((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
        }
        break;

    case GUILD_RETURN_REMOVE_FAMILY_USER:
        {
            oStream.Write((CHAR*)&m_cFamilyID, sizeof(m_cFamilyID));
            oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                oStream.Write((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;
    case GUILD_RETURN_MODIFY_DESC:
        {
            oStream.Write((CHAR*)&m_SourNameSize, sizeof(m_SourNameSize));
            if (m_SourNameSize>0)
            {
                oStream.Write((CHAR*)&m_SourName, m_SourNameSize);
            }
        }
        break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID MAIL::Read( SocketInputStream& iStream )
{
	iStream.Read((CHAR*)(&m_GUID),sizeof(m_GUID));
	iStream.Read((CHAR*)(&m_SourSize),sizeof(BYTE));
	if( m_SourSize>=MAX_CHARACTER_NAME )
	{
		Assert(FALSE) ;
		m_SourSize = MAX_CHARACTER_NAME ;
	}
	iStream.Read((CHAR*)(&m_szSourName),sizeof(CHAR)*m_SourSize);
	m_szSourName[MAX_CHARACTER_NAME-1] = 0 ;

	iStream.Read((CHAR*)(&m_nPortrait),sizeof(m_nPortrait));

	iStream.Read((CHAR*)(&m_DestSize),sizeof(BYTE));
	if( m_DestSize>=MAX_CHARACTER_NAME )
	{
		Assert(FALSE) ;
		m_DestSize = MAX_CHARACTER_NAME ;
	}
	iStream.Read((CHAR*)(&m_szDestName),sizeof(CHAR)*m_DestSize);
	m_szDestName[MAX_CHARACTER_NAME-1] = 0 ;

	iStream.Read((CHAR*)(&m_ContexSize),sizeof(WORD));
	if( m_ContexSize >= MAX_MAIL_CONTEX )
	{
		Assert(FALSE) ;
		m_ContexSize = MAX_MAIL_CONTEX ;
	}
	iStream.Read((CHAR*)(&m_szContex),sizeof(CHAR)*m_ContexSize);
	m_szContex[MAX_MAIL_CONTEX-1] = 0 ;

	iStream.Read((CHAR*)(&m_uFlag),sizeof(UINT));
	iStream.Read((CHAR*)(&m_uCreateTime),sizeof(m_uCreateTime));

	if( m_uFlag==MAIL_TYPE_SCRIPT )
	{
		iStream.Read((CHAR*)(&m_uParam0),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam1),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam2),sizeof(UINT));
		iStream.Read((CHAR*)(&m_uParam3),sizeof(UINT));
	}
}

VOID MAIL::Write( SocketOutputStream& oStream ) const
{
	oStream.Write((CHAR*)(&m_GUID),sizeof(m_GUID));
	Assert( m_SourSize<MAX_CHARACTER_NAME ) ;
	oStream.Write((CHAR*)(&m_SourSize),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_szSourName),sizeof(CHAR)*m_SourSize);

	oStream.Write((CHAR*)(&m_nPortrait),sizeof(m_nPortrait));

	Assert( m_DestSize<MAX_CHARACTER_NAME ) ;
	oStream.Write((CHAR*)(&m_DestSize),sizeof(BYTE));
	oStream.Write((CHAR*)(&m_szDestName),sizeof(CHAR)*m_DestSize);

	Assert( m_ContexSize<MAX_MAIL_CONTEX ) ;
	oStream.Write((CHAR*)(&m_ContexSize),sizeof(WORD));
	oStream.Write((CHAR*)(&m_szContex),sizeof(CHAR)*m_ContexSize);

	oStream.Write((CHAR*)(&m_uFlag),sizeof(UINT));
	oStream.Write((CHAR*)(&m_uCreateTime),sizeof(m_uCreateTime));

	if( m_uFlag==MAIL_TYPE_SCRIPT )
	{
		oStream.Write((CHAR*)(&m_uParam0),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam1),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam2),sizeof(UINT));
		oStream.Write((CHAR*)(&m_uParam3),sizeof(UINT));
	}
}


VOID MAIL_LIST::Read( SocketInputStream& iStream )
{
	iStream.Read((CHAR*)(&m_Count),sizeof(BYTE));
	if( m_Count>MAX_MAIL_SIZE )
	{
		Assert(FALSE) ;
		m_Count = MAX_MAIL_SIZE ;
	}
	INT i=0;
	for( i=0; i<m_Count; i++ )
	{
		m_aMail[i].Read( iStream ) ;
	}
	iStream.Read((CHAR*)(&m_TotalLeft),sizeof(BYTE));
}

VOID MAIL_LIST::Write( SocketOutputStream& oStream ) const
{
	Assert( m_Count<=MAX_MAIL_SIZE ) ;
	oStream.Write((CHAR*)(&m_Count),sizeof(BYTE));
	INT i=0;
	for( i=0; i<m_Count; i++ )
	{
		m_aMail[i].Write( oStream ) ;
	}
	oStream.Write((CHAR*)(&m_TotalLeft),sizeof(BYTE));
}

VOID BATCH_MAIL::CleanUp()
{
	m_GUID = INVALID_ID;

	m_SourSize = 0;
	memset( m_szSourName, 0, sizeof(m_szSourName) );

	m_ReceiverCount = 0;
	memset( m_Receivers, 0, sizeof(m_Receivers) );

	m_ContentSize = 0;
	memset( m_szContent, 0, sizeof(m_szContent) );

	m_uFlag = MAIL_TYPE_NORMAL2;
	m_uCreateTime = 0;
}

UINT BATCH_MAIL::GetSize() const
{
	UINT uSize;

	uSize = sizeof(m_GUID) + sizeof(m_SourSize) + m_SourSize + sizeof(m_ReceiverCount);

	if ( m_ReceiverCount > 0 )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			uSize += sizeof(m_Receivers[i].m_DestSize) + m_Receivers[i].m_DestSize;
		}
	}

	uSize += sizeof(m_ContentSize) + m_ContentSize;
	uSize += sizeof(m_uFlag) + sizeof(m_uCreateTime);

	return uSize;
}

VOID BATCH_MAIL::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	iStream.Read( (CHAR*)(&m_SourSize), sizeof(m_SourSize) );
	if( m_SourSize > 0 && m_SourSize < MAX_CHARACTER_NAME )
	{
		iStream.Read( (CHAR*)(&m_szSourName), m_SourSize );
	}
	else
	{
		m_SourSize = 0;
	}

	iStream.Read( (CHAR*)(&m_ReceiverCount), sizeof(m_ReceiverCount) );
	if( m_ReceiverCount > 0 && m_ReceiverCount < MAX_RECEIVER )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			iStream.Read( (CHAR*)(&m_Receivers[i].m_DestSize), sizeof(m_Receivers[i].m_DestSize) );

			if( m_Receivers[i].m_DestSize > 0 && m_Receivers[i].m_DestSize < MAX_CHARACTER_NAME )
			{
				iStream.Read( (CHAR*)(&m_Receivers[i].m_szDestName), m_Receivers[i].m_DestSize );
			}
			else
			{
				m_Receivers[i].m_DestSize = 0;
			}
		}
	}
	else
	{
		m_ReceiverCount = 0;
	}

	iStream.Read( (CHAR*)(&m_ContentSize), sizeof(m_ContentSize) );
	if( m_ContentSize > 0 && m_ContentSize < MAX_MAIL_CONTEX )
	{
		iStream.Read( (CHAR*)(&m_szContent), m_ContentSize );
	}
	else
	{
		m_ContentSize = 0;
	}

	iStream.Read( (CHAR*)(&m_uFlag), sizeof(m_uFlag) );
	iStream.Read( (CHAR*)(&m_uCreateTime), sizeof(m_uCreateTime) );
}

VOID BATCH_MAIL::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	oStream.Write( (CHAR*)(&m_SourSize), sizeof(m_SourSize) );
	if( m_SourSize > 0 && m_SourSize < MAX_CHARACTER_NAME )
	{
		oStream.Write( (CHAR*)(&m_szSourName), m_SourSize );
	}

	oStream.Write( (CHAR*)(&m_ReceiverCount), sizeof(m_ReceiverCount) );
	if( m_ReceiverCount > 0 && m_ReceiverCount < MAX_RECEIVER )
	{
		for( INT i=0; i<m_ReceiverCount; ++i )
		{
			oStream.Write( (CHAR*)(&m_Receivers[i].m_DestSize), sizeof(m_Receivers[i].m_DestSize) );

			if( m_Receivers[i].m_DestSize > 0 && m_Receivers[i].m_DestSize < MAX_CHARACTER_NAME )
			{
				oStream.Write( (CHAR*)(&m_Receivers[i].m_szDestName), m_Receivers[i].m_DestSize );
			}
		}
	}

	oStream.Write( (CHAR*)(&m_ContentSize), sizeof(m_ContentSize) );
	if( m_ContentSize > 0 && m_ContentSize < MAX_MAIL_CONTEX )
	{
		oStream.Write( (CHAR*)(&m_szContent), m_ContentSize );
	}

	oStream.Write( (CHAR*)(&m_uFlag), sizeof(m_uFlag) );
	oStream.Write( (CHAR*)(&m_uCreateTime), sizeof(m_uCreateTime) );
}


VOID	DB_QUERY::Parse(const CHAR* pTemplate,...)
{
	va_list argptr;
	va_start(argptr, pTemplate);
	int nchars  = tvsnprintf((char*)m_SqlStr, MAX_SQL_LENGTH, pTemplate, argptr );
	va_end(argptr);

	if (nchars == -1 || nchars > MAX_SQL_LENGTH )
	{
		Assert(FALSE);
		return;
	}

}


UINT TEAM_LIST_ENTRY::GetSize() const
{
	return sizeof(m_GUID) + sizeof(m_SceneID) + sizeof(m_SceneResID) + sizeof(m_ExtraID) + sizeof(m_NameSize)
		 + sizeof(CHAR) * m_NameSize + sizeof(m_nPortrait) + sizeof(m_uDataID) + sizeof(m_uFamily);
}

VOID TEAM_LIST_ENTRY::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
	iStream.Read( (CHAR*)&m_SceneResID, sizeof(m_SceneResID) );	
	iStream.Read( (CHAR*)&m_ExtraID, sizeof(m_ExtraID) );
	iStream.Read( (CHAR*)&m_NameSize, sizeof(m_NameSize) );

	if ( m_NameSize>0 )
	{
		iStream.Read( (CHAR*)m_Name, sizeof(CHAR) * m_NameSize );
	}

	iStream.Read( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	iStream.Read( (CHAR*)&m_uDataID, sizeof(m_uDataID) );
	iStream.Read( (CHAR*)&m_uFamily, sizeof(m_uFamily) );
}

VOID TEAM_LIST_ENTRY::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
	oStream.Write( (CHAR*)&m_SceneResID, sizeof(m_SceneResID) );	
	oStream.Write( (CHAR*)&m_ExtraID, sizeof(m_ExtraID) );
	oStream.Write( (CHAR*)&m_NameSize, sizeof(m_NameSize) );

	if ( m_NameSize>0 )
	{
		oStream.Write( (CHAR*)m_Name, sizeof(CHAR) * m_NameSize );
	}

	oStream.Write( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	oStream.Write( (CHAR*)&m_uDataID, sizeof(m_uDataID) );
	oStream.Write( (CHAR*)&m_uFamily, sizeof(m_uFamily) );
}


//////////////////////////////////////////////////////////////////////////
//请求当国王
//////////////////////////////////////////////////////////////////////////
VOID COUNTRY_CGW_KINDOM_REQUEST::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_Guid), sizeof(GUID_t));
}

VOID COUNTRY_CGW_KINDOM_REQUEST::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_Guid), sizeof(GUID_t));
}

//////////////////////////////////////////////////////////////////////////
//任命国家官职
//////////////////////////////////////////////////////////////////////////
VOID COUNTRY_CGW_COUNTRY_APPOINT::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_KindomGuid), sizeof(GUID_t));
    iStream.Read((CHAR*)(&m_cNameSize), sizeof(CHAR));
    iStream.Read((CHAR*)(&m_nOfficialPosition), sizeof(INT));
    if (m_cNameSize>0)
    {
        iStream.Read((CHAR*)(m_szTargetName), m_cNameSize);
    }
}

VOID COUNTRY_CGW_COUNTRY_APPOINT::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_KindomGuid), sizeof(GUID_t));
    oStream.Write((CHAR*)(&m_cNameSize), sizeof(CHAR));
    oStream.Write((CHAR*)(&m_nOfficialPosition), sizeof(INT));
    if (m_cNameSize)
    {
        oStream.Write((CHAR*)(m_szTargetName), m_cNameSize);
    }
}

//////////////////////////////////////////////////////////////////////////
//罢免国家官职
//////////////////////////////////////////////////////////////////////////
VOID COUNTRY_CGW_COUNTRY_REMOVE::Read(SocketInputStream& iStream)
{
    iStream.Read((CHAR*)(&m_KindomGuid), sizeof(GUID_t));
    iStream.Read((CHAR*)(&m_cNameSize), sizeof(CHAR));
    if (m_cNameSize>0)
    {
        iStream.Read((CHAR*)(m_szTargetName), m_cNameSize);
    }
    iStream.Read((CHAR*)(&m_nOfficialPosition), sizeof(m_nOfficialPosition));
}

VOID COUNTRY_CGW_COUNTRY_REMOVE::Write(SocketOutputStream& oStream) const
{
    oStream.Write((CHAR*)(&m_KindomGuid), sizeof(GUID_t));
    oStream.Write((CHAR*)(&m_cNameSize), sizeof(CHAR));
    if (m_cNameSize)
    {
        oStream.Write((CHAR*)(m_szTargetName), m_cNameSize);
    }
    oStream.Write((CHAR*)(&m_nOfficialPosition), sizeof(m_nOfficialPosition));
}

//////////////////////////////////////////////////////////////////////////
//国家相关返回值
//////////////////////////////////////////////////////////////////////////
VOID _COUNTRY_RETURN::Read( SocketInputStream& iStream )
{
    iStream.Read((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

    switch( m_ReturnType )
    {
    case COUNTRY_RETURN_REQUEST_KINDOM:
        break;
    case COUNTRY_RETURN_APPOINT:
        {
            iStream.Read((CHAR*)(&m_cPosition), sizeof(m_cPosition));
            iStream.Read((CHAR*)(&m_cCountry), sizeof(m_cCountry));
            iStream.Read((CHAR*)(m_szName), MAX_CHARACTER_NAME);
        }
        break;
    case COUNTRY_RETURN_REMOVE:
        {
            iStream.Read((CHAR*)(&m_cPosition), sizeof(m_cPosition));
            iStream.Read((CHAR*)(&m_cCountry), sizeof(m_cCountry));
        }
        break;
    case COUNTRY_ERROR_COUNTRY_ID:
        {
        }
        break;
    case COUNTRY_ERROR_USER_NOT_FIND:
        {
        }
        break;
    case COUNTRY_RETURN_DELATE_SCUESS:
        break;
    case COUNTRY_RETURN_DELATE:
        break;
    case COUNTRY_RETURN_REQUEST_KINDOM_FAIL:
        break;
    }
}

VOID _COUNTRY_RETURN::Write( SocketOutputStream& oStream ) const
{
    oStream.Write((CHAR*)&m_ReturnType, sizeof(m_ReturnType));

    switch( m_ReturnType )
    {
    case COUNTRY_RETURN_REQUEST_KINDOM:
        break;
    case COUNTRY_RETURN_APPOINT:
        {
            oStream.Write((CHAR*)(&m_cPosition), sizeof(m_cPosition));
            oStream.Write((CHAR*)(&m_cCountry), sizeof(m_cCountry));
            oStream.Write((CHAR*)(m_szName), MAX_CHARACTER_NAME);
        }
        break;
    case COUNTRY_RETURN_REMOVE:
        {
            oStream.Write((CHAR*)(&m_cPosition), sizeof(m_cPosition));
            oStream.Write((CHAR*)(&m_cCountry), sizeof(m_cCountry));
        }
        break;
    case COUNTRY_ERROR_COUNTRY_ID:
        {
        }
        break;
    case COUNTRY_ERROR_USER_NOT_FIND:
        {
        }
        break;
    case COUNTRY_RETURN_DELATE_SCUESS:
        break;
    case COUNTRY_RETURN_DELATE:
        break;
    case COUNTRY_RETURN_REQUEST_KINDOM_FAIL:
        break;

    }
}


//////////////////////////////////////////////////////////////////////////
//国家官员列表
//////////////////////////////////////////////////////////////////////////
VOID COUNTRY_CGW_COUNTRY_INFO::Read( SocketInputStream& iStream )
{
    iStream.Read((CHAR*)&m_CountryID, sizeof(m_CountryID));
    iStream.Read((CHAR*)&m_CountryInfo, sizeof(_COUNTRY));
}

VOID COUNTRY_CGW_COUNTRY_INFO::Write( SocketOutputStream& oStream ) const
{
    oStream.Write((CHAR*)&m_CountryID, sizeof(m_CountryID));
    oStream.Write((CHAR*)&m_CountryInfo, sizeof(_COUNTRY));
}

//////////////////////////////////////////////////////////////////////////
//国王募集
//////////////////////////////////////////////////////////////////////////
VOID COUNTRY_CGW_COUNTRY_COLLECT::Read( SocketInputStream& iStream )
{
    iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));
    iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));
    iStream.Read((CHAR*)&m_cKindomSize, sizeof(m_cKindomSize));
    iStream.Read((CHAR*)&m_cDescSize, sizeof(m_cDescSize));

    if (m_cKindomSize > 0)
    {
        iStream.Read((CHAR*)&m_szKindom, m_cKindomSize);
    }

    if (m_cDescSize > 0)
    {
        iStream.Read((CHAR*)&m_szDesc, m_cDescSize);
    }
}

VOID COUNTRY_CGW_COUNTRY_COLLECT::Write( SocketOutputStream& oStream ) const
{
    oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));
    oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));
    oStream.Write((CHAR*)&m_cKindomSize, sizeof(m_cKindomSize));
    oStream.Write((CHAR*)&m_cDescSize, sizeof(m_cDescSize));

    if (m_cKindomSize > 0)
    {
        oStream.Write((CHAR*)&m_szKindom, m_cKindomSize);
    }

    if (m_cDescSize > 0)
    {
        oStream.Write((CHAR*)&m_szDesc, m_cDescSize);
    }
}

VOID	LONG_DB_QUERY::Parse(const CHAR* pTemplate,...)
{
	va_list argptr;
	va_start(argptr, pTemplate);
	int nchars  = tvsnprintf((char*)m_SqlStr, MAX_LONG_SQL_LENGTH, pTemplate, argptr );
	va_end(argptr);
	if (nchars == -1 || nchars >= MAX_LONG_SQL_LENGTH )
	{
		Assert(FALSE);
		return;
	}

}
//add by gh for souxia message 2010/06/01 分开写可以减少传输的数据
VOID SOUXIA_DATA::ReadSouXiaVarAttr(SocketInputStream& iStream)
{
	iStream.Read((CHAR*)(&m_CurPos),sizeof(SHORT));
	iStream.Read((CHAR*)(&m_SouXiaID),sizeof(UINT));

	iStream.Read((CHAR*)(& (m_Skill.StudyCount) ),sizeof(BYTE));
	for (int i=0; i<m_Skill.StudyCount; ++i)
	{
		iStream.Read((CHAR*)( &(m_Skill.StudySkillId[i]) ),sizeof(SkillID_t));
	}

	iStream.Read((CHAR*)(& (m_Product.StudyCount) ),sizeof(BYTE));
	for (int i=0; i<m_Product.StudyCount; ++i)
	{
		iStream.Read((CHAR*)( &(m_Product.StudyProductId[i]) ),sizeof(SkillID_t));
	}

	iStream.Read((CHAR*)(& (m_Pet.StudyCount) ),sizeof(BYTE));
	for (int i=0; i<m_Pet.StudyCount; ++i)
	{
		iStream.Read((CHAR*)( &(m_Pet.StudyPet[i].StudyZhaoHuan) ),sizeof(SkillID_t));
		iStream.Read((CHAR*)( &(m_Pet.StudyPet[i].LeftUseTime) ),sizeof(SHORT));
	}

	iStream.Read((CHAR*)(& (m_ZuoJi.StudyCount) ),sizeof(BYTE));
	for (int i=0; i<m_ZuoJi.StudyCount; ++i)
	{
		iStream.Read((CHAR*)( &(m_ZuoJi.StudyZuoji[i].StudyZhaoHuan) ),sizeof(SkillID_t));
		iStream.Read((CHAR*)( &(m_ZuoJi.StudyZuoji[i].LeftUseTime) ),sizeof(SHORT));
	}

}

VOID SOUXIA_DATA::WriteSouXiaVarAttr(SocketOutputStream& oStream) const
{
	oStream.Write((CHAR*)(&m_CurPos),sizeof(SHORT));
	oStream.Write((CHAR*)(&m_SouXiaID),sizeof(UINT));

	if (m_CurPos >= 0 && m_SouXiaID > 0)
	{
		oStream.Write((CHAR*)(& (m_Skill.StudyCount) ),sizeof(BYTE));
		for (int i=0; i<m_Skill.StudyCount; ++i)
		{
			oStream.Write((CHAR*)( &(m_Skill.StudySkillId[i]) ),sizeof(SkillID_t));
		}

		oStream.Write((CHAR*)(& (m_Product.StudyCount) ),sizeof(BYTE));
		for (int i=0; i<m_Product.StudyCount; ++i)
		{
			oStream.Write((CHAR*)( &(m_Product.StudyProductId[i]) ),sizeof(SkillID_t));
		}

		oStream.Write((CHAR*)(& (m_Pet.StudyCount) ),sizeof(BYTE));
		for (int i=0; i<m_Pet.StudyCount; ++i)
		{
			oStream.Write((CHAR*)( &(m_Pet.StudyPet[i].StudyZhaoHuan) ),sizeof(SkillID_t));
			oStream.Write((CHAR*)( &(m_Pet.StudyPet[i].LeftUseTime) ),sizeof(SHORT));
		}

		oStream.Write((CHAR*)(& (m_ZuoJi.StudyCount) ),sizeof(BYTE));
		for (int i=0; i<m_ZuoJi.StudyCount; ++i)
		{
			oStream.Write((CHAR*)( &(m_ZuoJi.StudyZuoji[i].StudyZhaoHuan) ),sizeof(SkillID_t));
			oStream.Write((CHAR*)( &(m_ZuoJi.StudyZuoji[i].LeftUseTime) ),sizeof(SHORT));
		}
	}
			
}
