
#include "stdafx.h"
#include "Item.h"
#include "ItemRuler.h"
#include "DB_Struct.h"


Item::Item( )
{
__ENTER_FUNCTION

	m_pItem = NULL ;
	m_pPet	= NULL;
	m_bLock = FALSE ;
	m_bIsInExchange = FALSE;

__LEAVE_FUNCTION
}


Item::~Item( )
{
__ENTER_FUNCTION

	m_pItem = NULL ;
	m_pPet  = NULL;
	m_bLock = FALSE ;
	m_bIsInExchange = FALSE;

__LEAVE_FUNCTION
}


BOOL Item::Init( const _ITEM_INIT* pInit )
{
__ENTER_FUNCTION

	Assert( pInit ) ;
	
	m_pItem = const_cast<_ITEM*>(pInit->m_pItem);
	m_pPet	= const_cast<_PET_DB_LOAD*>(pInit->m_pPet);
	
	Assert((m_pItem!=0)||(m_pPet!=0));
	Assert(!(m_pItem==0 && m_pPet == 0));
	Assert(!(m_pItem!=0 && m_pPet!= 0));

	if(m_pItem) m_Type = IDT_ITEM;
	if(m_pPet)  m_Type = IDT_PET;

	//Assert( m_pItem ) ;
	

	m_bLock = FALSE ;

__LEAVE_FUNCTION

	return TRUE ;
}


VOID Item::CleanUp( )
{
__ENTER_FUNCTION

	if( m_pItem )
	{
		memset( m_pItem, 0, sizeof(_ITEM) ) ;
	}

	if(m_pPet)
	{
		m_pPet->CleanUp();
		//memset(m_pPet,0,sizeof(_PET_DB_LOAD));
	}
	m_bLock = FALSE ;

__LEAVE_FUNCTION
}

BOOL Item::IsEmpty( )
{
__ENTER_FUNCTION

	
	
	switch(m_Type)
	{
	case IDT_ITEM:
		{
			if(m_pItem==NULL)
			{
				Assert(FALSE);
				return TRUE ;
			}

			if( m_pItem->IsNullType() )
			return TRUE ;
		}
		break;
	case IDT_PET:
		{
			if(m_pPet == NULL)
			{
				Assert(FALSE);
				return TRUE ;
			}
			if( TRUE == m_pPet->m_GUID.IsNull() )
				return TRUE;
		}
		break;
	default:
		{
			Assert(FALSE);
			return TRUE;
		}
	}

	return FALSE ;

__LEAVE_FUNCTION

	return TRUE ;
}

//�жϴ���Ʒ�Ƿ�Ϊһ��װ��
BOOL	Item::IsEquip( ) const
{
__ENTER_FUNCTION

	Assert(m_pItem);

	BYTE bClass = m_pItem->ItemClass();
	if ( bClass == ICLASS_EQUIP ) return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsCanLay()	const //�Ƿ���Ե��ŵ��ж�
{
	__ENTER_FUNCTION
	
	switch(m_Type)
	{
		case IDT_ITEM:
			{
				Assert(m_pItem);
				return	ItemRulerCheck::CheckRuler(IRL_TILE,m_pItem->m_RulerID);
			}
			break;
		case IDT_PET:
			{
				Assert(m_pPet);
				return FALSE;
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

BOOL	Item::IsCanEquipBind()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return ItemRulerCheck::CheckRuler(IRL_EQUIPBIND,m_pItem->m_RulerID)	;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsRuler(ITEM_RULER_LIST ruler) const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return ItemRulerCheck::CheckRuler(ruler,m_pItem->m_RulerID);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsBind() const
{
	__ENTER_FUNCTION

		Assert(m_pItem);
	return m_pItem->GetItemBind();

	__LEAVE_FUNCTION

		return FALSE;
}

VOID	Item::SaveValueTo(_ITEM* pItem)
{
	__ENTER_FUNCTION
	
	 Assert(pItem);
	 Assert(m_pItem);
	 //if(m_pItem->ItemClass() == ICLASS_EQUIP /*&& GetSerialQual(m_pItem->m_ItemIndex)== EQUALITY_BLUE*/)
	 //{
		/* if(m_pItem->GetItemIdent())
		 {
			memcpy(pItem,m_pItem,sizeof(_ITEM));
		 }
		 else
		 {
			pItem->m_ItemGUID	= m_pItem->m_ItemGUID;
			pItem->m_ItemIndex	= m_pItem->m_ItemIndex;
			pItem->m_nsBind		= m_pItem->m_nsBind;
			pItem->m_RulerID	= m_pItem->m_RulerID;
		 }
	 }
	 else*/
		memcpy(pItem,m_pItem,sizeof(_ITEM));
	
	__LEAVE_FUNCTION
}

VOID	Item::SaveValueTo(_PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION

	Assert(pPet);
	Assert(m_pPet);
	memcpy(pPet, m_pPet, sizeof(_PET_DB_LOAD));

	__LEAVE_FUNCTION

}


UINT	Item::GetPrice()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	BYTE	 bClass		= m_pItem->ItemClass();
	//BYTE	 bQuality	= m_pItem->GetQual();
	
	switch(bClass)
	{
	case ICLASS_EQUIP:
		{
			return m_pItem->GetEquipData()->m_BasePrice;
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_TASKITEM:
	case ICLASS_IDENT:
		{
			return m_pItem->GetMedicData()->m_nBasePrice;
		}
		break;
	case ICLASS_GEM:
		{
			return m_pItem->GetGemData()->m_nPrice;
		}
		break;
	case ICLASS_STOREMAP:
		{
			return 0;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);
	return -1;

	__LEAVE_FUNCTION

	return -1;
}

UINT	Item::GetSellPrice() const
{
	__ENTER_FUNCTION

		Assert(m_pItem);
	BYTE	 bClass		= m_pItem->ItemClass();
	//BYTE	 bQuality	= m_pItem->GetQual();

	switch(bClass)
	{
	case ICLASS_EQUIP:
		{
			//���ۼ۸񣽻����۸�*1/2*��ǰ�;�/Max�;�*��ǰ�;�����/ Max�;�
			float nRate = 
				(float)0.5*(float)(m_pItem->GetEquipData()->m_CurDurPoint*m_pItem->GetEquipData()->m_CurMaxDur)/(float)(m_pItem->GetEquipData()->m_MaxDurPoint*m_pItem->GetEquipData()->m_MaxDurPoint);
			UINT nSellPrice = (float)m_pItem->GetEquipData()->m_BasePrice*nRate;
			if (nSellPrice == 0)
			{
				nSellPrice = 1;
			}
			return nSellPrice;
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_IDENT:
		{
			return m_pItem->GetMedicData()->m_nSellPrice;
		}
		break;
	case ICLASS_TASKITEM: 
		{
			return m_pItem->GetMedicData()->m_nSellPrice;
		}
		break;
	case ICLASS_GEM:
		{
			return m_pItem->GetGemData()->m_nSellPrice;
		}
		break;
	case ICLASS_STOREMAP:
		{
			return 0;
		}
	case ICLASS_EXTRABAG:
		{
			return m_pItem->GetExtraBagData()->m_nSellPrice;
		}
		break;
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);
	return -1;

	__LEAVE_FUNCTION

		return -1;
}

INT		Item::GetItemParam(UINT start,ItemParamValue& ipv)
{
	__ENTER_FUNCTION
		
		Assert(m_pItem);
		return m_pItem->GetItemParamValue(start,ipv);	

	__LEAVE_FUNCTION

		return -1;
}

VOID	Item::SetItemValue(_ITEM* pItem)
{	
	__ENTER_FUNCTION

	Assert(pItem);
	Assert(m_pItem);
	*m_pItem = *pItem;

	__LEAVE_FUNCTION
}


//������������
VOID Item::SetWuSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr)
{
	for (int i=0; i<attrCount; ++i)
	{
		_ITEM_ATTR ItemAttr = tmpItemAttr[i];

		for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)//������ظ���������ӣ��ı�ֵ
		{
			if (ItemAttr.m_AttrType == GetWuSoulBeadAttr(m).m_AttrType)
			{//��������ֵ
				m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
				break;
			}
		}

		switch (ItemAttr.m_AttrType)
		{
		case IATTRIBUTE_POINT_STR://����
		case IATTRIBUTE_POINT_DEX://��
		case IATTRIBUTE_POINT_INT://����
		case IATTRIBUTE_POINT_CON://����
		case IATTRIBUTE_BASE_ATTACK_NEAR://����Զ���ڹ���
		case IATTRIBUTE_BASE_ATTACK_FAR://����Զ���⹥��
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR://���������ڹ�����
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR://���������⹦����
			{
				SetWuSoulBeadAttr(ItemAttr);
			}
			break;
		case IATTRIBUTE_POINT_ATTACK_NEAR://Զ���ڹ�����
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_ATTACK_NEAR == GetWuSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}	
			break;
		case IATTRIBUTE_POINT_ATTACK_FAR://Զ���⹦����
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_ATTACK_FAR == GetWuSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;
		case IATTRIBUTE_POINT_ATTACK_MAGIC_NEAR://�����ڹ�����
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR == GetWuSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;
		case IATTRIBUTE_POINT_ATTACK_MAGIC_FAR://�����⹦����
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_ATTACK_MAGIC_FAR == GetWuSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pWuSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}
}
VOID Item::SetQiSoulBeadAttr(const BYTE attrCount, const _ITEM_ATTR* tmpItemAttr)
{
	for (int i=0; i<attrCount; ++i)
	{
		_ITEM_ATTR ItemAttr = tmpItemAttr[i];

		for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)//������ظ���������ӣ��ı�ֵ
		{
			if (ItemAttr.m_AttrType == GetQiSoulBeadAttr(m).m_AttrType)
			{//��������ֵ
				m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
				break;
			}
		}

		switch (ItemAttr.m_AttrType)
		{
		case IATTRIBUTE_POINT_STR://����
		case IATTRIBUTE_POINT_DEX://��
		case IATTRIBUTE_POINT_INT://����
		case IATTRIBUTE_POINT_CON://����
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR://���������ڹ�����
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR://���������⹦����
		case IATTRIBUTE_BASE_DEFENCE_NEAR://����Զ���ڹ�����
		case IATTRIBUTE_BASE_DEFENCE_FAR://����Զ���⹦����
			{
				SetQiSoulBeadAttr(ItemAttr);
			}
			break;
		case IATTRIBUTE_POINT_DEFENCE_MAGIC_NEAR://�����ڹ���������
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR == GetQiSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;
		case IATTRIBUTE_POINT_DEFENCE_MAGIC_FAR://�����⹦��������
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR == GetQiSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;
		case IATTRIBUTE_POINT_DEFENCE_NEAR://Զ���ڹ���������
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_DEFENCE_NEAR == GetQiSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;
		case IATTRIBUTE_POINT_DEFENCE_FAR://Զ���⹦��������
			{
				for (BYTE m=0; m<GetSoulBeadAttrCount(); ++m)
				{
					if (IATTRIBUTE_BASE_DEFENCE_FAR == GetQiSoulBeadAttr(m).m_AttrType)
					{//��������ֵ
						m_pItem->GetSoulBeadData()->m_pQiSoulBeadAttr[m].m_Value.m_Value += ItemAttr.m_Value.m_Value;
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

BOOL	Item::GetItemTransferString(CHAR* pOutBuf, UINT nOutBufLength)
{
	__ENTER_FUNCTION

		Assert(m_pItem);
	Assert(pOutBuf);

	INT nRealLength = 0;
	return Item2String(m_pItem, pOutBuf, nRealLength, (INT)nOutBufLength);
	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	Item::GetPetTransferString(CHAR* pOutBuf, UINT nOutBufLength)
{
	__ENTER_FUNCTION

		Assert(m_pPet);
	Assert(pOutBuf);

	if(GetPetGUID().IsNull()) return FALSE;

	INT nLen = 0;

	CHAR buf[ MAX_ITEM_STRING_LENGTH + 1 ] = {0};

	// ���޵�GUID
	memcpy( buf, &(m_pPet->m_GUID), sizeof(PET_GUID_t) );
	nLen += sizeof(PET_GUID_t);

	// ���� ����ϵͳ���ú󣬽��ó�����ϸ���Լ�����䵽buf�� 20100705


	//������ɣ���Ҫ�ڷ���ǰ���б�Ҫ��ת��
	CHAR strBuf[ MAX_ITEM_STRING_LENGTH + 1 ] = {0};
	Binary2String( buf, nLen, strBuf );

	//����ת���Ľ��
	CHAR strLen[16] = {0};
	tsnprintf( strLen, 15, "%03dp", strlen(strBuf) + 1 );

	nLen = (INT)( strlen(strLen) + strlen(strBuf) );

	Assert( (INT)nOutBufLength >= nLen );

	strcpy( pOutBuf, strLen );
	strcpy( pOutBuf + strlen(pOutBuf), strBuf );

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
