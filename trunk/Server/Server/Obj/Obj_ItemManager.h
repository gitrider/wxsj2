/********************************************************************
创建日期:	2008年1月22日
文件名称:	Obj_ItemManager.h
创建人:		李珣

文件功能： 实现对Obj_Item的管理
每个场景一个管理器
修改纪录：

*********************************************************************/
#ifndef			_OBJ_ITEM_MANAGER_H_
#define			_OBJ_ITEM_MANAGER_H_
#include		"Type.h"
#include		"Item.h"

class Scene;
class Obj_Item;
/*
*	Obj_Item 管理器类
*/
class Obj_ItemManager
{

public:
	Obj_ItemManager();
	~Obj_ItemManager();
public:

	VOID				Init();
	//清除数据信息
	VOID				CleanUp( ) ;
	//逻辑接口
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

	//添加Obj_Item 到管理器
	BOOL				Add(Obj_Item*	pItem);

	//移除Obj_Item 到管理器
	BOOL				Remove(ObjID_t ObjID);

	//产生Obj_Item
	Obj_Item*			GenObj_Item();
	//回收Obj_Item
	VOID				RecycleObj_Item(ObjID_t ObjID);

	//设置Obj_ItemManager 所属场景
	VOID				SetScene( Scene* pScene )
	{Assert( pScene ) ;
	m_pScene = pScene ;} ;

	//获得Obj_Item 所属场景
	Scene*				GetScene(){ return m_pScene ; } ;

	/*
	*	改变到指定大小	
	*/
	BOOL				Resize(uint iSize);

	Obj_Item*			GetObj_Item(ObjID_t ObjID);

	BOOL				RemoveAllObj_Item( VOID );
public:



	//计算部分方法
	/*
	*	从怪物产生掉落的物品容器
	*
	*	uint	iMonsterLevel			怪物级别
	*	uint	iKillerLevel			杀怪角色级别
		uint	iMonsterID				怪物ID	
	*	INT		iMonsterType			怪物类型
	*
	*/
	Obj_ItemContaner			CaculateItemDropFromMonster(uint iKillerLevel,
		uint iMonsterID,
		BOOL bTeam);
	/*
	*	
	*/
	Obj_Item*				CreateMonsterDropObj_Item(WORLD_POS* pPos,  _ITEM* item);



	Obj_Item*				CreatePersonDropObj_Item(WORLD_POS* pPos, Item* pItem);
	///*
	//*	生长点Item 的创建
	//*/
	//Obj_Item*				CreateGrowPointObj_Item(INT	IBType);


private:
	/*
	*	逻辑时间计数
	*/
	CMyTimer			m_LogicTimer;
private:
	// Obj_Item id 数组
	ObjID_t*			m_pObj_ItemID ;
	// Obj_ItemManager 最大管理大小
	uint				m_TotalSize ;
	// 当前管理的Obj_Item数量
	uint				m_Count ;
	// 场景指针
	Scene*				m_pScene;
};	

#endif

