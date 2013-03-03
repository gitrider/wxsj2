#ifndef _OPENBOX_H_
#define _OPENBOX_H_
#include "Type.h"
#include "GameTable.h"

class ItemContainer;

class OpenBox
{
public:
	struct ItemSelected
	{
		INT m_ItemID;					//物品在物品表的索引
		INT m_Rate;						//代表在周围格子中随机的概率
	};

	enum
	{
		CENTER_ARRAY_INDEX = 0,			//中间物品在数组中的索引
		AROUND_INDEX_BEGIN = 1,			//周围物品在数组中开始的索引
		AROUND_INDEX_END = 20,			//结束的索引
	};

	enum
	{
		MAX_ITEM_ARRAY = 21,			//m_ItemSelectedArray的尺寸
	};

public:
	OpenBox()
		:
	m_OffSet(0)
	{
		memset(m_ItemSelectedArray, 0, sizeof(m_ItemSelectedArray));
	}

	~OpenBox() {};

public:
	BOOL			IsUseable(INT Index)	//验证宝箱id是否可用
	{ 
		return Index > 0 ? TRUE : FALSE;
	}
											
	BOOL			ProcessItem( INT Index );											//处理物品，把物品随机出来
	INT				AroundItemRand( );													//从周围物品数组随机出一个，返回值为数组的的索引
	BOOL			GetRow( INT Index, OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow );	//通过索引得到一行
	INT				GetRate1Sum( OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow );
	ItemSelected*	GetItemArray() { return m_ItemSelectedArray; };
	BOOL			RandTheAround( ItemContainer *pSrcContainer, ItemContainer *pDestContainer );

	BOOL			Process(    INT index0,												//中间的物品索引
								INT index1, INT num1,									//index1和此索引中会出现的物品的数量
								INT index2, INT num2,
								INT index3, INT num3,
								INT index4, INT num4,
								INT index5, INT num5   );

private:
	ItemSelected m_ItemSelectedArray[MAX_ITEM_ARRAY];									//存放处理完后的item的id
	INT m_OffSet;																		//m_ItemIDArray的偏移

	

};

#endif