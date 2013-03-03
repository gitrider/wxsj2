/********************************************************************
	��������:	2005��11��2��
	����ʱ��:	16:55
	�ļ�����:	ItemTypes.h
	�ļ�·��:	d:\Prj\Server\Server\Item\ItemTypes.h
	������:		������
	
	�ļ����ܣ� ��Ʒ����\�궨���	��������
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _ITEM_TYPES_H_
#define _ITEM_TYPES_H_

#include "Type.h"
//��Ʒϵͳ��������


//ItemBox �����ֵ
#define		MAX_ITEM_BOX_POOL					5000

//Item �����ֵ
#define		MAX_OBJ_ITEM_POOL					10000

//������Ը���
#define		MAX_ATT_COUNT						1000

//�����������
#define		MAX_DROPBOX_CARRAGE					100

//���������
#define		MAX_MONSTER_DROPBOX					10

//ItemBox ����λ�������Χ
#define		ITEMBOX_DROP_RANGE					10

//���ItemQuality �� ��Ŀ����(�ڲ�ʹ��)
#define		MAX_ITEM_TABLE_QUALITYS				10

//���ItemValue �� ��Ŀ����
#define		MAX_ITEM_TABLE_VALUES				10

#define     MAX_MONSTER_VALUE					100000000


//�������ṹ
struct MONSTER_DROPBOXS 
{
	MONSTER_DROPBOXS()
	{
		memset(m_DropBox,0,sizeof(INT)*MAX_MONSTER_DROPBOX);
	}
	INT		m_DropBox[MAX_MONSTER_DROPBOX];
};

//�������ṹ
struct MONSTER_DROPS 
{
	MONSTER_DROPS()
	{
		m_nDropNum = 0;
		memset(m_DropItem,0,sizeof(INT)*MAX_MONSTER_DROPBOX);
	}
	INT     m_nDropNum;
	INT		m_DropItem[MAX_MONSTER_DROPBOX];
};

//�������ṹ
struct MONSTER_DROPCONTENT 
{
	MONSTER_DROPCONTENT()
	{
		m_nItemValue	= 0;
		m_nCycle		= 0;
		m_nItemCount	= 0;
		m_pDropContent	= NULL;
	}
	INT		m_nItemValue;
	INT     m_nCycle;
	INT		m_nItemCount;
	INT*	m_pDropContent;
};


#endif