/********************************************************************
	创建日期:	2005年11月2日
	创建时间:	16:55
	文件名称:	ItemTypes.h
	文件路径:	d:\Prj\Server\Server\Item\ItemTypes.h
	创建人:		曾鹏翔
	
	文件功能： 物品常量\宏定义等	类型数据
	修改纪录：
	
*********************************************************************/

#ifndef _ITEM_TYPES_H_
#define _ITEM_TYPES_H_

#include "Type.h"
//物品系统常量定义


//ItemBox 池最大值
#define		MAX_ITEM_BOX_POOL					5000

//Item 池最大值
#define		MAX_OBJ_ITEM_POOL					10000

//最大属性个数
#define		MAX_ATT_COUNT						1000

//最大掉落包容量
#define		MAX_DROPBOX_CARRAGE					100

//最大怪物掉落
#define		MAX_MONSTER_DROPBOX					10

//ItemBox 掉落位置随机范围
#define		ITEMBOX_DROP_RANGE					10

//最大ItemQuality 表 项目个数(内部使用)
#define		MAX_ITEM_TABLE_QUALITYS				10

//最大ItemValue 表 项目个数
#define		MAX_ITEM_TABLE_VALUES				10

#define     MAX_MONSTER_VALUE					100000000


//怪物掉落结构
struct MONSTER_DROPBOXS 
{
	MONSTER_DROPBOXS()
	{
		memset(m_DropBox,0,sizeof(INT)*MAX_MONSTER_DROPBOX);
	}
	INT		m_DropBox[MAX_MONSTER_DROPBOX];
};

//怪物掉落结构
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

//怪物掉落结构
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