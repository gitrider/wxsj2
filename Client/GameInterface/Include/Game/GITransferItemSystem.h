#pragma once
#include "GINode.h"
/*
|
|	物品信息传输系统,将物品转化为文字信息通过聊天来发送
|
*/
enum ElementType
{
	ELEMENT_INVALID = -1,

	ELEMENT_ITEMINFO,			//物品信息
	ELEMENT_HYPERLINK,			//超链
	ELEMENT_CHANNEL,			//频道切换
	ELEMENT_PET,				//珍兽
	ELEMENT_BOSS,				//BOSS
};

enum ElementAction
{
	ELEMENT_R_CLICK,			//右键点击
	ELEMENT_L_CLICK,			//左键点击
	ELEMENT_MOVE_IN,			//鼠标移进
	ELEMENT_MOVE_OUT,			//鼠标移出
};

class tObject_Item;
class KYLIN_API tTransferItemSystem : public tNode
{
public:
	//管理数据结构
	struct Element
	{
		int nID;			//ID
		int nType;			//类型

		int nObjID;				//物品ID
		STRING strDisplay;		//显示名称

		STRING strOrgDisplay;	//没有经过修改的原始字串
		STRING strContents;		//用于传输的字符串

		int useCount;		//使用计数，当减到0的时候，这个Element删除

		Element():nType(ELEMENT_INVALID){}
		virtual ~Element(){}
		BOOL				IsType(int nT){return (nType == nT)?TRUE:FALSE;}

		// 使用_DBC_SYSTEM_COLOR中的配置
		virtual STRING	GetDisplayColor() = 0;
		virtual STRING	GetDisplayPretxt() = 0;
	};

public:
	//将一个物品转化为字符串
	virtual Element*		ConvertFromItem(tObject_Item* pItem) = 0;
	//将一个珍兽转化为珍兽
	virtual Element*		ConvertFromPet(INT nIdx) = 0;
	//将字符串转化为物品
	virtual Element*		ConvertFromString(const STRING& strDisplay, const STRING& strContents) = 0;

	//添加一个HyperLink
	virtual Element*		AddHyperLink(const STRING& strName, const STRING& strContex,INT nCh = -1/*CHAT_TYPE_INVALID*/) = 0;
	//添加一个频道
	virtual Element*		AddChannel(INT nType, const STRING& strContex) = 0;

	//销毁一个数据结构
	virtual void			DeleteElement(int nID) = 0;
	//处理Element
	virtual void			DoElement(int nID, int Action,int msgID=-1) = 0;
	//判断Element是否存在
	virtual BOOL			IsElementExist(int nID) = 0;
	//获得Element的类型
	virtual int				GetElementType(int nID) = 0;
	//获得Element的显示名称
	virtual STRING			GetElementDisplayName(int nID) = 0;
	//获得Element的原始名称
	virtual STRING			GetOrgElementDisplayName(int nID) = 0;
	//获得Element的内容
	virtual STRING			GetElementContents(int nID) = 0;
	//增加Element的使用计数
	virtual VOID			AddElementUseCount(int nID)	= 0;
	//获得当前选中的Element
	virtual Element*		GetCurElement() = 0;

	virtual VOID	ReSet() = 0;
	KL_DECLARE_DYNAMIC(tTransferItemSystem);
};