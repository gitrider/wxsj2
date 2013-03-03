// 增加了处理宝石镶嵌以及物品属性加减的函数，fancy
#ifndef _HUMAN_ITEM_EXTRABAG_H_
#define _HUMAN_ITEM_EXTRABAG_H_
#include "ItemContainer.h"


class HumanDB;

struct _HUMAN_ITEMEXTRABAG_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	Item*					m_pExtraBagItem;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;
};

class Obj_Human;

class HumanItemExtraBag:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	Item*					m_pExtraBagItem;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;

public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//初始化容器

	//容器编号到Bag编号
	virtual		UINT		ConIndex2BagIndex(UINT uConIndex);
	//背包编号到容器编号
	virtual		UINT		BagIndex2ConIndex(UINT uBagIndex);
	//测试传入的一个值是不是能够在这个Con的范围之中
	virtual		BOOL		IsInContainer(UINT uBagIndex);

	virtual		BOOL		IsCanUse();
	
	virtual		BOOL		IsValid();

	virtual		INT			GetContainerType();

	//计算容器空格个数
	virtual		UINT		CalcItemSpace();	

	//判断容器是否是空
	virtual		BOOL		IsEmpty();

	virtual		INT		    GetContainerSize();

	//交易中计算容器空格个数
	//交易时的物品被交易锁锁住，计算空间时忽略被交易锁锁住的物品，这样物品就可以交换了。
	virtual		UINT		CalcExchangeItemSpace();	

	//取得一个空的物品位置, 如果返回INVALID_INDEX表明满了
	virtual		INT			GetEmptyItemIndex( ) ;

	virtual		BOOL		SetItemPWLock(INT nIndex,BOOL bLock);

	//通过物品的类型取得此物品在容器中的位置
	virtual		INT			GetIndexByType( const UINT uType ) ;

	//通过物品的类型取得此物品在容器中的位置
	virtual		INT			GetIndexByType( const UINT uType ,UINT emptyCount) ;

	//通过容器的位置取得物品
	virtual		Item*		GetItem( const INT nIndex ) ;
	
	//取得一个没满的物品位置,如果INVALID_INDEX 则表示没找到
	virtual		INT			GetNoFullItemIndex( UINT uItemIndex);

	virtual		BOOL		ChangeSize(const INT nSize);
protected :
	//设置物品的属性
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//设置物品重叠数量
	virtual		BOOL		SetItemLayCount(INT nIndex, INT nCount) ;
	//减少物品重叠数量
	virtual		BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//增加物品重叠数量
	virtual		BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//删除物品
	virtual		BOOL		EraseItem(UINT uIndex);
	//设置物品耐久度
	virtual	BOOL			SetItemDur(INT nIndex, INT nDur);
	//设置物品损伤点
	virtual	BOOL			SetItemDamagePoint(INT nIndex, INT nPoint);
	//设置物品最大耐久度
	virtual	BOOL			SetItemMaxDur(INT nIndex, INT nDur);

	//设置物品耐久度
	virtual		BOOL		SetItemCurMaxDur(INT nIndex, INT nDur);

	//设置物品属性
	virtual		BOOL		SetItemValue(INT nIndex,_ITEM*	pItem);
	//设置物品帮定
	virtual		BOOL		SetItemBind(INT nIndex);
	//设置物品鉴定信息
	virtual		BOOL		SetItemIdent(INT nIndex);
	//设置制造者信息
	virtual		BOOL		SetItemCreator(INT nIndex,const CHAR* CreatorName);
	//增加物品属性
	virtual		BOOL		AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除物品属性
	virtual		BOOL		DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除宝石信息
	virtual		BOOL		DelGemInfo(INT nIndex,INT GemIndex);
	//添加宝石信息
	virtual		BOOL		AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);

	virtual		INT			GetIndexByGUID( const _ITEM_GUID* pGuid );
protected :
	/*
	*	PET 使用方法
	*/
	virtual		INT			GetIndexByGUID( const PET_GUID_t* pGuid );
	//设置物品GUID
	virtual		BOOL		SetItemGuid(INT nIndex,PET_GUID_t* pGUID);
	//设置PET 属性
	//设置物品属性
	virtual	BOOL		SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet);

	virtual BOOL  		SetPetGUID(INT nIndex,PET_GUID_t GUID) ;				//设置宠物GUID
	virtual BOOL  		SetSpouseGUID(INT nIndex,PET_GUID_t GUID) ;				//设置宠物配偶GUID
	virtual BOOL		SetDataID(INT nIndex,INT ID);							//设置宠物模型		
	virtual BOOL 		SetName(INT nIndex,const CHAR* pName);					//设置名字
	virtual BOOL   		SetNick(INT nIndex,const CHAR* pNick);					//设置昵称
	virtual BOOL  		SetLevel(INT nIndex,INT level);							//设置等级
	virtual BOOL  		SetTakeLevel(INT nIndex,INT takeLevel);					//设置携带等级
	virtual BOOL  		SetAttackType(INT nIndex,INT attackType);				//设置进攻类型（物/法）
	virtual BOOL  		SetAIType(INT nIndex,INT AIType);						//设置AI类型
	virtual BOOL  		SetCampData(INT nIndex,const _CAMP_DATA* pCamp);		//设置阵营
	virtual BOOL  		SetPetType(INT nIndex,BYTE PetType);					//宝宝，变异，野生
	virtual BOOL  		SetGeneration(INT nIndex,BYTE Gen);						//几代宠
	virtual BOOL  		SetHappiness(INT nIndex,BYTE byHappiness);				//快乐度		
	virtual BOOL  		SetStrengthPer(INT nIndex,INT strper);					//力量资质	
	virtual BOOL  		SetSmartnessPer(INT nIndex,INT conper);					//敏捷资质	
	virtual BOOL   		SetMindPer(INT nIndex,INT dexper);						//智力资质
	virtual BOOL   		SetConstitutionPer(INT nIndex,INT intper);				//体质资质
	virtual BOOL		SetGenGu(INT nIndex,INT gengu);							//根骨
	virtual BOOL		SetSavvy(INT nIndex,INT iSavvy);						//悟性
	virtual BOOL  		SetGrowRate(INT nIndex,FLOAT rate);						//成长率
	virtual BOOL  		SetRemainPoint(INT nIndex,INT rPoint);					//一级属性剩余点数
	virtual BOOL  		SetExp(INT nIndex,INT exp) ;							//经验值
	virtual BOOL  		SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value);//基础一级战斗属性（不包括技能和装备增加的部分）
	virtual BOOL  		SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill);	//宠物技能

private:
	BOOL					SetDBDirty(INT OffSet);
};



#endif