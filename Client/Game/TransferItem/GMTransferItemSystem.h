/****************************************\
*										*
* 			  物品信息传输系统			*
*										*
*						 by fangyuan		*
\****************************************/

#pragma once
#include "GITransferItemSystem.h"
#include "../DataPool/GMDP_Struct_Pet.h"

struct SDATA_PET;
class tObject_Item;

class ItemElement : public tTransferItemSystem::Element
{
public:
	ItemElement();
	virtual ~ItemElement(){}

	tObject_Item*	pData;
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt();
public:
	//查询Tooltip的物品的蓝色属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	STRING		GetItemExtBlueAttrInfo();

	// 
	// 得到蓝色装备是否鉴定
	// 
	INT	IsBlueEquipIdentify();
};

class HyperLinkElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt(){return "";}

	HyperLinkElement();
	virtual ~HyperLinkElement(){}
public:
	INT		m_nChannelType;				//对应的频道类型
};

class ChannelElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor(){return "";}
	virtual STRING		GetDisplayPretxt(){return "";}

	ChannelElement();
	virtual ~ChannelElement(){}
public:
	INT		m_nChannelType;				//对应的频道类型
};

class PetElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor();
	virtual STRING		GetDisplayPretxt();

	PetElement();
	virtual ~PetElement(){}
public:
	SDATA_PET		m_Pet;
};

class BossElement : public tTransferItemSystem::Element
{
public:
	virtual STRING		GetDisplayColor(){return "";}
	virtual STRING		GetDisplayPretxt(){return "";}

	BossElement();
	virtual ~BossElement(){}
public:
	INT		m_nBossID;					//对应的BOSS的ID
};

class CTransferItemSystem : public tTransferItemSystem
{
	/******************************************
		功能接口
	*******************************************/
public:
	//将一个物品转化为字符串
	virtual Element*		ConvertFromItem(tObject_Item* pItem);
	//将一个珍兽转化为珍兽
	virtual Element*		ConvertFromPet(INT nIdx);
	//将字符串转化为物品
	virtual Element*		ConvertFromString(const STRING& strIn, const STRING& strContents);
	
	//添加一个HyperLink
	virtual Element*		AddHyperLink(const STRING& strName, const STRING& strContex, INT nCh = -1/*CHAT_TYPE_INVALID*/);
	//添加一个频道
	virtual Element*		AddChannel(INT nType, const STRING& strContex);
	//销毁一个数据结构
	virtual void			DeleteElement(int nID);

	//处理Element
	virtual void			DoElement(int nID, int Action,int msgID = -1);
	//判断Element是否存在
	virtual BOOL			IsElementExist(int nID);
	//获得Element的类型
	virtual int				GetElementType(int nID);
	//获得Element的显示名称
	virtual STRING			GetElementDisplayName(int nID);
	//获得Element的原始名称
	virtual STRING			GetOrgElementDisplayName(int nID);
	//获得Element的内容
	virtual STRING			GetElementContents(int nID);
	//增加Element的使用计数
	virtual VOID			AddElementUseCount(int nID);
	//获得当前选中的Element
	virtual Element*		GetCurElement(){return m_pCurElement;}
protected:
	
	//将字串转化为物品
	Element*				ConvertItemFromString(const STRING& strIn, const STRING& strContents);

	//将字串转化为珍兽
	Element*				ConvertPetFromString(const STRING& strIn, const STRING& strContents);

	//将字串转化为BOSS链接信息
	Element*				ConvertBossFromString(const STRING& strIn, const STRING& strContents);

	/******************************************
		内部数据
	*******************************************/
private:
	typedef std::map< int, Element* > ElementMap;

	ElementMap				m_mapElements;	//所有数据

	int						m_nIdFactory;

	Element*				m_pCurElement;	//当前选中的Element
	/******************************************
	内部函数
	*******************************************/
	Element*				GetElement(int nID);

	STRING					GetDisplayColour(Element& el);
	STRING					GetDisplayPrefix(Element& el);

	// 新物品系统相关修改[12/7/2006]
	static BOOL				GenItemBinStr( tObject_Item* pItem, STRING& strContents);
	static BOOL				GenBinStrItem(tObject_Item*& pItem, const STRING& strContents);
	/******************************************
		核心继承
	*******************************************/
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	ReSet(VOID);
public:
	CTransferItemSystem();
	virtual ~CTransferItemSystem();
	static CTransferItemSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CTransferItemSystem*		s_pMe;

	KL_DECLARE_DYNAMIC(CTransferItemSystem);
};
