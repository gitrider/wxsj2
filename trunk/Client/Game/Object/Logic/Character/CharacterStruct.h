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
struct  NPC_AUTOPATH	// NPC�Զ�Ѱ·����ʹ�ýṹ
{
	BOOL   m_isAutoPath;		// �Ƿ����Զ�Ѱ·״̬�����ڽ�ɫ�ƶ�ֹͣ���ж�ʹ��
	STRING m_strAutoNPCName;    // �Զ�Ѱ·��NPC����
	INT	   m_nSceneID;			//�Ǹ�������NPC

	NPC_AUTOPATH()
	{
		m_strAutoNPCName = "";
		m_isAutoPath = FALSE;
		m_nSceneID = -1;
	};
	
};
// 20100415 AddCodeEnd

// ��Ч��
struct SEffectSet
{
protected:
	UINT	m_uEffect;	// ��Ч���

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

	INT			m_nNumOfLayer;				// �ۻ��Ĳ�����buff�Ĳ����ۻ���	
	INT			m_nImpactID;				// ��Ӧeffect���еı�ʾid
	BOOL		m_bLineEffect;				// ���ߵ���Ч
	BOOL		m_bRotateEffect;			// ��ת��Ч
	ObjID_t		m_CreatorID;				// ������ID
	LPCSTR		m_lpszCreatorLocator;		// �����ߵİ󶨵�

	std::vector<INT> m_vBulletIndexList;	// �ӵ��б�,��ת��Ч��

	BOOL		m_bShowEffect;				// 
	UINT		m_nBeginTime;				// ��ʼ�����ӳ�ʱ��
	UINT		m_nDelayTime;				// �ӳ�
	INT			m_nArrayIndex;				// �ڱ��������е�����

	time_t		m_nTimeNow;		// ����buff��ǰ����ʱ��������ʼʱ�� 20100531 BLL
	INT			m_nTimeElapse;	// ����buff��ʣ������

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

	// ����buff��ʣ������   20100531 BLL
	VOID	SetContinuance(Time_t nContinuance) {m_nTimeElapse=nContinuance;};
	INT		GetContinuance(VOID) const {return m_nTimeElapse;};

	// ����buff��ǰ����ʱ��������ʼʱ��
	VOID	SetTimeNow(Time_t nTime) {m_nTimeNow=nTime;};
	INT		GetTimeNow(VOID) const {return m_nTimeNow;};

};

typedef std::map< UINT, SImpactEffect*> CImpactEffectMap;




enum ENUM_LOGIC_EVENT_TYPE
{
	LOGIC_EVENT_TYPE_INVALID	= -1,	
	LOGIC_EVENT_TYPE_DAMAGE,	/// �˺�

	LOGIC_EVENT_TYPE_NUMBERS
};

struct _LOGIC_EVENT
{
	UINT					m_uBeginTime;			// ����Ч�����õĿ�ʼʱ��
	UINT					m_uRemoveTime;			// ����Ч�����õĳ���ʱ��
	ObjID_t					m_nSenderID;			// �����ߵ�ID
	INT						m_nSenderLogicCount;	// �����ߵ��߼�����

	INT						m_nEventType;			// ���� ENUM_LOGIC_EVENT_TYPE

	_DAMAGE_INFO			m_damage;				// ����Ч�����������˺���Ϣ		


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
