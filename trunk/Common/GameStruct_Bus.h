// GameStruct_Bus.h

#ifndef __GAMESTRUCT_BUS_H__
#define __GAMESTRUCT_BUS_H__

#include "Type.h"

// <=此值为无效高度
#define DEF_BUS_PATH_NODE_INVALID_Y		(-100000.f)

// 是大乘客数量
#define DEF_BUS_MAX_PASSENGER_COUNT		(8)

// 提供给脚本用的参数数量
#define DEF_BUS_SCRIPT_PARAM_BUF_SIZE	(8)


struct _BUS_SEAT_INFO
{
	INT		m_nActionSetIndex;			// 在CharModel.txt中的动作组索引
	CHAR	*m_pszLocator;				// 乘客坐位的绑定点名称
};

enum ENUM_BUS_TYPE
{
	BUS_TYPE_INVALID	= -1,
	BUS_TYPE_ONE_WAY,				// 单程车
	BUS_TYPE_SHUTTLE,				// 往返车

	BUS_TYPE_NUMBERS
};

struct _BUS_INFO
{
	INT				m_nDataID;					// 数据编号
	CHAR			*m_pszName;					// 名称
	INT				m_nScriptID;				// 脚本ID
	INT				m_nType;					// 类型 ENUM_BUS_TYPE
	INT				m_nStopTime;				// 停站时间
	INT				m_nModelID;					// 模型数据ID
	FLOAT			m_fModelHeight;				// 模型高度
	INT				m_nMaxPassengerCount;		// 准乘人数
	FLOAT			m_fMoveSpeed;				// 行驶速度
	_BUS_SEAT_INFO	m_aSeatInfo[DEF_BUS_MAX_PASSENGER_COUNT];	// 乘客坐位的绑定点名称

	_BUS_INFO(VOID)
	{
		CleanUp();
	}

	VOID CleanUp(VOID)
	{
		m_nDataID					= INVALID_ID;
		m_nScriptID					= INVALID_ID;
		m_pszName					= NULL;
		m_nType						= BUS_TYPE_INVALID;
		m_nStopTime					= 0;
		m_nModelID					= INVALID_ID;
		m_fModelHeight				= -1.f;
		m_nMaxPassengerCount		= -1;
		m_fMoveSpeed				= -1.f;
		memset(m_aSeatInfo, 0, sizeof(m_aSeatInfo));
	}
};

/////////////////////////////////////////////////////////////////////
//坐骑表
/////////////////////////////////////////////////////////////////////
//
//struct _DBC_CHARACTER_MOUNT
//{
//	INT				m_nID;
//	LPCSTR			m_pszName;					// 骑乘名称
//	INT				m_nMaxPassengerCount;		// 准乘人数
//	INT				m_nModelID;
//	FLOAT			m_fAddHeight;
//	INT				m_nRecipe;					// 对应的骑乘配方
//	INT				m_nMainActionFileIndex;		// 主动作索引
//	LPCSTR			m_pszMainAttachLocator;		// 主绑定点
//	INT				m_nSecondActionFileIndex;	// 副动作索引
//	LPCSTR			m_pszSecondAttachLocator;	// 副绑定点
//	INT				m_nMoveSoundID;				// 坐骑的移动声音ID
//	INT				m_nCanJump;					// 是否能跳跃 -1不能跳跃 1能跳跃
//
//	_DBC_CHARACTER_MOUNT(VOID)
//	{
//		CleanUp();
//	}
//
//	VOID CleanUp(VOID)
//	{
//		m_nID = INVALID_ID;
//		m_pszName = NULL;
//		m_nMaxPassengerCount = 0;
//		m_nModelID = INVALID_ID;
//		m_fAddHeight = -1.f;
//		m_nRecipe = INVALID_ID;
//		m_nMainActionFileIndex = -1;
//		m_pszMainAttachLocator = NULL;
//		m_nSecondActionFileIndex = -1;
//		m_pszSecondAttachLocator = NULL;
//		m_nMoveSoundID = INVALID_ID;
//		m_nCanJump = -1;
//	}
//};

#endif // __GAMESTRUCT_BUS_H__
