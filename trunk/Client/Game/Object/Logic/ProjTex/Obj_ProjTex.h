
/**
 *	�ڵر������
 */

#pragma once


#include "../Obj_Static.h"



/**
 *	����
 */
class CObject_ProjTex : public CObject_Static
{
public:

	/// ��������
	enum PROJTEX_TYPE
	{
		PROJTEX_NULL,

		PROJTEX_REACHTARGET,	// ���ѡ���Ŀ���
		PROJTEX_AURARUNE,		// ���ܷ�Χ��
		PROJTEX_BLOODSTAIN,		// Ѫ��
	};
	
	static FLOAT PROJTEX_HEIGHT;

public:

	CObject_ProjTex();
	virtual ~CObject_ProjTex();

	/** ���ݳ�ʼ�����壬��ͬ������Ⱦ�� */
	virtual	VOID Initial(VOID*);
	virtual VOID Release( VOID );


	/**
	 *	��ȡ��������
	 */
	virtual PROJTEX_TYPE GetType(VOID) const = 0;

	/**
	 *	���ø߶�
	 */
	virtual VOID SetHeight( FLOAT fHeight = 1.7f );


protected:

	KL_DECLARE_DYNAMIC(CObject_ProjTex);

};	// class



//--------------------------------------------------

/** 
 *	���ָ���Ŀ�ĵ�
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
 *	���ܷ�Χ��
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
 *	Ѫ��
 */
class CObject_ProjTex_BloodStain : public CObject_ProjTex
{
public:

	CObject_ProjTex_BloodStain( VOID );


	virtual PROJTEX_TYPE GetType(VOID) const	{ return PROJTEX_BLOODSTAIN; }

	/**
	 *	���ò���
	 *
	 *	nRingRange		��Χ�ߴ�
	 *	vPos			λ��
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
