// CharacterStruct.h
//
//////////////////////////////////////////////////////

#ifndef __CHARACTERSTRUCT_H__
#define __CHARACTERSTRUCT_H__

#pragma once

#include "Type.h"
#include <map>
#include "GameStruct_Impact.h"


// 20100415 AddCodeBegin
struct  NPC_AUTOPATH	// NPC自动寻路功能使用结构
{
	BOOL   m_isAutoPath;		// 是否处于自动寻路状态，用于角色移动停止后判断使用
	STRING m_strAutoNPCName;    // 自动寻路的NPC名称
	INT	   m_nSceneID;			//那个场景的NPC

	NPC_AUTOPATH()
	{
		m_strAutoNPCName = "";
		m_isAutoPath = FALSE;
		m_nSceneID = -1;
	};
	
};
// 20100415 AddCodeEnd

// 特效组
struct SEffectSet
{
protected:
	UINT	m_uEffect;	// 特效句柄

public:
	SEffectSet( VOID )
	{
		m_uEffect = NULL;
	}

	virtual VOID Reset( VOID )
	{
		m_uEffect = 0;
	}

	VOID SetEffect( UINT uEffect )
	{
		m_uEffect = uEffect;
	}

	UINT GetEffect( VOID )const
	{
		return m_uEffect;
	}
};

struct SImpactEffect :	public SEffectSet
{
protected:

	INT			m_nNumOfLayer;				// 累积的层数，buff的层数累积用	
	INT			m_nImpactID;				// 对应effect表中的标示id
	BOOL		m_bLineEffect;				// 连线的特效
	BOOL		m_bRotateEffect;			// 旋转特效
	ObjID_t		m_CreatorID;				// 创建者ID
	LPCSTR		m_lpszCreatorLocator;		// 创建者的绑定点

	std::vector<INT> m_vBulletIndexList;	// 子弹列表,旋转特效用

	BOOL		m_bShowEffect;				// 
	UINT		m_nBeginTime;				// 开始计算延迟时间
	UINT		m_nDelayTime;				// 延迟
	INT			m_nArrayIndex;				// 在保存数组中的索引

	time_t		m_nTimeNow;		// 持续buff当前倒计时秒数的起始时间 20100531 BLL
	INT			m_nTimeElapse;	// 持续buff的剩余秒数

public:

	SImpactEffect( VOID )
	{
		m_nNumOfLayer			= 1;		
		m_nImpactID				= INVALID_ID;
		m_bLineEffect			= FALSE;
		m_bRotateEffect			= FALSE;
		m_CreatorID				= INVALID_ID;
		m_lpszCreatorLocator	= NULL;
		m_bShowEffect			= FALSE;
		m_nBeginTime			= INVALID_ID;
		m_nDelayTime			= INVALID_ID;
		m_nArrayIndex			= INVALID_ID;

		m_nTimeNow   = 0;

		m_nTimeElapse = 0;

		m_vBulletIndexList.clear();
	}

	virtual VOID Reset( VOID )
	{
		SImpactEffect();		
		SEffectSet::Reset();
	}	

	VOID    SetLayerNum( INT nNumOflayer )				{ m_nNumOfLayer = nNumOflayer; }
	INT		GetLayerNum( VOID )	const					{ return m_nNumOfLayer; }

	VOID	SetImpactID( INT nImpactID )				{ m_nImpactID = nImpactID; }
	INT		GetImpactID( VOID )const					{ return m_nImpactID; }	

	BOOL	IsLineEffect(VOID)const						{ return m_bLineEffect; }
	VOID	SetLineEffect(BOOL bSet)					{ m_bLineEffect = bSet; }

	BOOL	IsRotateEffect(VOID)const					{ return m_bRotateEffect; }
	VOID    SetRotateEffect(BOOL bSet)					{ m_bRotateEffect = bSet; }

	ObjID_t	GetCreatorID(VOID)const						{ return m_CreatorID; }
	VOID	SetCreatorID(ObjID_t nCreatorID)			{ m_CreatorID = nCreatorID; }

	LPCSTR	GetCreatorLocator(VOID)const				{ return m_lpszCreatorLocator; }
	VOID	SetCreatorLocator(LPCSTR lpszCreatorLocator){ m_lpszCreatorLocator = lpszCreatorLocator; }

	std::vector<INT>& GetBulletIndexList( VOID )		{ return m_vBulletIndexList; }

	VOID	SetShowEffect( BOOL bShowEffect )			{ m_bShowEffect = bShowEffect; };
	BOOL	GetShowEffect( VOID )						{ return m_bShowEffect; };

	VOID	SetBeginTime( UINT nBeginTime )				{ m_nBeginTime = nBeginTime; };
	UINT	GetBeginTime( VOID )						{ return m_nBeginTime; };

	VOID	SetDelayTime( UINT nDelayTime )				{ m_nDelayTime = nDelayTime; };
	UINT	GetDelayTime( VOID )						{ return m_nDelayTime; };

	VOID	SetArrayIndex( INT nArrayIndex )			{ m_nArrayIndex = nArrayIndex; };
	INT		GetArrayIndex( VOID )						{ return m_nArrayIndex; }

	// 持续buff的剩余秒数   20100531 BLL
	VOID	SetContinuance(Time_t nContinuance) {m_nTimeElapse=nContinuance;};
	INT		GetContinuance(VOID) const {return m_nTimeElapse;};

	// 持续buff当前倒计时秒数的起始时间
	VOID	SetTimeNow(Time_t nTime) {m_nTimeNow=nTime;};
	INT		GetTimeNow(VOID) const {return m_nTimeNow;};

};

typedef std::map< UINT, SImpactEffect*> CImpactEffectMap;




enum ENUM_LOGIC_EVENT_TYPE
{
	LOGIC_EVENT_TYPE_INVALID	= -1,	
	LOGIC_EVENT_TYPE_DAMAGE,	/// 伤害

	LOGIC_EVENT_TYPE_NUMBERS
};

struct _LOGIC_EVENT
{
	UINT					m_uBeginTime;			// 技能效果作用的开始时间
	UINT					m_uRemoveTime;			// 技能效果作用的持续时间
	ObjID_t					m_nSenderID;			// 攻击者的ID
	INT						m_nSenderLogicCount;	// 攻击者的逻辑计数

	INT						m_nEventType;			// 类型 ENUM_LOGIC_EVENT_TYPE

	_DAMAGE_INFO			m_damage;				// 机能效果所产生的伤害信息		


	_LOGIC_EVENT( VOID )
	{
		m_uBeginTime	= 0;
		m_uRemoveTime	= 1000;
	}

	VOID Init(ObjID_t nSenderID, INT nSenderLogicCount, const _DAMAGE_INFO *pDamageInfo)
	{
		m_nSenderID			= nSenderID;
		m_nSenderLogicCount	= nSenderLogicCount;

		m_nEventType		= LOGIC_EVENT_TYPE_DAMAGE;
		m_damage			= *pDamageInfo;
	}	

	VOID Reset( VOID )
	{
		m_uBeginTime	= 0;
		m_uRemoveTime	= 1000;

		m_damage.Reset();		
	}
};

typedef std::list< _LOGIC_EVENT* >		CLogicEventList;


#endif // __CHARACTERSTRUCT_H__
