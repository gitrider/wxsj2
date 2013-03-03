/********************************************************************
创建日期:	2008年1月22日
文件名称:	Obj_ItemPool.h
创建人:		李珣

文件功能： 池化游戏中掉落物品数据
修改纪录：

*********************************************************************/
#ifndef		_ITEM_POOL_H_
#define		_ITEM_POOL_H_
#include	"Type.h"

#define		ITEM_POOL_INVAILD	-1				//无效Obj_Item编号


class Obj_Item;



/*
*		Obj_Item	池类
*/
class Obj_ItemPool
{
	//Obj_Item数组
	Obj_Item*		m_pObj_Item;
	//空闲Obj_Item池Id
	INT*			m_pFreeObj_ItemID;
	//使用的Obj_Item池Id数组
	INT*			m_pUseObj_ItemID;
	//空闲位置标记
	INT				m_nFreePosition;
	//共享锁
	MyLock			m_Lock;



public:
	Obj_ItemPool();
	~Obj_ItemPool();
	/*
	*	完成Obj_ItemPool 初始化,在Server->Init中执行
	*/
	BOOL			Init();

	/*
	*	产生一个新的Obj_Box
	*
	*	
	*/

	Obj_Item*		NewObj_Item();

	/*	删除一个Obj_Itembox
	*	
	*	
	*
	*/
	//根据位置删除
	VOID			DelObj_Item(uint iItemPos);
	//根据指针删除
	VOID			DeleteObj(Obj_Item*	pItem);


};

extern Obj_ItemPool* g_pObj_ItemPool;


#endif
