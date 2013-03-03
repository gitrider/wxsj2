
/**
 *	在地表的贴花
 */

#pragma once


#include "../Obj_Static.h"



/**
 *	贴花
 */
class CObject_ProjTex : public CObject_Static
{
public:

	/// 贴花类型
	enum PROJTEX_TYPE
	{
		PROJTEX_NULL,

		PROJTEX_REACHTARGET,	// 鼠标选择的目标点
		PROJTEX_AURARUNE,		// 技能范围环
		PROJTEX_BLOODSTAIN,		// 血迹
	};
	
	static FLOAT PROJTEX_HEIGHT;

public:

	CObject_ProjTex();
	virtual ~CObject_ProjTex();

	/** 根据初始化物体，并同步到渲染层 */
	virtual	VOID Initial(VOID*);
	virtual VOID Release( VOID );


	/**
	 *	获取贴花类型
	 */
	virtual PROJTEX_TYPE GetType(VOID) const = 0;

	/**
	 *	设置高度
	 */
	virtual VOID SetHeight( FLOAT fHeight = 1.7f );


protected:

	KL_DECLARE_DYNAMIC(CObject_ProjTex);

};	// class



//--------------------------------------------------

/** 
 *	鼠标指向的目的地
 */
class CObject_ProjTex_MouseTarget : public CObject_ProjTex
{
public:

	CObject_ProjTex_MouseTarget(void);
	virtual ~CObject_ProjTex_MouseTarget(void);

	virtual PROJTEX_TYPE GetType(VOID) const	{ return PROJTEX_REACHTARGET; }

	VOID SetReachAble(BOOL bReachAble);
	VOID UpdateAsCursor(void);

	virtual VOID Tick(VOID);


protected:

	UINT		m_uEnableTime;

protected:

	KL_DECLARE_DYNAMIC(CObject_ProjTex_MouseTarget);

};	// class


//--------------------------------------------------

/**
 *	技能范围环
 */
class CObject_ProjTex_AuraDure : public CObject_ProjTex
{
public:

	CObject_ProjTex_AuraDure() : m_bShowEnable( FALSE )		{ }
//	CObject_ProjTex_AuraDure() : m_fRingRange(0.0f) {}

	virtual VOID Tick(VOID);

	virtual  PROJTEX_TYPE GetType(VOID) const	{ return PROJTEX_AURARUNE; }

	VOID SetShowEnable(BOOL bEnable);
	VOID SetShowEnable(BOOL bEnable, const CHAR* effectName) ;
	BOOL GetShowEnable(void) const				{ return m_bShowEnable; }

	VOID SetRingRange(FLOAT nRingRange);
	FLOAT GetRingRange(void) const				{ return m_fRingRange; }


protected:

	BOOL		m_bShowEnable;
	FLOAT		m_fRingRange;

protected:

	KL_DECLARE_DYNAMIC(CObject_ProjTex_AuraDure);

};	// class


//--------------------------------------------------

/**
 *	血迹
 */
class CObject_ProjTex_BloodStain : public CObject_ProjTex
{
public:

	CObject_ProjTex_BloodStain( VOID );


	virtual PROJTEX_TYPE GetType(VOID) const	{ return PROJTEX_BLOODSTAIN; }

	/**
	 *	设置参数
	 *
	 *	nRingRange		范围尺寸
	 *	vPos			位置
	 */
	VOID SetParameter( FLOAT fRingRange, const fVector2& vPos );

	virtual VOID Tick(VOID);


protected:

	UINT		m_uEnableTime;
	FLOAT		m_fRingRange;

	tEntityNode::PROJTEX_TYPE	m_enumBloodType;

protected:

	KL_DECLARE_DYNAMIC( CObject_ProjTex_BloodStain );

};	// class
