/********************************************************************
	创建日期:	2005年11月10日
	创建时间:	10:58
	文件名称:	HumanItemLogic.h
	文件路径:	d:\Prj\Server\Server\Item\HumanItemLogic.h
	创建人:		曾鹏翔
	
	文件功能： Obj_Human 对物品操作的封装
	修改纪录：
	
*********************************************************************/

#ifndef _HUMAN_ITEM_LOGIC_H_
#define _HUMAN_ITEM_LOGIC_H_
#include "Type.h"

class Obj_Human;
class Item;
class ItemContainer;
struct _EXCHANGE_ITEM_LIST ;
struct ITEM_LOG_PARAM;
struct _CREATEITEM_INIT;

class HumanItemLogic
{
public:
	static	ItemContainer*	GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID, INT& nBagIndex);

	//通过物品GUID,取得Human上物品的容器
	static	ItemContainer*	GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID);

	//获得Human身上位置为uIndex的所属容器
	static ItemContainer*	GetBagContainer(Obj_Human*	pHuman,UINT uBagIndex);

	//获得Human身上装备栏容器
	static ItemContainer*	GetEquipContainer(Obj_Human* pHuman);

	static ItemContainer*   GetBagByExtraContainPos(Obj_Human*	pHuman,UINT uExtraIndex);
	//-----------------------------------------------------------------------------------------------------------------
	/*
	*	背包部分
	*/
	/*
	*	背包部分
	*  从Human身上删除uCount个类型为uItemIndex 的物品
	*/
	static	BOOL			EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uItemIndex,UINT uCount);

	/*
	*	背包部分
	*  从Human身上背包pContainer, 删除uCount个类型为uItemIndex 的物品
	*  返回剩余未删除个数
	*/
	static	INT				EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer* pContainer, UINT uItemIndex, INT uCount);

	//从Human身上删除第uBagIndex 位置上的物品
	static	BOOL			EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uBagIndex);

	//减少Human身上uBagIndex 位置上的物品
	static  BOOL			DecBagItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);

	//增加Human身上uBagIndex 位置上的物品,uBagIndex 上面一定要有东西
	static  BOOL			IncBagItemLayCount(Obj_Human*,UINT uBagIndex,UINT uCount =1);

	//pItemContainer指定容器
	//uItemIndex 是指需要计算容纳空间的物品类型
	//Number 是指需要容纳的 uItemIndex 类型的物品个数
	//NewSpace 用于保存容纳 Number 个 uItemIndex 类型的物品需要的新背包空间
	static	INT				CalcBagItemSpace(Obj_Human* pHuman,ItemContainer* pItemContainer, UINT uItemIndex, INT Number,INT& NewSpace);
	
	//uItemIndex 是指需要计算容纳空间的物品类型
	//Number 是指需要容纳的 uItemIndex 类型的物品个数
	//NewSpace 用于保存容纳 Number 个 uItemIndex 类型的物品需要的新背包空间
	static	BOOL			CalcBagItemSpace(Obj_Human* pHuman,UINT uItemIndex,INT Number,INT& NewSpace);

	//计算背包空格总和
	static	INT				CalcBagSpace(Obj_Human* pHuman);

	//计算是否可以容下ItemList
	static	BOOL			CanBagReceiveItemList(Obj_Human* pHuman,_ITEM_LIST& List);

	//接收ItemList
	static	BOOL			ReceiveBagItemList(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,_ITEM_LIST& List);

	//获得Human背包位置为uIndex的Item
	static	Item*			GetBagItem(Obj_Human*	pHuman,UINT uBagIndex);

	//获得Human背包中对应GUID的物品位置
	static	INT				GetBagItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid);

	//获得Human背包中对应ItemIndex的物品位置
	static	INT				GetBagItemPosByType(Obj_Human* pHuman,UINT uItemIndex);

	//获得Human身上物品编号为uItemIndex的统计个数
	static	INT				CalcBagItemCount(Obj_Human* pHuman,UINT uItemIndex);

	//设置human身上物品的耐久度
	static	BOOL			SetBagItemDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	static	BOOL			SetBagItemCurMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	//设置Human背包中物品的鉴定属性
	static	BOOL			SetBagItemIdent(Obj_Human*	pHuman,UINT uBagIndex);

	//设置Human背包中物品的参数
	static	BOOL			SetBagItemParam(Obj_Human* pHuman,UINT uBagIndex,UINT Start,INT Type,INT value);

	//设置human身上物品的最大耐久度
	static	BOOL			SetBagItemMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	//往身上放置uCount个类型为ItemIndex 的物品
	static	BOOL			CreateMultiItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT uCount);

	//往身上放置一个类型为uItemIndex 的物品
	static	BOOL			CreateItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human*	pHuman,UINT uItemIndex,UINT& uBagIndex,UINT uQuality = 1);

	//往身上放置一个类型为uItemIndex,属性为pTemplateItem的物品
	static	BOOL			CreateItemToBag(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman, _CREATEITEM_INIT* pInit, UINT& uBagIndex,UINT uQuality =1);

	//锁定背包内的物品
	VOID	LockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID) ;

	//解锁背包内的物品
	VOID	UnlockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID ) ;

	/*
	*	设置背包中装备的耐久度	
	*
	*	UINT uBagIndex	背包中位置
	*
	*  INT Dur					耐久度点数
	*/
	static	BOOL			SetBagEquipDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);

	/*
	*	设置背包中装备的最大耐久度	
	*
	*	UINT uBagIndex	背包中位置
	*
	*  INT Dur					耐久度点数
	*/
	static	BOOL			SetBagEquipMaxDur(Obj_Human*	pHuman, UINT uBagIndex, INT iDur);

	/*
	*	设置背包中装备的当前最大耐久度	
	*
	*	UINT uBagIndex	背包中位置
	*
	*  INT Dur					耐久度点数
	*/
	static	BOOL			SetCurBagEquipMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur);


	static BOOL				ExchangeBagItem(Obj_Human*	pHuman1,UINT uBagIndex1, Obj_Human* pHuman2, UINT uBagIndex2);
	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	 *	装备部分
	 */
	
	/*
	 *	设置已经装备上物品的耐久度	
	 *
	 *	HUMAN_EQUIP EquipPoint	装配点位置
	 *
	 *  INT Dur					耐久度点数
	 */
	static	BOOL			SetEquipDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*
	*	设置已经装备上物品的最大耐久度	
	*
	*	HUMAN_EQUIP EquipPoint	装配点位置
	*
	*  INT Dur					耐久度点数
	*/
	static	BOOL			SetEquipMaxDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*
	*	设置已经装备上物品的当前最大耐久度	
	*
	*	HUMAN_EQUIP EquipPoint	装配点位置
	*
	*  INT Dur					耐久度点数
	*/
	static	BOOL			SetCurEquipMaxDur(Obj_Human*	pHuman,HUMAN_EQUIP EquipPoint,INT iDur);

	/*	删除角色已经装备上的物品
	 *
	 *	EquipPoint 装备点编号
	 */ 
	static	BOOL			EraseEquip(Obj_Human* pHuman,HUMAN_EQUIP	EquipPoint);

	/*
	 *	获得装备上的物品
	 *	EquipPoint 装备点编号
     */
	static	Item*			GetEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint);
	
	
	//获得Human身上Equip编号为uItemIndex的统计个数
	static	INT				CalcEquipItemCount(Obj_Human* pHuman,UINT uItemIndex);


	//设置Human身上EquipPoint装备点的装备伤害点
	static	BOOL			SetEquipDamagePoint(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, UINT nDamagePoint);
	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	 *	银行部分
	 */
	static	Item*			GetBankItem(Obj_Human* pHuman,UINT BankIndex);
	
	/*
	*	收物品到包中
	*/
	static	BOOL			RecieveItemToBag(Obj_Human* pHuman, ItemContainer* pContainer, UINT uIndex );

	/*
	*	收指定物品到包中
	*/
	static	BOOL			RecieveItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human* pHuman, _ITEM* pItem);

	/*
	*	交易中测试物品能不能被收下
	*/
	static	BOOL			CanReceiveExchangeItemList(Obj_Human* pHuman,_EXCHANGE_ITEM_LIST& List);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	坐骑部分  [7/8/2010 陈军龙]
	*/
	static	ItemContainer*	GetMountContainer(Obj_Human*	pHuman); 
	static	Item*	        GetMount(Obj_Human* pHuman, INT nIndex);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	额外容器部分
	*/
	//从Human身上删除额外容器第uBagIndex 位置上的物品
	static	BOOL			EraseExtraContainerItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,UINT uBagIndex);

	//计算背包空格总和
	static	UINT			CalcExtraContainerSpace(Obj_Human* pHuman,UINT uItemIndex);

	//获得Human背包位置为uIndex的Item
	static	Item*			GetExtraContainerItem(Obj_Human*	pHuman,UINT uBagIndex);

	static	Item*			GetExtraBagFromBagIndex(Obj_Human* pHuman, UINT uBagIndex);
	//获得Human背包中对应GUID的物品位置
	static	UINT			GetExtraContainerItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid);

	//-----------------------------------------------------------------------------------------------------------------
	//
	/*
	*	容器之间
	*/
	//在容器之间移动物品的操作
	static	BOOL			MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer*	pContainer,UINT uIndex, INT& uBagPos);
	
	//在容器之间移动物品的操作
	static	BOOL			MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human*	pHuman,ItemContainer*	pContainer,UINT uIndex, Obj_Human* pDestMan, INT& uBagPos);

	//在容器之间切分物品

	static BOOL				SplitItem(Obj_Human* pHuman,
		ItemContainer* pSourContainer, 
		UCHAR uSourIndex,  
		UCHAR uCount,  
		ItemContainer* pDestContainer,
		INT nDestIndex /* =INVALID_INDEX  */,
        INT opType = 0);

	//在容器之间合并物品
	static	BOOL			SpliceItem(Obj_Human* pHuman,
		ItemContainer* pSourContainer, 
		UCHAR uSourIndex,
		ItemContainer* pDestContainer,
		UCHAR uDestIndex);
};


#endif