#include "stdafx.h"
#include "OpenBox.h"
#include "Rand.h"
#include "ItemOperator.h"
#include "HumanOpenBox.h"


BOOL OpenBox::GetRow( INT Index, OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow )
{
	__ENTER_FUNCTION

	for ( INT i=0; i<g_OpenBox_ItemInfo_Table.m_KindRealNum; ++i)
	{
		if ( Index == g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i].m_Index ) //得到满足index的那一行
		{
			ItemRow = g_OpenBox_ItemInfo_Table.m_ItemInfoRow[i];
			return TRUE;
		}
	}

	__LEAVE_FUNCTION

		return FALSE;
}

INT OpenBox::GetRate1Sum(OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow )
{	
	INT ret=0;

	__ENTER_FUNCTION

	for ( INT i=0; i<ItemRow.m_RealNum; ++i )
	{
		ret +=ItemRow.m_ItemInfo[i].m_Rate1;
	}

	__LEAVE_FUNCTION

	return ret;
}

BOOL OpenBox::ProcessItem( INT Index )
{
	__ENTER_FUNCTION

	INT ret = 0;

	if ( IsUseable( Index ) )
	{
		OpenBox_ItemInfo_Table::ItemInfoRow _ItemRow;

		if ( !GetRow( Index, _ItemRow ) )
			return FALSE;


		INT nRate1Sum = GetRate1Sum( _ItemRow );

		Assert( nRate1Sum > 0 );

        INT nRand = RandGen::GetRand(0, nRate1Sum);

		for ( INT i=0; i<_ItemRow.m_RealNum; ++i )
		{
			if ( nRand<= _ItemRow.m_ItemInfo[i].m_Rate1 )
			{
				m_ItemSelectedArray[m_OffSet].m_ItemID = _ItemRow.m_ItemInfo[i].m_ItemID;
				m_ItemSelectedArray[m_OffSet].m_Rate = _ItemRow.m_ItemInfo[i].m_Rate2;
				++m_OffSet;
				return TRUE;
			}
			else
			{
				nRand -= _ItemRow.m_ItemInfo[i].m_Rate1;

			}
		}


	}

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL OpenBox::Process(  INT index0,				//中间的物品索引
						INT index1, INT num1,	//index1和此索引中会出现的物品的数量
						INT index2, INT num2,
						INT index3, INT num3,
						INT index4, INT num4,
						INT index5, INT num5  )
{
	__ENTER_FUNCTION

	if ( !ProcessItem( index0 ) )		//对中间物品的处理
	{
		return FALSE;
	}
	

	int i;

	if ( num1 > 0 )                     //对周围物品处理
	{
		for ( i=0; i<num1; ++i  )
		{
			if ( !ProcessItem(index1) )
			{
				return FALSE;
			}
			
		}

	}

	if ( num2 > 0 )
	{
		for ( i=0; i<num2; ++i )
		{
			if ( !ProcessItem(index2) )
			{
				return FALSE;
			}
		}
	}

	if ( num3 > 0 )
	{
		for ( i=0; i<num3; ++i)
		{
			if ( !ProcessItem(index3) )
			{
				return FALSE;
			}
		}
	}

	if ( num4 > 0 )
	{
		for ( i=0; i<num4; ++i)
		{
			if ( !ProcessItem(index4) )
			{
				return FALSE;
			}
		}
	}

	if ( num5 > 0 )
	{
		for ( i=0; i<num5; ++i)
		{
			if ( !ProcessItem(index5) )
			{
				return FALSE;
			}
		}
	}	

	__LEAVE_FUNCTION

	return TRUE;
}

INT OpenBox::AroundItemRand()		//从周围物品数组随机出一个，返回值为数组的的索引
{
	__ENTER_FUNCTION

	INT nRate2Sum = 0;

	int i;

	for ( i=AROUND_INDEX_BEGIN; i<=AROUND_INDEX_END; ++i )
	{
		nRate2Sum += m_ItemSelectedArray[i].m_Rate;
	}

	INT nRand = RandGen::GetRand(0, nRate2Sum);

	for ( i=AROUND_INDEX_BEGIN; i<=AROUND_INDEX_END; ++i )
	{
		if ( nRand<= m_ItemSelectedArray[i].m_Rate )
		{
			return i;
		}
		else
		{
			nRand -= m_ItemSelectedArray[i].m_Rate;

		}
	}

	__LEAVE_FUNCTION

	return INVALID_ID;
}

BOOL OpenBox::RandTheAround(ItemContainer *pSrcContainer, ItemContainer *pDestContainer )
{
	__ENTER_FUNCTION

	INT nIndex = AroundItemRand();

	Assert(pSrcContainer);
	Assert(pDestContainer);

	if ( INVALID_INDEX != nIndex )
	{
		//m_ItemSelectedArray和pSrcContainer中存的数据错着一位
		g_ItemOperator.CopyItem( pSrcContainer, nIndex-1, pDestContainer, HumanOpenBox::RAND_ITEM_INDEX );
		return TRUE;
	}

	__LEAVE_FUNCTION

	return FALSE;
	
}