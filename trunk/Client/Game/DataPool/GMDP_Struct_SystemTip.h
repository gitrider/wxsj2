
/**	GMDP_Struct_SystemTip.h
 *
 *	功能：	系统提示相关的数据
 *
 *	修改记录：	
 *					
 */

#pragma once


#include "Type.h"



// 事件类型
enum SYSTIP_TYPE
{
	STT_INVALID	= -1,	// 无效

	STT_LEVELUP,		// 升级
	STT_NEWTIP,			// 新手提示
	STT_SKILL,			// 技能学习
	STT_TEAMINVITE,		// 队伍邀请
	STT_EXCHANGEINVITE,	// 交易邀请
	STT_FAMILYINVITE,	// 家族邀请
	STT_GUILDINVITE,	// 帮会邀请
	STT_SYSTEMTIP,		// 系统提示
	STT_MISSTIP,		// 任务提示
	STT_FRIENDCHAT,		// 好友聊天消息
	STT_EQUIPTIP,		// 耐久提示（低于10，5 或等于0）

	STT_NUM,			// 数量
};

// 每个事件的内容
struct SystemTipEvent
{
	SYSTIP_TYPE	m_eType;		// 事件的类型
	INT			m_nIndex;		// 在界面中的索引
	INT			m_nLevelIndex;	// 如果存在多个事件在同一图标上， 则当前的事件所在的层级
	STRING		m_strTipDesc;	// 显示的提示信息
	BOOL		m_bDispose;		// 是否已处理， 处理了就不再显示
	
	INT			m_nParameter;	// 参数， 给组队使用


	SystemTipEvent()
	{
		m_eType			= STT_INVALID;
		m_nIndex		= -1;
		m_nLevelIndex	= -1;
		m_strTipDesc	= "";
		m_bDispose		= FALSE;

		m_nParameter	= -1;
	};
};


