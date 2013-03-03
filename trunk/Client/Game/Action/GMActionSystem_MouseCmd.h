
/** GMActionSystem_MouseCmd.h
 *	
 *	功能：	操作管理器-鼠标指令
 *			用于处理诸如鉴定，修理，加为好友等特殊鼠标状态
 *
 *	修改记录：
 *			080421	CActionItem_MouseCmd_Friend
 *					CActionItem_MouseCmd_Exchange
 *			
 *			
 */

#pragma once


#include "GMActionSystem.h"


//-------------------------------------
//修理
class CActionItem_MouseCmd_Repair : public CActionItem
{
public:

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_REPARE; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_REPARE;}

protected:

	static CActionItem_MouseCmd_Repair*	s_pMe;

public:

	CActionItem_MouseCmd_Repair(INT id);
	virtual ~CActionItem_MouseCmd_Repair();

	static CActionItem_MouseCmd_Repair*	GetMe(VOID) { return s_pMe; }

};

//-------------------------------------
//鉴定
class CActionItem_MouseCmd_Identify : public CActionItem
{
public:

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_IDENTIFY; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_IDENTIFY;}
	//设置Default状态(只能通过ActionSystem调用)
	virtual VOID			SetDefaultState(BOOL bDefault);

protected:

	static CActionItem_MouseCmd_Identify*	s_pMe;

public:

	CActionItem_MouseCmd_Identify(INT id);
	virtual ~CActionItem_MouseCmd_Identify();

	static CActionItem_MouseCmd_Identify*	GetMe(VOID) { return s_pMe; }
};


////////////////////////////////////////////////////////////////////////////////////
// CActionItem_ChangeSuit
// 一键换装上图标区要拖拽到快捷栏
////////////////////////////////////////////////////////////////////////////////////

class CActionItem_ChangeSuit : public CActionItem
{
public:
	CActionItem_ChangeSuit(INT nID);
	virtual ~CActionItem_ChangeSuit();

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_CHANGE_SUIT;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_CHANGE_SUIT; }
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID)			{ return -1;	}
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)&m_idSuit;	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//得到所在容器的索引
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID)		{ return TRUE; }
	//刷新
	virtual VOID			Update(tActionReference* pRef) {}
	
	virtual VOID			UpdateSuit(STRING& iconName, INT suitId);
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID)	{ return TRUE; }
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	virtual void			SpecialOperation(void* pData);

protected:

	INT		m_idSuit;	// 第几套套装
};


//-------------------------------------
// 添加好友鼠标命令
class CActionItem_MouseCmd_Friend : public CActionItem
{
public:

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_MOUSECMD_ADDFRIEND; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_ADDFRIEND;}

protected:

	static CActionItem_MouseCmd_Friend*	s_pMe;

public:

	CActionItem_MouseCmd_Friend(INT id);
	virtual ~CActionItem_MouseCmd_Friend();

	static CActionItem_MouseCmd_Friend*	GetMe(VOID) { return s_pMe; }

};

//-------------------------------------
// 交易
class CActionItem_MouseCmd_Exchange : public CActionItem
{
public:

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_MOUSECMD_EXCHANGE; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_EXCHANGE;}

protected:

	static CActionItem_MouseCmd_Exchange*	s_pMe;

public:

	CActionItem_MouseCmd_Exchange(INT id);
	virtual ~CActionItem_MouseCmd_Exchange();

	static CActionItem_MouseCmd_Exchange*	GetMe(VOID) { return s_pMe; }
};


//-------------------------------------

