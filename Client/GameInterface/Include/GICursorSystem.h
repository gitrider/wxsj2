
/** GICursorSystem.h
 */

#pragma once


#include "GINode.h"



/// 指针类型
enum ENUM_CURSOR_TYPE
{
	CURSOR_WINBASE = 0,

	CURSOR_NORMAL,			/// 普通
	CURSOR_ATTACK,			/// 攻击
	CURSOR_AUTORUN,			/// 自动行走中
	CURSOR_PICKUP,			/// 拾取物品
	CURSOR_UNREACHABLE,		/// 无法到达的区域
	CURSOR_MINE,			/// 采矿
	CURSOR_HERBS,			/// 采药
	CURSOR_SPEAK,			/// 谈话
	CURSOR_INTERACT,		/// 齿轮
	CURSOR_REPAIR,			/// 修理
	CURSOR_HOVER,			/// 鼠标激活(挂接物品...)
	CURSOR_IDENTIFY,		/// 鼠标鉴定
	CURSOR_RING,			/// 技能环
	CURSOR_DIRECTION,		///	方向技能

	CURSOR_NUMBER,
};


class KYLIN_API tCursorSystem : public tNode
{
public:

	/** 设置鼠标光标 */
	virtual VOID SetCursor(ENUM_CURSOR_TYPE nType) = 0;
	/** 得到当前光标 */
	virtual HCURSOR GetCursor(VOID) = 0;

	/** 进入UI控制模式 */
	virtual VOID EnterUICursor(HCURSOR hCursor) = 0;
	virtual VOID LeaveUICursor(VOID) = 0;

	/** 响应WM_SETCURSOR */
	virtual VOID OnSetCursor(VOID) = 0;

	/** 显示/隐藏鼠标 */
	virtual VOID ShowCursor(BOOL bShow) = 0;

protected:

	KL_DECLARE_DYNAMIC(tCursorSystem);

};	// class

