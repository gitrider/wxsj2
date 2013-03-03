/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Item.inl
//������������Ʒ���ݶ�ȡд���inline����
//�����ߣ�jack
//�޸ļ�¼��2005-11-2����
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

inline UINT Item::GetItemTableIndex( ) const//ȡ����Ʒ�������Ʒ�������
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

inline INT Item::GetLayedNum( ) const//ȡ�õ�����Ʒ������
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

inline INT Item::GetMaxLayedNum( ) const//ȡ���ܹ�������Ʒ���������
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
//װ��

_ITEM*				Item::GetItemData()			const				//���װ������������
{
	return m_pItem;
}


INT					Item::GetSetNum()			const				//��װ���
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return 0;
	}
}
BYTE					Item::GetMaxSetCount()			const				//��װ����
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return 0;
	}
	
}
BYTE					Item::GetEquipPoint()		const				//��Ʒװ���
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return -1;

	}
	
}

INT					Item::GetMaxDurPoint()		const			//����;�ֵ
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return -1;
	}
}
BYTE					Item::GetRequireLevel()		const			//����ȼ�
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_NeedLevel;
}

BYTE					Item::GetCurLevel()		const			//����ȼ�
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_Level;
}

BYTE					Item::GetGemSlotMax()		const			//���ʯ����
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return -1;
	}
}

INT					Item::GetDurPoints()		const 				//��ǰ�;�ֵ
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDurPoint;
}

INT					Item::GetCurMaxDurPoint()		const 				//��ǰ�;�����
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

BYTE					Item::GetAttrCount()		const				//���Ե�����
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_AttrCount;
}

BYTE					Item::GetEquipGemCount()	const				//��Ƕ��ʯ������
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_StoneCount;
}

BYTE					Item::GetAffixSkillCount()		const			//��ø��Ӽ��ܸ���
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return 0;
	}
}

BYTE					Item::GetEffectSkillCount()		const			//���װ��Ӱ�켼�ܸ���
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return 0;
	}
}

_ITEM_EFFECT_SKILL*		Item::GetEffectSkill(UINT pos)		const			//���װ��Ӱ�켼�ܸ���
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return NULL;
	}
}

BOOL					Item::IsWuMenPai()				const	//���װ���Ƿ�����������
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
		AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
		return TRUE;
	}

	__LEAVE_FUNCTION

	return TRUE;
}

BOOL				Item::InReqJob(MENPAI_ATTRIBUTE menpaiID)	const	//ָ���������Ƿ���װ������������һ��
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
			AssertEx(FALSE,"��Ʒ����ʱ���ݳ���!������Ʒ��������");
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

BYTE					Item::GetCurGemHole()		const			//���ʯ����
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

UINT					Item::GetEquipGem(INT nPos)	const				//��Ƕ��ʯ������
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
	//����ǰ�ñ�ʯ
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

const _ITEM_ATTR				Item::GetEquipAttr(UCHAR	nIndex)		const	//���װ������
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

const _ITEM_GEMINFO			Item::GetEquipGemInfo(UCHAR nIndex)	const		//��ñ�ʯ��Ϣ
{
	Assert(m_pItem);
	Assert(nIndex<GetGemSlotMax());
	return m_pItem->GetEquipData()->m_pGemInfo[nIndex];
}

BYTE						Item::GetEquipPaiType()		    const			//���װ������
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
			// ��װ���б��ݻ��������
			m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_AttrType = Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType;
			m_pItem->m_Equip.m_pWuSoulBeadAttr[i].m_Value.m_Value = Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_Value.m_Value;
		
			for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
			{
				// ���װ������ͬ���͵����ԣ����������Ե�10%���ӵ�ԭ������
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType)&&
					(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value += (INT)(Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_Value.m_Value/10);
					m_pItem->m_Equip.m_SoulBeadAttrCount++;
					IsHold = TRUE;
				}
			}
			// ����������ԣ�
			if ( (!IsHold) &&( Bead->m_SoulBead.m_pWuSoulBeadAttr[i].m_AttrType !=0 ) )
			{
				for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
				{
					// ����ǿ�λ
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
			// ��װ���б��ݻ��������
			m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_AttrType = Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType;
			m_pItem->m_Equip.m_pQiSoulBeadAttr[i].m_Value.m_Value = Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_Value.m_Value;
		
			for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
			{
				// ���װ������ͬ���͵����ԣ����������Ե�10%���ӵ�ԭ������
				if ((m_pItem->m_Equip.m_pAttr[j].m_AttrType == Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType)&&
					(m_pItem->m_Equip.m_pAttr[j].m_AttrType != 0))
				{
					m_pItem->m_Equip.m_pAttr[j].m_Value.m_Value += (INT)(Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_Value.m_Value/10);
					m_pItem->m_Equip.m_SoulBeadAttrCount++;
					IsHold = TRUE;
				}
			}
			// ����������ԣ�
			if ( (!IsHold) &&(Bead->m_SoulBead.m_pQiSoulBeadAttr[i].m_AttrType != 0) )
			{
				for ( INT j=0; j<MAX_ITEM_ATTR; ++j )
				{
					// ����ǿ�λ
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

BYTE						Item::GetItemLevel()		const				//��Ʒ����,�߻�����
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nLevel;
}
BYTE						Item::GetReqSkillLevel()	const				//��ü��ܼ�������
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_nReqSkillLevel;
}
INT							Item::GetReqSkill()			const				//��Ҫӵ�еļ��ܺ�
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nReqSkill;
}
INT							Item::GetScriptID()			const				//�����ؽű����
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nScriptID;
}
INT							Item::GetSkillID()			const				//�����ؼ��ܱ��
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nSkillID;
}

BYTE						Item::GetTargetType()		const			   //��Ʒѡ���������
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_TargetType;
}

BOOL						Item::IsBroadCast()			const				//�Ƿ�㲥
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_bBroadcast;
}

//-------------------------------------------------------------------------------------------
//��ʯ
const _ITEM_ATTR				Item::GetGemAttr(UINT i)		const				//��ʯ����
{
	Assert(m_pItem);
	Assert(i<m_pItem->GetGemData()->m_AttCount);
	return m_pItem->GetGemData()->m_GenAttr[i];
}

//ǰ����Ƕ�ı�ʯ
UINT						Item::GetGemReqNum()		const
{
	Assert(m_pItem);
	return m_pItem->GetGemData()->m_GemReqCount;
}

const _GEM_REQ_ATT			Item::GetReqGemAtt(INT nPos)	const		//��ñ�ʯ��Ϣ
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
//�����������	

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
//�ر�ͼ
INT						Item::GetMapLevel()			const				//�ر�ͼ����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_nLevel;
}
FLOAT					Item::GetXPos()				const				//x����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_xPos;
}
FLOAT					Item::GetZPos()				const				//z����
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_zPos;
}
INT						Item::GetSceneID()			const				//�������
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_SceneID;
}
INT						Item::GetGrowType()			const				//��Ӧ����������
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_GrowType;
}

inline  PET_GUID_t		Item::GetPetGUID( )								//ȡ�ó���GUID
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

inline	INT				Item::GetDataID()					//ȡ�ó���ģ��		
{
	Assert(m_pPet);
	return m_pPet->m_nDataID;
}
inline  const CHAR*		Item::GetName()	const				//ȡ������
{
	Assert(m_pPet);
	return m_pPet->m_szName;	
}
inline  const CHAR*		Item::GetNick() const				//ȡ���ǳ�
{
	Assert(m_pPet);
	return m_pPet->m_szNick;
}
inline  INT				Item::GetLevel() const				//ȡ�õȼ�
{
	Assert(m_pPet);
	return m_pPet->m_nLevel;
}
inline  INT				Item::GetTakeLevel() const			//ȡ��Я���ȼ�
{
	Assert(m_pPet);
	return m_pPet->m_nTakeLevel;
}
inline  INT				Item::GetSex() const
{
	Assert(m_pPet);
	return m_pPet->m_iSex;
}
inline  INT				Item::GetAttackType() const			//ȡ�ý������ͣ���/����
{
	Assert(m_pPet);
	return m_pPet->m_nAttackType;
}
inline  INT				Item::GetAIType() const				//ȡ��AI����
{
	Assert(m_pPet);
	return m_pPet->m_AIType;
}
inline  const _CAMP_DATA *Item::GetCampData() const				//ȡ����Ӫ
{
	Assert(m_pPet);
	return &(m_pPet->m_CampData);
}
//inline  INT				Item::GetHP() const					//ȡ������ֵ
//{
//	Assert(m_pPet);
//	return m_pPet->m_nHP;
//}
//inline  INT				Item::GetBaseMaxHP() const				//ȡ������ֵ����
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
//inline  INT				Item::GetLife() const				//ȡ�õ�ǰ����
//{
//	Assert(m_pPet);
//	return m_pPet->m_nLife;
//}
//inline  BYTE			Item::GetPetType() const				// ���������죬Ұ��
//{
//	Assert(m_pPet);
//	return m_pPet->m_byPetType;
//}
inline  BYTE			Item::GetGeneration() const				// ������
{
	Assert(m_pPet);
	return m_pPet->m_byGeneration;
}
inline  BYTE			Item::GetHappiness() const				// ���ֶ�		
{
	Assert(m_pPet);
	return m_pPet->m_byHappiness;
}
inline  INT				Item::GetStrengthPer() const			// ��������	
{
	Assert(m_pPet);
	return m_pPet->m_iStrengthPerception;
}

inline  BOOL			Item::IsMating() const					// �Ƿ�������
{
	Assert(m_pPet);
	return m_pPet->m_bMating;
}

inline  INT				Item::GetSmartnessPer() const			// ��������	
{
	Assert(m_pPet);
	return m_pPet->m_iSmartnessPerception;
}
inline  INT 			Item::GetMindPer() const				// ��������
{
	Assert(m_pPet);
	return m_pPet->m_iMindPerception;
}

inline  INT 			Item::GetConstitutionPer() const		// ��������
{
	Assert(m_pPet);
	return m_pPet->m_iConstitutionPerception;
}
inline	INT				Item::GetGenGu() const					// ����
{
	Assert(m_pPet);
	return m_pPet->m_nGenGu;
}

inline  INT				Item::GetSavvy() const					//����
{
	Assert(m_pPet);
	return m_pPet->m_iSavvy;
}

inline  FLOAT			Item::GetGrowRate() const				// �ɳ���
{
	Assert(m_pPet);
	return m_pPet->m_fGrowRate;
}
inline  INT				Item::GetRemainPoint() const			// һ������ʣ�����
{
	Assert(m_pPet);
	return m_pPet->m_nRemainPoint;
}
inline  INT				Item::GetExp() const					// ����ֵ
{
	Assert(m_pPet);
	return m_pPet->m_nExp;
}
inline  INT				Item::GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const// ����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	Assert(m_pPet);
	return m_pPet->m_BaseAttrLevel1.Get(type);
}
inline  _PET_SKILL		Item::GetSkill(UINT SkillIndex)		//���＼��
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
inline	_IMPACT_LIST&	Item::GetImpactList()				// �õ�����BUFF�б�
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
//�������Ե�����
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

//������������
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
//�������������
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
