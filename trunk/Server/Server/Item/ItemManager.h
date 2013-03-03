/********************************************************************
创建日期:	2005年11月1日
创建时间:	10:28
文件名称:	ItemManager.h
文件路径:	d:\Prj\Server\Server\Item\ItemManager.h
创建人:		曾鹏翔

文件功能：	对物品的生成销毁,传递进行管理类
修改纪录：

*********************************************************************/

#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_


#include "Type.h"
#include "LuaInterface.h"
#include "LogDefine.h"

class Scene;
class Obj_Human;
struct _CREATEITEM_INIT;

class ItemManager
{

	MyLock			m_Lock;

public:

	//物品管理器初始化
	BOOL			Init();

	//产生一个唯一的物品Serial
	INT				GenItemSerial();

	//产生一个唯一物品GUID
	BOOL			GenItemGuid(_ITEM_GUID& ig);

	/*	
	 *	根据_ITEM_TYPE 生成 _ITEM
	 *	iQuality 是蓝色装备的品质段 
	 */
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE it,_ITEM& item,INT iQuality=1);

	/*	
	 *	根据 ItemSerial 生成 _ITEM
	 *  iQuality 是蓝色装备的品质段 
	 */
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam,UINT ItemSerial,_ITEM& item,INT iQuality=1);

	
	/*	
	*	根据 _CREATEITEM_INIT 生成 _ITEM
	*  pInit 是初始化数据 
	*/
	BOOL			CreateItem(ITEM_LOG_PARAM* pLogParam, _ITEM& item, _CREATEITEM_INIT* pInit=NULL);
protected:
private:
	/*
	 *	内部使用分类物品生成方法
	 */
	//创建装备
	BOOL			_CreateEquipFromTable(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE  it,INT iQuality,_ITEM& item);
	//创建宝石
	BOOL			_CreateGemFromTable(_ITEM_TYPE it,_ITEM& item);
	//创建宝石
	BOOL			_CreateGemFromTable(UINT	TableIndex,_ITEM& item);
	//创建普通道具
	BOOL			_CreateMedicFromTable(_ITEM_TYPE it,_ITEM& item);

	BOOL			_CreateTaskFromTable(_ITEM_TYPE it,_ITEM& item);
	
	//创建藏宝图
	BOOL			_CreateStoreMapFromTable(_ITEM_TYPE	it,_ITEM& item);
	
	//创建额外包裹
	BOOL			_CreateExtraBagFromTable(_ITEM_TYPE	it,_ITEM& item);

	//创建魂珠类道具add by gh for Soulbead 2010/06/28
	BOOL			_CreateSoulBeadFromTable(_ITEM_TYPE	it,_ITEM& item);
	//创建坐骑 [7/14/2010 陈军龙]
	BOOL	        _CreateMountFromTable(_ITEM_TYPE	it,_ITEM& item);
	//创建装备
	BOOL			_CreateEquipFromTable(ITEM_LOG_PARAM* pLogParam, _ITEM& item, _CREATEITEM_INIT* pInit);
	//创建宝石
	BOOL			_CreateGemFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//创建普通道具
	BOOL			_CreateMedicFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	BOOL			_CreateTaskFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//创建藏宝图
	BOOL			_CreateStoreMapFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//创建额外包裹
	BOOL			_CreateExtraBagFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);

	//创建坐骑
	BOOL	        _CreateMountFromTable(_ITEM& item, _CREATEITEM_INIT* pInit);
	//物品生成序列号
	INT				m_nLastGenItemSerial;
	
	//世界编号
	ID_t			m_WorldId;
	//服务器编号
	ID_t			m_ServerId;
	//共享内存访问单元
	ItemSerialKeySMU*	m_pSerialSMU;
};

BOOL EquipCanAddHole(EQUIP_TYPE equip_type);

//全局唯一物品管理器
extern	ItemManager*		g_pItemManager;
#endif