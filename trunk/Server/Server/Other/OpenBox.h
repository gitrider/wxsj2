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
		INT m_ItemID;					//��Ʒ����Ʒ�������
		INT m_Rate;						//��������Χ����������ĸ���
	};

	enum
	{
		CENTER_ARRAY_INDEX = 0,			//�м���Ʒ�������е�����
		AROUND_INDEX_BEGIN = 1,			//��Χ��Ʒ�������п�ʼ������
		AROUND_INDEX_END = 20,			//����������
	};

	enum
	{
		MAX_ITEM_ARRAY = 21,			//m_ItemSelectedArray�ĳߴ�
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
	BOOL			IsUseable(INT Index)	//��֤����id�Ƿ����
	{ 
		return Index > 0 ? TRUE : FALSE;
	}
											
	BOOL			ProcessItem( INT Index );											//������Ʒ������Ʒ�������
	INT				AroundItemRand( );													//����Χ��Ʒ���������һ��������ֵΪ����ĵ�����
	BOOL			GetRow( INT Index, OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow );	//ͨ�������õ�һ��
	INT				GetRate1Sum( OpenBox_ItemInfo_Table::ItemInfoRow &ItemRow );
	ItemSelected*	GetItemArray() { return m_ItemSelectedArray; };
	BOOL			RandTheAround( ItemContainer *pSrcContainer, ItemContainer *pDestContainer );

	BOOL			Process(    INT index0,												//�м����Ʒ����
								INT index1, INT num1,									//index1�ʹ������л���ֵ���Ʒ������
								INT index2, INT num2,
								INT index3, INT num3,
								INT index4, INT num4,
								INT index5, INT num5   );

private:
	ItemSelected m_ItemSelectedArray[MAX_ITEM_ARRAY];									//��Ŵ�������item��id
	INT m_OffSet;																		//m_ItemIDArray��ƫ��

	

};

#endif