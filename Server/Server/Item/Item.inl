/////////////////////////////////////////////////////////////////////////////////
//文件名：Item.inl
//功能描述：物品数据读取写入的inline函数
//开发者：jack
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_INL__
#define __ITEM_INL__
#include "DB_Struct.h"
#include "GameTable.h"
#include "ItemTable.h"
#include "TimeManager.h"

inline _ITEM_GUID Item::GetGUID( )
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemGUID ;

	__LEAVE_FUNCTION

	_ITEM_GUID	ig;
	return	ig;
};

inline UINT Item::GetItemTableIndex( ) const//取得物品表里的物品索引编号
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemIndex ;

	__LEAVE_FUNCTION

	return	-1;
}

inline UCHAR Item::GetItemClass()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->ItemClass();

	__LEAVE_FUNCTION

	return	0;

}

//inline UCHAR	Item::GetItemQual() const
//{
//	__ENTER_FUNCTION
//
//	Assert(m_pItem);
//	return	m_pItem->GetQual();
//
//	__LEAVE_FUNCTION
//
//	return	0;
//}


inline UCHAR	Item::GetItemType() const
{
	__ENTER_FUNCTION
	
	Assert(m_pItem);
	return	m_pItem->ItemType();

	__LEAVE_FUNCTION

	return	0;
}

inline UCHAR	Item::GetItemIndex() const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->GetIndex();

	__LEAVE_FUNCTION

	return	0;
}

inline INT Item::GetLayedNum( ) const//取得叠放物品的数量
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemCount();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}


	__LEAVE_FUNCTION

	return 0 ;
}


inline BOOL Item::IsPWLock() const
{
	__ENTER_FUNCTION
	switch(m_Type)
	{
		case IDT_ITEM:
			{
				Assert( m_pItem ) ;
				return m_pItem->GetItemPLock();
			}
			break;
		case IDT_PET:
			{
				Assert(m_pPet);
				return 1;
			}
			break;
		default:
			{
				Assert(FALSE);
				return 0;
			}
	}

	__LEAVE_FUNCTION
	return TRUE;
}

inline INT Item::GetMaxLayedNum( ) const//取得能够叠放物品的最大数量
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemTileMax();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL	Item::IsFull() const
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert(m_pItem);
			return m_pItem->isFullTile();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return !m_pPet->m_GUID.IsNull();
		}
		break;
	default:
		{
			Assert(FALSE);
			return FALSE;
		}
	}

	__LEAVE_FUNCTION

	return FALSE;
}



//----------------------------------------------------------------------------------
//装备

_ITEM*				Item::GetItemData()			const				//获得装备的所有属性
{
	return m_pItem;
}


INT					Item::GetSetNum()			const				//套装编号
{
	Assert(m_pItem);
	if(m_pItem->m_ItemIndex==0)
	{
		return 0;
	}
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_EquipSetNum;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return 0;
	}
}
BYTE					Item::GetMaxSetCount()			const				//套装个数
{
	Assert(m_pItem);
	if(m_pItem->m_ItemIndex==0)
	{
		return 0;
	}
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_EquipSetMaxNum;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return 0;
	}
	
}
BYTE					Item::GetEquipPoint()		const				//物品装佩点
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_EquipPoint;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return -1;

	}
	
}

INT					Item::GetMaxDurPoint()		const			//最大耐久值
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_MaxDur;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return -1;
	}
}
BYTE					Item::GetRequireLevel()		const			//需求等级
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_NeedLevel;
}

BYTE					Item::GetCurLevel()		const			//需求等级
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_Level;
}

BYTE					Item::GetGemSlotMax()		const			//最大宝石个数
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_MAXGemHole;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return -1;
	}
}

INT					Item::GetDurPoints()		const 				//当前耐久值
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDurPoint;
}

INT					Item::GetCurMaxDurPoint()		const 				//当前耐久上限
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurMaxDur;
}

USHORT					Item::GetDamagePoint()	const
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDamagePoint;
}

BOOL					Item::GetItemBind()	const
{
	Assert(m_pItem);
	return m_pItem->GetItemBind();
}

BOOL					Item::GetItemIdent()	const
{
	Assert(m_pItem);

	if(m_pItem->ItemClass()==ICLASS_EQUIP/*&&m_pItem->GetQual() == EQUALITY_BLUE*/)
	{
		return m_pItem->GetItemIdent();
	}
	return TRUE;
	
}

BYTE					Item::GetAttrCount()		const				//属性的数量
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_AttrCount;
}

BYTE					Item::GetEquipGemCount()	const				//镶嵌宝石的数量
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_StoneCount;
}

BYTE					Item::GetAffixSkillCount()		const			//获得附加技能个数
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_AffSkillCount;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return 0;
	}
}

BYTE					Item::GetEffectSkillCount()		const			//获得装备影响技能个数
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		return pEquipTable->m_EffSkillCount;
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return 0;
	}
}

_ITEM_EFFECT_SKILL*		Item::GetEffectSkill(UINT pos)		const			//获得装备影响技能个数
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);
	if(pEquipTable)
	{
		if (pos>=pEquipTable->m_EffSkillCount)
		{
			return NULL;
		}
		return &pEquipTable->m_EffectSkill[pos];
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return NULL;
	}
}

BOOL					Item::IsWuMenPai()				const	//获得装备是否无门派需求
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);

	if(pEquipTable)
	{
		return (pEquipTable->m_PrefessionReq[0] == MATTRIBUTE_WUMENPAI);
	}
	else
	{
		AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
		return TRUE;
	}

	__LEAVE_FUNCTION

	return TRUE;
}

BOOL				Item::InReqJob(MENPAI_ATTRIBUTE menpaiID)	const	//指定的门派是否与装备的门派需求一致
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	Assert(menpaiID >= MATTRIBUTE_SHAOLIN_GWS && menpaiID <= MATTRIBUTE_WUMENPAI);

	if (menpaiID >= MATTRIBUTE_SHAOLIN_GWS && menpaiID <= MATTRIBUTE_WUMENPAI)
	{
		EQUIP_TB* pEquipTable = g_ItemTable.GetEquipTB(m_pItem->m_ItemIndex);

		if(pEquipTable)
		{
			for (int i = 0; i < MATTRIBUTE_NUMBER; i++)
			{
				if (pEquipTable->m_PrefessionReq[i] == menpaiID)
				{
					return TRUE;
				}
			}
			return FALSE;
		}
		else
		{
			AssertEx(FALSE,"物品运行时数据出错!或者物品表有问题");
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

	return FALSE;
}

BYTE					Item::GetCurGemHole()		const			//最大宝石个数
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	return m_pItem->GetEquipData()->m_CurGemHole;
}

BOOL					Item::AddHole()
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	if(m_pItem->GetEquipData()->m_CurGemHole == MAX_GEM_HOLE)
		return FALSE;
	++m_pItem->GetEquipData()->m_CurGemHole;
	return TRUE;
}

BYTE					Item::GetEquipBindLevel()
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	return m_pItem->GetEquipData()->m_Bind;
}

VOID					Item::EquipBind(UINT nBindLevel)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	
	Assert(nBindLevel<=MAX_EQUIP_BIND_LEVEL);
	m_pItem->GetEquipData()->m_Bind = nBindLevel;
}


BOOL					Item::EquipLevelup(INT nLevel)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	INT nFinalLevel = m_pItem->GetEquipData()->m_Level + nLevel;

	if((nFinalLevel > MAX_EQUIP_LEVELUP_NUM)||(nFinalLevel<0))
	{
		Assert(0);
	}
	m_pItem->GetEquipData()->m_Level = nFinalLevel;
	
	EQUIP_TB* pGET	=	g_ItemTable.GetEquipTB(GetItemTableIndex());
	Assert(pGET);
	if (!pGET)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Item::EquipLevelup] GetEquipTB index is error [%d]",GetItemTableIndex());
		return FALSE;
	}
	
	if (m_pItem->GetEquipData()->m_Level == 0)
	{
		for (UINT i=0; i<m_pItem->GetEquipData()->m_AttrCount; ++i)
		{
			INT AttrType = m_pItem->GetEquipData()->m_pAttr[i].m_AttrType;
			switch(AttrType)
			{
			case IATTRIBUTE_BASE_ATTACK_NEAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_NearAttack;
				}
				break;
			case IATTRIBUTE_BASE_ATTACK_FAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_FarAttack;
				}
				break;
			case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicNearAttack;
				}
				break;
			case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicFarAttack;
				}
				break;
			case IATTRIBUTE_BASE_DEFENCE_NEAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_NearDefence;
				}
				break;
			case IATTRIBUTE_BASE_DEFENCE_FAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_FarDefence;
				}
				break;
			case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicNearDefense;
				}
				break;
			case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:
				{
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicFarDefense;
				}
				break;
			default:
				break;
			}
		}
		return TRUE;
	}
	EQUIP_LEVELUP_TB* pLevelupInfoTb = g_ItemTable.GetEquipLeveupTB(m_pItem->GetEquipData()->m_Level);
	Assert(pLevelupInfoTb);
	if (!pLevelupInfoTb)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Item::EquipLevelup] GetEquipLeveupTB index is error [%d]",m_pItem->GetEquipData()->m_Level);
		return FALSE;
	}

	for (UINT i=0; i<m_pItem->GetEquipData()->m_AttrCount; ++i)
	{
		INT AttrType = m_pItem->GetEquipData()->m_pAttr[i].m_AttrType;
		switch(AttrType)
		{
		case IATTRIBUTE_BASE_ATTACK_NEAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_NearAttack;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_ATTACK_FAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_FarAttack;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicNearAttack;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicFarAttack;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_NEAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_NearDefence;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_FAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_FarDefence;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicNearDefense;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:
			{
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value = pGET->m_MagicFarDefense;
				m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value += 
					m_pItem->GetEquipData()->m_pAttr[i].m_Value.m_Value*pLevelupInfoTb->m_EffectRate/100;
			}
			break;
		default:
			break;
		}
	}
	return TRUE;
}

UINT					Item::GetEquipScore()			const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_EquipScore;
}

BYTE					Item::GetEquipFromType()		    const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	return m_pItem->GetEquipData()->m_EquipType;
}

VOID				Item::SetEquipScore(UINT nScore)	const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);

	m_pItem->GetEquipData()->m_EquipScore = nScore;
}

VOID				Item::SetEquipFromType(BYTE nType) const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	Assert(nType<ITEM_NUM);
	m_pItem->GetEquipData()->m_EquipType = nType;
}

UINT					Item::GetEquipGem(INT nPos)	const				//镶嵌宝石的数量
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	Assert(nPos<MAX_ITEM_GEM);
	return m_pItem->GetEquipData()->m_pGemInfo[nPos].m_GemIndex;
}


BOOL					Item::AddGem(INT nHolePos, INT GemId) 
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	Assert(nHolePos<=m_pItem->GetEquipData()->m_CurGemHole);

	if(m_pItem->GetEquipData()->m_pGemInfo[nHolePos].m_GemIndex == INVALID_ITEM_ID)
		++m_pItem->GetEquipData()->m_StoneCount;
	m_pItem->GetEquipData()->m_pGemInfo[nHolePos].m_GemIndex = GemId;
	return TRUE;
}

VOID					Item::UpDataEquipGem() const
{
	//检验前置宝石
	INT GemTypeCount[GEM_TYPE_NUM];
	memset(GemTypeCount, 0, sizeof(INT)*GEM_TYPE_NUM);
	for(INT nGemPos=0; nGemPos<m_pItem->GetEquipData()->m_CurGemHole; ++nGemPos)
	{
		if (m_pItem->GetEquipData()->m_pGemInfo[nGemPos].m_GemIndex == INVALID_ITEM_ID)
		{
			continue;
		}
		switch(GetSerialType(m_pItem->GetEquipData()->m_pGemInfo[nGemPos].m_GemIndex))
		{
		case GEM_COLORFUL:
			++GemTypeCount[GEM_COLORFUL];
			break;
		case GEM_RED:
			++GemTypeCount[GEM_RED];
			break;
		case GEM_GREEN:
			++GemTypeCount[GEM_GREEN];
			break;
		case GEM_YELLOW:
			++GemTypeCount[GEM_YELLOW];
			break;
		case GEM_BLUE:
			++GemTypeCount[GEM_BLUE];
			break;
		default:
			break;
		}
	}

	for(INT i=0; i<m_pItem->GetEquipData()->m_CurGemHole; ++i)
	{
		if(m_pItem->GetEquipData()->m_pGemInfo[i].m_GemIndex != INVALID_ITEM_ID)
		{
			BOOL bEable = TRUE;
			GEMINFO_TB* pGET = g_ItemTable.GetGemInfoTB(m_pItem->GetEquipData()->m_pGemInfo[i].m_GemIndex);
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Item::UpDataEquipGem] GetGemInfoTB index is error [%d]",m_pItem->GetEquipData()->m_pGemInfo[i].m_GemIndex);
				return;
			}
			for(UINT j=0; j<pGET->m_GemReqCount; ++j)
			{
				if(GemTypeCount[pGET->m_GemReqAttr[j].m_GemType] < pGET->m_GemReqAttr[j].m_Value.m_Value)
				{
					bEable = FALSE;
					break;
				}
			}
			m_pItem->GetEquipData()->m_pGemInfo[i].m_bEnable = bEable;
		}
	}
}

const _ITEM_ATTR				Item::GetEquipAttr(UCHAR	nIndex)		const	//获得装备属性
{	
	Assert(m_pItem);
	Assert(nIndex<m_pItem->GetEquipData()->m_AttrCount);
	return m_pItem->GetEquipData()->m_pAttr[nIndex];
}

VOID							Item::SetEquipAttr(UCHAR	nIndex, _ITEM_ATTR ItemAttr)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	Assert(nIndex<m_pItem->GetEquipData()->m_AttrCount);
	m_pItem->GetEquipData()->m_pAttr[nIndex].m_AttrType = ItemAttr.m_AttrType;
	m_pItem->GetEquipData()->m_pAttr[nIndex].m_Value.m_Value = ItemAttr.m_Value.m_Value;
}

const _ITEM_GEMINFO			Item::GetEquipGemInfo(UCHAR nIndex)	const		//获得宝石信息
{
	Assert(m_pItem);
	Assert(nIndex<GetGemSlotMax());
	return m_pItem->GetEquipData()->m_pGemInfo[nIndex];
}

BYTE						Item::GetEquipPaiType()		    const			//获得装备牌型
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_PaiType;
}

VOID						Item::SetEquipPaiType(BYTE nPaiType)
{
	Assert(m_pItem);
	switch(nPaiType)
	{
	case 0:
	case EQUIPPAI_ANY:
	case EQUIPPAI_ANYFENG:
	case EQUIPPAI_ANYJIAN:
	case EQUIPPAI_ANYZI:
	case EQUIPPAI_ANYFEIZI:
	case EQUIPPAI_ANYTIAO:
	case EQUIPPAI_ANYTONG:
	case EQUIPPAI_ANYWAN:
	case EQUIPPAI_ANYSHUN:
	case EQUIPPAI_ANYKE:
	case EQUIPPAI_ANYJIANG:
	case EQUIPPAI_DONG:
	case EQUIPPAI_NAN:
	case EQUIPPAI_XI	:
	case EQUIPPAI_BEI:
	case EQUIPPAI_ZHONG:
	case EQUIPPAI_FA:
	case EQUIPPAI_BAI:
	case EQUIPPAI_1TIAO:
	case EQUIPPAI_2TIAO:
	case EQUIPPAI_3TIAO:
	case EQUIPPAI_4TIAO:
	case EQUIPPAI_5TIAO:
	case EQUIPPAI_6TIAO:
	case EQUIPPAI_7TIAO:
	case EQUIPPAI_8TIAO:
	case EQUIPPAI_9TIAO:
	case EQUIPPAI_1TONG:
	case EQUIPPAI_2TONG:
	case EQUIPPAI_3TONG:
	case EQUIPPAI_4TONG:
	case EQUIPPAI_5TONG:
	case EQUIPPAI_6TONG:
	case EQUIPPAI_7TONG:
	case EQUIPPAI_8TONG:
	case EQUIPPAI_9TONG:
	case EQUIPPAI_1WAN:
	case EQUIPPAI_2WAN:
	case EQUIPPAI_3WAN:
	case EQUIPPAI_4WAN:
	case EQUIPPAI_5WAN:
	case EQUIPPAI_6WAN:
	case EQUIPPAI_7WAN:
	case EQUIPPAI_8WAN:
	case EQUIPPAI_9WAN:
		m_pItem->GetEquipData()->m_PaiType = nPaiType;
		break;
	default:
		Assert(0);
	    break;
	}
}


BOOL						Item::IsAdsorbMagic()			const
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_SoulBeadAttrCount > 0 ? TRUE : FALSE;
}


BOOL						Item::AdsorbMagic(_ITEM* Bead, BYTE Kind )
{
	if ( (Kind != SOUL_BEAD_WU) && (Kind != SOUL_BEAD_QI) )
	{
		return FALSE;
	}

	if (Kind == SOUL_BEAD_WU)
	{

		m_pItem->m_Equip.m_SoulBeadType = SOUL_BEAD_WU;
		for ( INT i=0; i<MAX_WU_SOUL_BEAD_ATTR; ++i )
		{
			BOOL IsHold = FALSE;
			// 在装备中备份魂珠的属性
			m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_AttrType = Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType;
			m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_Value.m_Value = Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_Value.m_Value;
		
			for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
			{
				// 如果装备有相同类型的属性，则珠子属性的10%叠加到原属性上
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType)&&
					(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value += (INT)(Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_Value.m_Value/10);
					m_pItem->m_Equip.m_SoulBeadAttrCount++;
					IsHold = TRUE;
				}
			}
			// 如果是新属性，
			if ( (!IsHold) &&( Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType !=0 ) )
			{
				for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
				{
					// 如果是空位
					if ( m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value == 0 )
					{
						m_pItem->m_Equip.m_pAttr[j].m_AttrType = Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType;
						m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value = (INT)(Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_Value.m_Value/10);
						m_pItem->m_Equip.m_AttrCount++;
						m_pItem->m_Equip.m_SoulBeadAttrCount++;
						break;
					}
				}
			}
		}
	}
	else
	{
		m_pItem->m_Equip.m_SoulBeadType = SOUL_BEAD_QI;
		for (INT i=0; i<MAX_QI_SOUL_BEAD_ATTR; ++i)
		{
			BOOL IsHold = FALSE;
			// 在装备中备份魂珠的属性
			m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_AttrType = Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType;
			m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_Value.m_Value = Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_Value.m_Value;
		
			for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
			{
				// 如果装备有相同类型的属性，则珠子属性的10%叠加到原属性上
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType)&&
					(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value += (INT)(Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_Value.m_Value/10);
					m_pItem->m_Equip.m_SoulBeadAttrCount++;
					IsHold = TRUE;
				}
			}
			// 如果是新属性，
			if ( (!IsHold) &&(Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType != 0) )
			{
				for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
				{
					// 如果是空位
					if ( m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value == 0 )
					{
						m_pItem->m_Equip.m_pAttr[j].m_AttrType = Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType;
						m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value = (INT)(Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_Value.m_Value/10);
						m_pItem->m_Equip.m_AttrCount++;
						m_pItem->m_Equip.m_SoulBeadAttrCount++;
						break;
					}
				}
			}
		
		}
	}

	return TRUE;
}


BOOL						Item::CancelMagic()
{
	Assert(m_pItem);
	if ( m_pItem->m_Equip.m_SoulBeadType == SOUL_BEAD_WU )
	{
		for ( INT i=0; i<m_pItem->m_Equip.m_SoulBeadAttrCount; ++i )
		{
			for ( INT j=0; j<m_pItem->m_Equip.m_AttrCount; ++j )
			{
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_AttrType)&&
					(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value -= (INT)(m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_Value.m_Value/10);
					if ( m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value <= 0 )
					{
						m_pItem->m_Equip.m_pAttr[j].CleanUp();
						m_pItem->m_Equip.m_AttrCount--;
					}
				}
			}
		}
		for ( INT i=0; i<m_pItem->m_Equip.m_SoulBeadAttrCount; ++i )
		{
			m_pItem->m_Equip.m_pWuSoulBeadAttr[i].CleanUp();
		}
		m_pItem->m_Equip.m_SoulBeadType = 0;
		m_pItem->m_Equip.m_SoulBeadAttrCount = 0;

		return TRUE;
	}
	else if (m_pItem->m_Equip.m_SoulBeadType == SOUL_BEAD_QI )
	{
		for ( INT i=0; i<m_pItem->m_Equip.m_SoulBeadAttrCount; ++i )
		{
			for ( INT j=0; j<m_pItem->m_Equip.m_AttrCount; ++j )
			{
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_AttrType)&&
				(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value -= (INT)(m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_Value.m_Value/10);
					if ( m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value <= 0 )
					{
						m_pItem->m_Equip.m_pAttr[j].CleanUp();
						m_pItem->m_Equip.m_AttrCount--;
					}
				}
			}
		}
		for ( INT i=0; i<m_pItem->m_Equip.m_SoulBeadAttrCount; ++i )
		{
			m_pItem->m_Equip.m_pQiSoulBeadAttr[i].CleanUp();
		}
		m_pItem->m_Equip.m_SoulBeadType = 0;
		m_pItem->m_Equip.m_SoulBeadAttrCount = 0;

		return TRUE;
	}
	
	return FALSE;
	
}


UINT						Item::GetItemSellPrice()		const
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nSellPrice;
}

BYTE						Item::GetItemLevel()		const				//物品级别,策划需求
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nLevel;
}
BYTE						Item::GetReqSkillLevel()	const				//获得技能级别需求
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_nReqSkillLevel;
}
INT							Item::GetReqSkill()			const				//需要拥有的技能号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nReqSkill;
}
INT							Item::GetScriptID()			const				//获得相关脚本编号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nScriptID;
}
INT							Item::GetSkillID()			const				//获得相关技能编号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nSkillID;
}

BYTE						Item::GetTargetType()		const			   //物品选择对象类型
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_TargetType;
}

BOOL						Item::IsBroadCast()			const				//是否广播
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_bBroadcast;
}

//-------------------------------------------------------------------------------------------
//宝石
const _ITEM_ATTR				Item::GetGemAttr(UINT i)		const				//宝石属性
{
	Assert(m_pItem);
	Assert(i<m_pItem->GetGemData()->m_AttCount);
	return m_pItem->GetGemData()->m_GenAttr[i];
}

//前置镶嵌的宝石
UINT						Item::GetGemReqNum()		const
{
	Assert(m_pItem);
	return m_pItem->GetGemData()->m_GemReqCount;
}

const _GEM_REQ_ATT			Item::GetReqGemAtt(INT nPos)	const		//获得宝石信息
{
	Assert(m_pItem);
	Assert(nPos<MAX_GEM_REQ);
	return m_pItem->GetGemData()->m_GemReqAttr[nPos];
}

UINT				Item::GetReqEquip(INT nEquipType) const
{
	Assert(m_pItem);
	Assert(nEquipType<MAX_BASE_EQUIP_COUNT);
	return m_pItem->GetGemData()->m_EquipType[nEquipType];
}

//---------------------------------------------------------------------------------
//额外包裹属性	

UINT					Item::GetExtraBagSellPrice()		const	
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	return m_pItem->GetExtraBagData()->m_nSellPrice;
}

UINT					Item::GetExtraBagBasePrice()		const	
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	return m_pItem->GetExtraBagData()->m_nBasePrice;
}

UINT					Item::GetExtraBagValidTime()		const				//
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	return m_pItem->GetExtraBagData()->m_nValidTime;
}

UINT					Item::GetExtraBagRemainTime()		const				//
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	UINT elpsTime = (UINT)g_pTimeManager->GetANSITime()-m_pItem->GetExtraBagData()->m_nBeginTime;
	if (elpsTime>=m_pItem->GetExtraBagData()->m_nValidTime)
	{
		return 0;
	}
	return elpsTime-m_pItem->GetExtraBagData()->m_nValidTime;
}

UINT					Item::GetExtraBagBeginTime()		const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	return m_pItem->GetExtraBagData()->m_nBeginTime;
}

VOID					Item::SetExtraBagBeginTime()
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	Assert(m_pItem->GetExtraBagData()->m_nBeginTime = INVALID_TIME);
	m_pItem->GetExtraBagData()->m_nBeginTime = (UINT)g_pTimeManager->GetANSITime();
}

UINT					Item::GetExtraBagSize()			const					//
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EXTRABAG);
	return m_pItem->GetExtraBagData()->m_nSize;
}

//---------------------------------------------------------------------------------
//藏宝图
INT						Item::GetMapLevel()			const				//藏宝图级别
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_nLevel;
}
FLOAT					Item::GetXPos()				const				//x坐标
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_xPos;
}
FLOAT					Item::GetZPos()				const				//z坐标
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_zPos;
}
INT						Item::GetSceneID()			const				//场景编号
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_SceneID;
}
INT						Item::GetGrowType()			const				//对应生长点类型
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_GrowType;
}

inline  PET_GUID_t		Item::GetPetGUID( )								//取得宠物GUID
{
	Assert(m_pPet);
	return m_pPet->m_GUID;
}

inline  PET_GUID_t		Item::GetSpouseGUID( )
{
	Assert(m_pPet);
	return m_pPet->m_SpouseGUID;
}

inline  INT				Item::IsCheckup()
{
	Assert(m_pPet);
	return m_pPet->m_bCheckup;
}

inline	INT				Item::GetDataID()					//取得宠物模型		
{
	Assert(m_pPet);
	return m_pPet->m_nDataID;
}
inline  const CHAR*		Item::GetName()	const				//取得名字
{
	Assert(m_pPet);
	return m_pPet->m_szName;	
}
inline  const CHAR*		Item::GetNick() const				//取得昵称
{
	Assert(m_pPet);
	return m_pPet->m_szNick;
}
inline  INT				Item::GetLevel() const				//取得等级
{
	Assert(m_pPet);
	return m_pPet->m_nLevel;
}
inline  INT				Item::GetTakeLevel() const			//取得携带等级
{
	Assert(m_pPet);
	return m_pPet->m_nTakeLevel;
}
inline  INT				Item::GetSex() const
{
	Assert(m_pPet);
	return m_pPet->m_iSex;
}
inline  INT				Item::GetAttackType() const			//取得进攻类型（物/法）
{
	Assert(m_pPet);
	return m_pPet->m_nAttackType;
}
inline  INT				Item::GetAIType() const				//取得AI类型
{
	Assert(m_pPet);
	return m_pPet->m_AIType;
}
inline  const _CAMP_DATA *Item::GetCampData() const				//取得阵营
{
	Assert(m_pPet);
	return &(m_pPet->m_CampData);
}
//inline  INT				Item::GetHP() const					//取得生命值
//{
//	Assert(m_pPet);
//	return m_pPet->m_nHP;
//}
//inline  INT				Item::GetBaseMaxHP() const				//取得生命值上限
//{
//	Assert(m_pPet);
//
//	INT nBaseParam = 0;
//	INT nValue = 0;
//	INT nAttribParam = 0;
//	FLOAT fLevelRate = 0;
//	FLOAT fAttribRate = 0.f;
//
//	INT nLevel = GetLevel();
//	FLOAT fGrowRate = GetGrowRate();
//
//	nBaseParam = g_TableInit.m_PetConfig.m_BaseHP;
//	fAttribRate = g_TableInit.m_PetConfig.m_Con_HP_Pram;
//	fLevelRate = g_TableInit.m_PetConfig.m_Level_HP_Pram;
//	nAttribParam = GetLvl1Attr(CATTR_LEVEL1_CON);
//	nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
//	return nValue;
//}
//inline  INT				Item::GetLife() const				//取得当前寿命
//{
//	Assert(m_pPet);
//	return m_pPet->m_nLife;
//}
//inline  BYTE			Item::GetPetType() const				// 宝宝，变异，野生
//{
//	Assert(m_pPet);
//	return m_pPet->m_byPetType;
//}
inline  BYTE			Item::GetGeneration() const				// 几代宠
{
	Assert(m_pPet);
	return m_pPet->m_byGeneration;
}
inline  BYTE			Item::GetHappiness() const				// 快乐度		
{
	Assert(m_pPet);
	return m_pPet->m_byHappiness;
}
inline  INT				Item::GetStrengthPer() const			// 力量资质	
{
	Assert(m_pPet);
	return m_pPet->m_iStrengthPerception;
}

inline  BOOL			Item::IsMating() const					// 是否交配锁定
{
	Assert(m_pPet);
	return m_pPet->m_bMating;
}

inline  INT				Item::GetSmartnessPer() const			// 敏捷资质	
{
	Assert(m_pPet);
	return m_pPet->m_iSmartnessPerception;
}
inline  INT 			Item::GetMindPer() const				// 智力资质
{
	Assert(m_pPet);
	return m_pPet->m_iMindPerception;
}

inline  INT 			Item::GetConstitutionPer() const		// 体质资质
{
	Assert(m_pPet);
	return m_pPet->m_iConstitutionPerception;
}
inline	INT				Item::GetGenGu() const					// 根骨
{
	Assert(m_pPet);
	return m_pPet->m_nGenGu;
}

inline  INT				Item::GetSavvy() const					//悟性
{
	Assert(m_pPet);
	return m_pPet->m_iSavvy;
}

inline  FLOAT			Item::GetGrowRate() const				// 成长率
{
	Assert(m_pPet);
	return m_pPet->m_fGrowRate;
}
inline  INT				Item::GetRemainPoint() const			// 一级属性剩余点数
{
	Assert(m_pPet);
	return m_pPet->m_nRemainPoint;
}
inline  INT				Item::GetExp() const					// 经验值
{
	Assert(m_pPet);
	return m_pPet->m_nExp;
}
inline  INT				Item::GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const// 基础一级战斗属性（不包括技能和装备增加的部分）
{
	Assert(m_pPet);
	return m_pPet->m_BaseAttrLevel1.Get(type);
}
inline  _PET_SKILL		Item::GetSkill(UINT SkillIndex)		//宠物技能
{
	Assert(m_pPet);
	Assert(SkillIndex<PET_MAX_SKILL_COUNT);
	return m_pPet->m_SkillList[SkillIndex];
}
inline  VOID			Item::SetCooldown(CooldownID_t nID, Time_t nTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.RegisterCooldown(nID, nTime);
}
inline	BOOL			Item::IsCooldowned(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.IsSpecificSlotCooldownedByID(nID);
}
inline	VOID			Item::HeartBeat_Cooldown(Time_t nDeltaTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.HeartBeat(nDeltaTime);
}
inline	Time_t			Item::GetCooldownRemained(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.GetRemainTimeByID(nID);
}

/*
inline	_IMPACT_LIST&	Item::GetImpactList()				// 得到宠物BUFF列表
{
	Assert(m_pPet);
	return m_pPet->m_ImpactList;
}
*/
// add by gh for Soulbead
VOID		Item::SetSoulBeadPrice(UINT price) 
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_SOUL_BEAD);
	m_pItem->GetSoulBeadData()->m_nSellPrice = price;
}

UINT		Item::GetSoulBeadPrice()		const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_SOUL_BEAD);
	return m_pItem->GetSoulBeadData()->m_nSellPrice;
}

VOID		Item::SetSoulBeadNeedLevel(BYTE needLevel)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_SOUL_BEAD);
	m_pItem->GetSoulBeadData()->m_nNeedLevel = needLevel;
}
//魂珠属性的数量
BYTE	Item::GetSoulBeadAttrCount()	const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_SOUL_BEAD);
	return m_pItem->GetSoulBeadData()->m_nAttrCount;
}

BYTE		Item::GetSoulBeadNeedLevel()	const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_SOUL_BEAD);
	return m_pItem->GetSoulBeadData()->m_nNeedLevel;
}

//获得武魂珠属性
const _ITEM_ATTR	Item::GetWuSoulBeadAttr(UCHAR	nIndex)		const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemType()==SOUL_BEAD_WU);
	Assert(nIndex<MAX_WU_SOUL_BEAD_ATTR);
	return m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[nIndex];
}
VOID	Item::SetWuSoulBeadAttr(const _ITEM_ATTR& ItemAttr)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemType()==SOUL_BEAD_WU);
	BYTE nIndex = m_pItem->GetSoulBeadData()->m_nAttrCount;
	Assert(nIndex<MAX_WU_SOUL_BEAD_ATTR);
	m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[nIndex].m_AttrType = ItemAttr.m_AttrType;
	m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[nIndex].m_Value.m_Value = ItemAttr.m_Value.m_Value;
	++m_pItem->GetSoulBeadData()->m_nAttrCount;
}
//获得器魂珠属性
const _ITEM_ATTR	Item::GetQiSoulBeadAttr(UCHAR	nIndex)		const
{
	Assert(m_pItem);
	Assert(m_pItem->ItemType()==SOUL_BEAD_QI);
	Assert(nIndex<MAX_QI_SOUL_BEAD_ATTR);
	return m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[nIndex];
}

VOID Item::SetQiSoulBeadAttr(const _ITEM_ATTR&  ItemAttr)
{
	Assert(m_pItem);
	Assert(m_pItem->ItemType()==SOUL_BEAD_QI);
	BYTE nIndex = m_pItem->GetSoulBeadData()->m_nAttrCount;
	Assert(nIndex<MAX_QI_SOUL_BEAD_ATTR);
	m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[nIndex].m_AttrType = ItemAttr.m_AttrType;
	m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[nIndex].m_Value.m_Value = ItemAttr.m_Value.m_Value;
	++m_pItem->GetSoulBeadData()->m_nAttrCount;
}

#endif					
