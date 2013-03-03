///////////////////////////////////////////////////////////////////////////////
// �ļ�����GameStruct_Impact.h
// ����Ա������
// ����˵����Ч���Ĵ洢�ṹ
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file GameStruct_Impact.h
*		\brief The struct of Impact datas
*/

#ifndef GAMESTRUCT_IMPACT_H
#define GAMESTRUCT_IMPACT_H
#include "Type.h"
#include "GameDefine2.h"
#include "GameUtil.h"

typedef enum _DIRECT_IMPACT_SEID
{
	SPECIAL_DROPBOX = -3,//DROPBOX
	SPECIAL_DIE = -2,	//DIE
	DISPLAY_MISS = -1,	//MISS
	DAMAGE_PHYSICS = 0, //�����˺�
	DAMAGE_MAGIC = 1, 	//ħ���˺�
	DAMAGE_COLD = 2, 	//���˺�
	DAMAGE_FIRE = 3,	//���˺�
	DAMAGE_LIGHT = 4,	//���˺�
	DAMAGE_POISON = 5,	//���˺�
	DISPEL_RAGE = 6,	//��ŭ��
	
	MAX_DIRECT_IMPACT_SEID, 
} DIRECT_IMPACT_SEID;
/*
typedef struct _OWN_IMPACT
{
	enum
	{
		MAX_IMPACT_PARAM_NUMBER = 12
	};
	_OWN_IMPACT()
	{
		CleanUp();
	};
	VOID CleanUp(VOID)
	{
		m_nSN = -1;
		m_nLogicID = 0;
		m_nSkillID = -1;
		m_nCasterID = -1;
		m_bIgnoreFliters = FALSE;
		//m_bIsGoodImpact = FALSE;
		m_nContinuance = 0;
		m_bFadeOut = FALSE;
		m_bFadeOutWhenSupportLost=FALSE;
		m_bIsCritical = FALSE;
		m_nImpactID = -1;
		m_nMutexID = -1;
		m_nPRI = 0;
		memset((VOID*)m_aParams, 0, sizeof(m_aParams));		
	};
	_OWN_IMPACT& operator=(_OWN_IMPACT const& rhs)
	{
		memcpy((VOID*)this, (VOID const*)&rhs, sizeof(*this));
		return *this;
	};
	INT m_nSN;
	INT m_nLogicID;
	INT m_nSkillID;
	INT m_nCasterID;
	//GUID m_CasterGUID;
	BOOL m_bIgnoreFliters;
	//BOOL m_bIsGoodImpact;
	Time_t m_nContinuance;
	BOOL m_bFadeOut;
	BOOL m_bFadeOutWhenSupportLost;
	BOOL m_bIsCritical;
	INT m_nImpactID;
	INT m_nMutexID;
	INT m_nPRI;
	INT m_aParams[MAX_IMPACT_PARAM_NUMBER];
	//BitFlagSet_T<NUMBER_OF_EVENTS> m_CaredEvent;
} OWN_IMPACT;
*/
// _IMPACT_DIRECT
#define DEF_IMPACT_DIRECT_PARAM_NUMBERS	(4)
struct _IMPACT_DIRECT
{
	INT				m_nID;
	INT				m_nCreaterID;
	INT				m_nLogicCountOfCreater;
	INT 			m_aParams[DEF_IMPACT_DIRECT_PARAM_NUMBERS];

	_IMPACT_DIRECT( VOID )
	{
		m_nID					= INVALID_ID;
		m_nCreaterID			= INVALID_ID;
		m_nLogicCountOfCreater	= -1;
		memset( m_aParams, 0, sizeof( m_aParams ) );
	}

	VOID Reset( VOID )
	{
		m_nID					= INVALID_ID;
		m_nCreaterID			= INVALID_ID;
		m_nLogicCountOfCreater	= -1;
		memset( m_aParams, 0, sizeof( m_aParams ) );
	}
};
typedef class _OWN_IMPACT //struct like class, can use memcpy to copy it 
{
	private:
		enum
		{
			//��ɢ
			IDX_FADE_OUT = 0,
			//����
			IDX_CRITICAL = 1,
			//����Ч���ļ���������ʹ�õ�
			IDX_CREATE_BY_PLAYER,
			IDX_MAX,
		};
	public:
		enum
		{
			MAX_FLAG_NUMBER = IDX_MAX
		};
		enum
		{
			//����impact����,ʵ���������������MAX_IMPACT_PARAM_NUMBER-1
			//���һ����ԱΪ��Ӧ���㷨����
			MAX_IMPACT_PARAM_NUMBER = 13//modify by gh for impact83 11->13
		};
		
		enum
		{
			//����buff���Ӳ���
			MAX_LAYER_NUMBER = 2
		};
	private:
		typedef BitFlagSet_T<MAX_FLAG_NUMBER> InternalFlags_T;
	public:
		_OWN_IMPACT()
		{
			CleanUp();
		};
		~_OWN_IMPACT() {};
		VOID CleanUp(VOID)
		{
			m_nSN					= INVALID_ID;
			m_nDataIndex			= INVALID_ID;
			m_nImpactID				= INVALID_ID;
			m_nSkillID				= INVALID_ID;
			m_nCasterObjID			= INVALID_ID;
			m_nCasterUniqueID		= INVALID_ID;
			m_nCasterLogicCount		= 0;
			m_nContinuance			= 0;
			m_nContinuanceElapsed	= 0;
			m_nIntervalElapsed		= 0;
			m_nDelayTime			= 0;
			//��֪��ͬ�༼�ܵ���С�����
			m_uMaxLayerCount		= 1;
			//Ч���ĵ�ǰ����
			m_nNumOfLayer			= 1;
			m_Flags.ClearAllFlags();
			//MarkFadeOutFlag();// ȱʡ����Ч��
			memset((VOID*)m_aParams, 0, sizeof(m_aParams));	
			memset((VOID*)m_aLayerContinuance, 0, sizeof(m_aLayerContinuance));	
			m_iSkillLevel = 0;
		};
		_OWN_IMPACT& operator=(_OWN_IMPACT const& rhs)
		{
			m_nSN					= rhs.GetSN();
			m_nDataIndex			= rhs.GetDataIndex();
			m_nImpactID				= rhs.GetImpactID();
			m_nSkillID				= rhs.GetSkillID();
			m_nCasterObjID			= rhs.GetCasterObjID();
			m_nCasterUniqueID		= rhs.GetCasterUniqueID();
			m_nCasterLogicCount		= rhs.GetCasterLogicCount();
			m_nContinuance			= rhs.GetContinuance();
			m_nContinuanceElapsed	= rhs.GetContinuanceElapsed();
			m_nIntervalElapsed		= rhs.GetIntervalElapsed();
			m_Flags					= rhs.m_Flags;
			m_nDelayTime			= rhs.m_nDelayTime;
			m_uMaxLayerCount		= rhs.m_uMaxLayerCount;
			m_nNumOfLayer			= rhs.m_nNumOfLayer;
			m_iSkillLevel			= rhs.m_iSkillLevel;
			memcpy( (VOID*)&rhs.m_aLayerContinuance[0], m_aLayerContinuance, sizeof(m_aLayerContinuance) );
			for(INT nIdx=0; MAX_IMPACT_PARAM_NUMBER>nIdx; ++nIdx)
			{
				m_aParams[nIdx] = rhs.GetParamByIndex(nIdx);
			}
			return *this;
		};
		INT		GetSN(VOID) const {return m_nSN;}
		VOID 	SetSN(INT nSN) {m_nSN=nSN;}
		
		ID_t	GetDataIndex(VOID) const {return m_nDataIndex;}
		VOID	SetDataIndex(ID_t nIdx) {m_nDataIndex = nIdx;}

		ID_t	GetImpactID(VOID) const {return m_nImpactID;}
		VOID	SetImpactID(ID_t nID) {m_nImpactID = nID;}

		ID_t	GetSkillID(VOID) const {return m_nSkillID;}
		VOID	SetSkillID(ID_t nID) {m_nSkillID = nID;}

		ObjID_t	GetCasterObjID(VOID) const {return m_nCasterObjID;}
		VOID	SetCasterObjID(ObjID_t nID) {m_nCasterObjID = nID;}

		ObjID_t	GetCasterUniqueID(VOID) const {return m_nCasterUniqueID;}
		VOID	SetCasterUniqueID(ObjID_t nID) {m_nCasterUniqueID = nID;}

		INT		GetCasterLogicCount(VOID) const {return m_nCasterLogicCount;}
		VOID	SetCasterLogicCount(INT nCount) {m_nCasterLogicCount= nCount;}

		Time_t	GetContinuance(VOID) const {return m_nContinuance;}
		VOID	SetContinuance(Time_t nTime) {m_nContinuance = nTime;}

		Time_t	GetContinuanceElapsed(VOID) const {return m_nContinuanceElapsed;}
		VOID	SetContinuanceElapsed(Time_t nTime) {m_nContinuanceElapsed = nTime;}

		Time_t	GetIntervalElapsed(VOID) const {return m_nIntervalElapsed;}
		VOID	SetIntervalElapsed(Time_t nTime) {m_nIntervalElapsed = nTime;}

		BOOL	IsCreateByPlayer(VOID) const {return m_Flags.GetFlagByIndex(IDX_CREATE_BY_PLAYER);}
		VOID	MarkCreateByPlayerFlag(VOID) {m_Flags.MarkFlagByIndex(IDX_CREATE_BY_PLAYER);}
		VOID	ClearCreateByPlayerFlag(VOID) {m_Flags.ClearFlagByIndex(IDX_CREATE_BY_PLAYER);}

		BOOL	IsFadeOut(VOID) const {return m_Flags.GetFlagByIndex(IDX_FADE_OUT);}
		VOID	MarkFadeOutFlag(VOID) {m_Flags.MarkFlagByIndex(IDX_FADE_OUT);}
		VOID	ClearFadeOutFlag(VOID) {m_Flags.ClearFlagByIndex(IDX_FADE_OUT);}

		BOOL	IsCriticalHit(VOID) const {return m_Flags.GetFlagByIndex(IDX_CRITICAL);}
		VOID	MarkCriticalFlag(VOID) {m_Flags.MarkFlagByIndex(IDX_CRITICAL);}
		VOID	ClearCriticalFlag(VOID) {m_Flags.ClearFlagByIndex(IDX_CRITICAL);}
		VOID	SetDelayTime( Time_t iTime ) { m_nDelayTime = iTime; }
		Time_t	GetDelayTime(VOID) const { return m_nDelayTime; }
		INT		GetSkillLevel() const { return m_iSkillLevel; }
		VOID	SetSkillLevel(INT val) { m_iSkillLevel = val; }

		//�ο����Ե�ע��
		VOID	SetMaxLayerCount( UINT uCount )
		{ 
			m_uMaxLayerCount = uCount; 
		}
		//�ο����Ե�ע��
		UINT	GetMaxLayerCount() const { return m_uMaxLayerCount; }
		//�ο����Ե�ע��
		VOID	SetLayerCount( UINT uCount ) 
		{ 
			m_nNumOfLayer = uCount; 
		}
		//�ο����Ե�ע��
		INT	GetLayerCount() const { return m_nNumOfLayer; }
		
		INT		GetParamByIndex(INT nIdx) const
		{
			if(0>nIdx || MAX_IMPACT_PARAM_NUMBER<=nIdx)
			{
				Assert(NULL=="[OWN_IMPACT::GetParamByIndex]:Illegal index found!!");
				return 0;
			}
			return m_aParams[nIdx];
		}
		VOID	SetParamByIndex(INT nIdx, INT nValue)
		{
			if(0>nIdx || MAX_IMPACT_PARAM_NUMBER<=nIdx)
			{
				Assert(NULL=="[OWN_IMPACT::SetParamByIndex]:Illegal index found!!");
				return;
			}
			m_aParams[nIdx] = nValue;
		}

		Time_t GetLayerContinuanceByIdx( INT iIdx )
		{ 
			if( 0>iIdx || MAX_LAYER_NUMBER<=iIdx)
			{
				Assert(NULL=="[OWN_IMPACT::GetLayerContinuanceByIdx]:Illegal index found!!");
				return 0;
			}
			return m_aLayerContinuance[iIdx];
		}
		VOID SetLayerContinuanceByIdx( INT iIdx, Time_t iTime )
		{ 
			if( 0>iIdx || MAX_LAYER_NUMBER-1<=iIdx)
			{
				Assert(NULL=="[OWN_IMPACT::SetLayerContinuanceByIdx]:Illegal index found!!");
				return;
			}
			m_aLayerContinuance[iIdx] = iTime;
		}
	private:
		INT 	m_nSN;
		//��impact����е�����
		ID_t 	m_nDataIndex;
		ID_t	m_nImpactID;
		//������Ч���ļ���ID
		ID_t 	m_nSkillID;
		//����ʹ���ߵ�ID
		ObjID_t m_nCasterObjID;
		INT		m_nCasterUniqueID;
		INT		m_nCasterLogicCount;
		//Ч������ʱ��
		Time_t	m_nContinuance;
		//Ч������ʱ�����ȥֵ
		Time_t 	m_nContinuanceElapsed;
		//�ڲ���ȥʱ��
		Time_t 	m_nIntervalElapsed;
		//Ч��״̬��λ���
		InternalFlags_T	m_Flags;
		//Ч������������
		INT 	m_aParams[MAX_IMPACT_PARAM_NUMBER];
		Time_t	m_nDelayTime;
		//Ч����������, ��InitFromData�г�ʼ��Ϊ����е�С��������
		//��������¼��stand_impact�����
		UINT	m_uMaxLayerCount;
		//Ч���ĵ�ǰ����
		INT		m_nNumOfLayer;
		Time_t	m_aLayerContinuance[MAX_LAYER_NUMBER];
		INT		m_iSkillLevel;
		//BitFlagSet_T<NUMBER_OF_EVENTS> m_CaredEvent;
} OWN_IMPACT;

#endif //GAMESTRUCT_IMPACT_H
