///////////////////////////////////////////////////////////////////////////////
// �ļ�����DataRecords.h
// ����Ա������
// ����˵�������ܺ�Ч���ı��¼�ṹ
//
// �޸ļ�¼��
// ף����  ����ע�� Ŀ�����ڽ����ĵ���ͬʱ����
//		   ��׼ȷ�������๦�ܣ��˴��޸�ֻ��������ע��
//		   �����κδ����޸ġ�date: 10/30/2007
//			
//			������������߼���������ԣ�����صļ��ط���
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DATARECORDS_H
#define DATARECORDS_H
#include "Type.h"

class SplitStringLevelOne;

namespace DBC
{
	class DBCFile;
}

const INT MAX_SKIL_LEVEL_LIMIT = 12;

//FILE_SKILL_DATA "../Public/Config/SkillData_V1.txt"
//��SkillInstanceData_T���Ӷ���
//���������ĵ�����
class Descriptor_T
{
	public:
		Descriptor_T():m_szDesc(NULL) {memset(m_nValue, 0, sizeof(m_nValue));}
		Descriptor_T(Descriptor_T const& rhs): m_szDesc(rhs.m_szDesc)
		{
			memcpy( m_nValue, rhs.m_nValue, sizeof(m_nValue) );
		};
		~Descriptor_T() {};
		Descriptor_T& operator=(Descriptor_T const& rhs) 
		{
			m_szDesc = rhs.m_szDesc;
			memcpy( m_nValue, rhs.m_nValue, sizeof(m_nValue) );
			return *this;
		};
		BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow, INT& nColumn, BOOL bNeedSplit=0 );
		CHAR const* GetDescription(VOID) const {return m_szDesc;};
		INT GetValueBySkillLevel( INT iSkillLevel ) const {return m_nValue[iSkillLevel];};
		VOID SetValueBySkillLevel(INT const nValue, INT iSkillLevel ) {m_nValue[iSkillLevel]=nValue;};
	protected:
	private:
		//���ĵ�����
		CHAR const* m_szDesc;
		//���ĵľ���ֵ
		INT	m_nValue[MAX_SKIL_LEVEL_LIMIT];
};

//FILE_SKILL_DATA "../Public/Config/SkillData_V1.txt"
//��SkillInstanceData_T���Ӷ���
//�������������ĵĲ���
class ConDepTerm_T
{
	//����������
	public:
		ConDepTerm_T(): m_szDesc(NULL),m_nType(INVALID_ID),m_nParam0(0),m_nParam1(0){};
		ConDepTerm_T(ConDepTerm_T const& rhs) : m_szDesc(rhs.m_szDesc), m_nType(rhs.m_nType),m_nParam0(rhs.m_nParam0), m_nParam1(rhs.m_nParam1){};
		~ConDepTerm_T(){};
		ConDepTerm_T& operator=(ConDepTerm_T const& rhs) 
		{
			m_szDesc = rhs.m_szDesc;
			m_nType = rhs.m_nType;
			m_nParam0 = rhs.m_nParam0;
			m_nParam1 = rhs.m_nParam1; 
			return *this;
		};
		BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow, INT& nColumn);
		CHAR const* GetDescription(VOID) const {return m_szDesc;};
		INT GetType(VOID) const {return m_nType;};
		//VOID SetType(INT nValue) {m_nType=nValue;};
		INT GetParam0(VOID) const {return m_nParam0;};
		//VOID SetParam0(INT nValue) {m_nParam0=nValue;};
		INT GetParam1(VOID) const {return m_nParam1;};
		//VOID SetParam1(INT nValue) {m_nParam1=nValue;};
	protected:
	private:
		CHAR const* m_szDesc;
		INT m_nType;
		INT m_nParam0;
		INT m_nParam1;
};

//ս��ģ��
namespace Combat_Module
{
	//IDCollection_T
	//�������ܻ����ϵ����Ӧ�ı��ΪFILE_ID_COLLECTION
	//"../Public/Config/IDCollections.txt"
	//��g_IDCollectionMgr����
	class IDCollection_T
	{
		public:
			enum
			{
				MAX_ID = 1024,
			};
			enum
			{
				COLLECTION_SIZE = 30,
			};
			typedef enum
			{
				TYPE_BUFF_ID = 0,
				TYPE_IMPACT_MUTEX_ID,
				TYPE_IMPACT_LOGIC_ID,
				TYPE_SKILL_ID,
				TYPE_SKILL_LOGIC_ID,
				TYPE_DIRECT_IMPACT_ID,
			} CollectionIdType_T;
			IDCollection_T() :m_bInited(FALSE), m_nID(INVALID_ID),m_szDescription(NULL), m_nSize(0)
			{
				memset((VOID*)m_aID, INVALID_ID, sizeof(m_aID));
			};
			~IDCollection_T() {};
			BOOL IsInited(VOID) const {return m_bInited;};
			BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
			ID_t GetID(VOID) const{return m_nID;};
			CHAR const* GetDescription(VOID) const {return m_szDescription;};
			INT GetType(VOID) const {return m_nType;}
			INT GetCollectionSize(VOID) const 
			{
				INT& rSize = (INT&)m_nSize;
				COLLECTION_SIZE<rSize?rSize=COLLECTION_SIZE:NULL;
				return m_nSize;
			};
			INT GetIDByIndex(INT nIndex) const;
			BOOL IsThisIDInCollection(ID_t nID) const;
			CHAR const* GetEndDesc(VOID) const {return m_szEndDesc;};
		protected:
		private:
			BOOL m_bInited;
			ID_t m_nID;
			CHAR const* m_szDescription;
			INT m_nType;
			INT m_nSize;
			INT m_aID[COLLECTION_SIZE];
			CHAR const* m_szEndDesc;
	};
	//����ģ��
	namespace Skill_Module
	{
		//�������
		typedef enum
		{
			//�Ƿ�
			INVALID_TYPE = SKILL_TYPE_INVALID,
			//��������
			SKILL_NEED_CHARGING = SKILL_TYPE_GATHER,
			//��������
			SKILL_NEED_CHANNELING = SKILL_TYPE_LEAD,
			//ֱ��ʹ�ü���
			SKILL_INSTANT_LAUNCHING = SKILL_TYPE_LAUNCH,
			//��������
			SKILL_PASSIVE = SKILL_TYPE_PASSIVE,
			NUMBER_OF_SKILL_TYPE
		} SkillType_T;

		//XinFaData
		//FILE_XINFA	"../Public/Config/XinFa_V1.txt"
		//��Ӧ�ķ�ID������ID
		//��g_XinFaDataMgr����
		class XinFaData_T
		{
			public:
				enum
				{
					MAX_ID = 255,
				};
				XinFaData_T(): m_nMenPai(-1),m_bInited(FALSE),m_szDescription(NULL),m_szName(NULL),m_szIcon(NULL){};
				~XinFaData_T(){};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				inline MenPaiID_t GetMenPai() const {return m_nMenPai;}
				inline CHAR const* GetName(VOID) {return m_szName;}
				inline CHAR const* GetDescription(VOID) {return m_szDescription;}
				inline CHAR const* GetIcon(VOID){return m_szIcon;}
				inline BOOL IsInited(VOID) const {return m_bInited;}
			protected:
			private:
				MenPaiID_t m_nMenPai;
				CHAR const* m_szName;
				CHAR const* m_szDescription;
				CHAR const* m_szIcon;
				BOOL m_bInited;
		};
		//SkillTemplate
		//���ܵ�ȫ�����ԣ���ϵͳ����ʱ��ʼ��
		//FILE_SKILL_TEMPLATE					"../Public/Config/SkillTemplate_V1.txt"
		//�����ͷ�ʱ�������ж�
		//��g_SkillTemplateDataMgr����
		class SkillTemplateData_T
		{
			public:
				enum
				{
					MAX_ID			= 36384,
					MAX_XINFA_LEVEL = 12,
					MAX_SUB_SKILL	= 16
				};
				SkillTemplateData_T();
				~SkillTemplateData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				SkillID_t GetSkillID(VOID) const {return m_nSkillID;};
				MenPaiID_t GetMenPai(VOID) const {return m_nMenPai;};
				CHAR const* GetName(VOID) const {return m_szName;};
				INT GetXinFaLevelRequirement(VOID) const {return m_nXinFaLevelRequirement;};
				INT GetMustUseWeaponFlag(VOID) const {return m_bMustUseWeapon;};
				INT GetDisableByFlag1(VOID) const {return m_nDisableByFlag1;};
				INT GetDisableByFlag2(VOID) const {return m_nDisableByFlag2;};
				INT GetDisableByFlag3(VOID) const {return m_nDisableByFlag3;};
				XinFaID_t GetXinFa(VOID) const {return m_nXinFa;};
				INT GetXinFaParam(VOID) const {return m_nXinFaParam;};
				INT GetRangedSkillFlag(VOID) const {return m_nRangedSkillFlag;};
				BOOL ForceBreakPreSkill(VOID) const {return m_bForceBreakPreSkill;};
				INT GetSkillType(VOID) const {return m_nSkillType;};
				CooldownID_t GetCooldownID(VOID) const {return m_nCooldownID;};
				BOOL GetTargetMustInSpecialState(VOID) const {return m_nTargetMustInSpecialState;};
				ID_t GetClassByUser(VOID) const {return m_nClassByUser;};
				ID_t GetPassiveFlag(VOID) const {return m_nPassiveFlag;};
				ID_t GetSelectType(VOID) const {return m_nSelectType;};
				ID_t GetOperateModeForPetSkill(VOID) const {return m_nOperateModeForPetSkill;};
				ID_t GetPetRateOfSkill(VOID) const {return m_nPetRateOfSkill;};
				ID_t GetTypeOfPetSkill(VOID) const {return m_nTypeOfPetSkill; };
				ID_t GetImpactIDOfSkill(VOID) const {return m_nImpactIDOfSkill; };
				ID_t GetTargetingLogic(VOID) const {return m_nTargetingLogic;};
				Time_t GetPlayActionTime(VOID) const {return m_nPlayActionTime;};
				FLOAT GetOptimalRangeMin(VOID) const {return m_fOptimalRangeMin;};
				FLOAT GetOptimalRangeMax(VOID) const {return m_fOptimalRangeMax;};
				INT GetStandFlag(VOID) const {return m_nStandFlag;};
				ID_t GetTargetLogicByStand(VOID) const {return m_nTargetLogicByStand;};
				ID_t GetTargetCheckByObjType(VOID) const {return m_nTargetCheckByObjType;};
				BOOL IsPartyOnly(VOID) const {return m_bPartyOnly;};
				INT GetChargesOrInterval(VOID) const {return m_nChargesOrInterval;};
				BOOL IsAutoShotSkill(VOID) const {return m_bAutoShot;};
				BOOL UseNormalAttackRate(VOID) const {return m_bUseNormalAttackRate;};
				Time_t GetActiveTime(VOID) const {return m_nActiveTime;};
				FLOAT GetAngle(VOID) const {return m_fAngle;};
				BOOL CanInterruptAutoShot(VOID) const {return m_bCanInterruptAutoShot;};
				Time_t GetDelayTime(VOID) const {return m_nDelayTime;};
				ID_t GetSkillInstance(INT const nIdx) const 
				{
					if(0>nIdx||MAX_XINFA_LEVEL<=nIdx)
					{
						AssertEx(FALSE,"[SkillTemplateData_T::GetSkillInstance]:nIdx Out of range!!");
						return INVALID_ID;
					}
					return m_aSkillInstance[nIdx];
				};
				CHAR const* Description(VOID) const { return m_szDescription; };
				Time_t	GetHurtDelayTime() const { return m_nHurtDelayTime; }
				BOOL	IsInterruptMove() const { return m_bInterruptMove; }
				FLOAT	GetBeatBackDistance() const { return m_fBeatBackDistance; }
				FLOAT	GetBeatBackRate() const { return m_fBeatBackRate; }
				FLOAT	GetBeatFlyDistance() const { return m_fBeatFlyDistance; }
				FLOAT	GetBeatFlyRate() const { return m_fBeatFlyRate; }
				BOOL	IsUseBallSkill() const { return m_bUseBallSkill; }
				INT		GetSkillMaxLevex() const { return m_iSkillMaxLevel; }
				INT		GetSkillClass( VOID ) const	{ return m_iSkillClass; }
				INT		GetSubSkill( INT iIndex ) const { return m_aSubSkill[iIndex]; }
				INT		GetSubSkillSuccessRate( INT iIndex ) const { return m_aSubSkillSuccessRate[iIndex]; }
				BOOL	IsRunSkill() const 
				{
					return m_iIsRunSkill != INVALID_ID;
				}
				INT GetChannelBackRate(){ return m_iChannelBackRate; }
				INT GetChannelBackHappanRate(){ return m_iChannelBackHappanRate; }
				BOOL IsCanInteruptChannel(){ return m_bCanInterruptChannel; }
				BOOL GetSpeedOfBullet() const { return m_nSpeedOfBullet; }
			protected:
				VOID ReadSplitStringToTable( SplitStringLevelOne *pSplitL1, const CHAR* pSplitString, INT* pArry );

			private:
				BOOL		m_bInited;					//�ü�¼�Ƿ��Ѿ���ʼ��
				SkillID_t	m_nSkillID;					//���ܱ��
				MenPaiID_t	m_nMenPai;					//���ɱ��
				CHAR const* m_szName;					//���ܵ�����
				INT			m_nXinFaLevelRequirement;	//���ܵ��ķ��ȼ�Ҫ��
				INT			m_nClientOnly1;				//�ͻ���ר������
				BOOL		m_bMustUseWeapon;			//�˼��ܱ���ʹ������
				INT			m_nDisableByFlag1;			//�����ڱ��1
				INT			m_nDisableByFlag2;			//�����ڱ��2
				INT			m_nDisableByFlag3;			//�����ڱ��3
				XinFaID_t	m_nXinFa;					//�ķ�ID
				INT			m_nXinFaParam;				//�ķ���������
				INT			m_nRangedSkillFlag;			//�Ƿ���Զ�̼���
				BOOL		m_bForceBreakPreSkill;		//�Ƿ�ǿ���ж���һ������ִ�еļ���
				ID_t		m_nSkillType;				// Charge, channel or instant shot skill
				CooldownID_t m_nCooldownID;				//��ȴʱ���ID
				BOOL		m_nTargetMustInSpecialState;// Ŀ�������: 0:��ģ�1:����; -1: ��Ч
				ID_t		m_nClassByUser;				// ��ʹ�������ͷ��࣬0:���, 1:����, 2:����, 3:��Ʒ,
				ID_t		m_nPassiveFlag;				// �������Ǳ������ܣ�0:��������,1:��������;
				ID_t		m_nSelectType;				//����ѡ����
				ID_t		m_nOperateModeForPetSkill;	//���＼�ܷ������� 0:�����ֶ���ѡ,1:AI�Զ�ִ��,2:��ǿ�������Եı�������
				ID_t		m_nPetRateOfSkill;			//���ܷ�������,ֻ�Գ��＼����Ч
				ID_t		m_nTypeOfPetSkill;			//���＼������,0:�﹦,1:����,2:����,3:����,4:����;
				ID_t		m_nImpactIDOfSkill;			//���＼�ܲ�����Ч��ID
				INT			m_nSpeedOfBullet;			//�ӵ������ٶ�
				ID_t		m_nTargetingLogic;			//Ŀ��ѡȡ�߼�
				Time_t		m_nPlayActionTime;			//���ܶ������ŵ�ʱ��
				FLOAT		m_fOptimalRangeMin;			//����ʹ�÷�Χ�½�
				FLOAT		m_fOptimalRangeMax;			//����ʹ�÷�Χ�Ͻ�
				INT			m_nStandFlag;				//����Ǽ��ܵ�������ǣ�-1����������Ŀ�꣬0�����ԣ�1�Ǽ�����ǿĿ��
				//0: Ŀ���ʹ����Ӧ�����Ѻù�ϵ
				//1:Ŀ���ʹ����Ӧ���ǵжԹ�ϵ
				ID_t		m_nTargetLogicByStand;		//����������ʲô��Ӫ��Ŀ��
				//0: Ŀ����������	//1:Ŀ������ǳ���	//2:Ŀ������ǹ���
				ID_t		m_nTargetCheckByObjType;	//���ܵ����ļ���ؼ��
				BOOL		m_bPartyOnly;				//����ֻ�������ڶ���
				INT			m_nChargesOrInterval;		//������Ч�����������е���Ч����
				BOOL		m_bAutoShot;				//�Զ������ͷż���
				BOOL		m_bUseNormalAttackRate;		//���ܵ���ȴ�Ƿ������﹥���ٶ�Ӱ��
				Time_t		m_nActiveTime;				//����ʱ��
				FLOAT		m_fAngle;					//���ýǶ�
				BOOL		m_bCanInterruptAutoShot;	//�����ܻ��ж��Զ�������ܵ������ͷ�
				Time_t		m_nDelayTime;				//�ӳ�ʱ��
				ID_t		m_aSkillInstance[MAX_XINFA_LEVEL];//�������ķ�Ӱ�����������
				CHAR const* m_szDescription;			//���ܵ�����
				//�˺�ͨ���ӳ�ʱ��,�����ʹ�ü�����Чʱ,����client�㲥����ʹ�óɹ���Ϣ,Ȼ�󾭹����ӳ�
				//ʱ�������client�㲥�˺��б�,�Լ���server�˲����˺�Ч��
				Time_t		m_nHurtDelayTime;
				//�Ƿ�˲������,�����ʹ�ü���ʱ,�Ƿ��ж���ҵ��ߡ��ܡ������ƶ�״̬
				BOOL		m_bInterruptMove;
				//���˾��룬ÿ�����ܻ����Լ��Ļ��˾���
				FLOAT		m_fBeatBackDistance;
				//���˸��ʣ�ÿ���˶����ո��ʼ���
				FLOAT		m_fBeatBackRate;
				//���ɾ���
				FLOAT		m_fBeatFlyDistance;
				//���ɸ��ʣ�ÿ���˶����ո��ʼ���
				FLOAT		m_fBeatFlyRate;
				//�Ƿ�Ϊ��֪�༼��
				BOOL		m_bUseBallSkill;
				//����������ϵ��
				INT			m_iSkillClass;
				//���ܵĵȼ�����
				INT			m_iSkillMaxLevel;
				//�Ӽ����б�
				INT			m_aSubSkill[MAX_SUB_SKILL];
				INT			m_aSubSkillSuccessRate[MAX_SUB_SKILL];
				//�Ƿ��ǳ���˲��
				INT			m_iIsRunSkill;
				//�Ƿ����ʹ��������������
				BOOL		m_bCanInterruptChannel;
				//���˵İٷֱ�
				INT			m_iChannelBackRate;
				//�������˵ĸ���
				INT			m_iChannelBackHappanRate;

		};
		//SkillInstance
		//FILE_SKILL_DATA "../Public/Config/SkillData_V1.txt"
		//�����ķ��仯��ļ��ܲ����仯
		//��g_SkillDataMgr����
		//����m_nLogicID��Ӧ SkillLogic_T ��
		class SkillInstanceData_T
		{
			public:
				enum
				{
					MAX_ID = SkillTemplateData_T::MAX_ID,
				};
				enum
				{
					DESCRIPTOR_NUMBER = 12,
					CONDITION_AND_DEPLETE_TERM_NUMBER = 3,
				};
				SkillInstanceData_T();
				~SkillInstanceData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				INT GetInstanceID(VOID) const {return m_nInstanceID;};
				//INT GetIDForManagment(VOID) const {return m_nGUID;};
				CHAR const* GetDescription(VOID) const {return m_szDesc;};
				ID_t	GetLogicID(VOID) const {return m_nLogicID;};
				Time_t GetCooldownTimeBySkillLevel( INT iSkillLevel ) const {return m_nCooldownTime[iSkillLevel];}
				Time_t GetChargeTimeBySkillLevel( INT iSkillLevel ) const {return m_nChargeTime[iSkillLevel];};
				Time_t GetChannelTimeBySkillLevel( INT iSkillLevel ) const {return m_nChannelTime[iSkillLevel];};
				INT GetAccuracy(INT iSkillLevel) const {return m_nAccuracy[iSkillLevel];};
				INT GetMaxTargetNumber(INT iSkillLevel) const {return m_nMaxTargetNumber[iSkillLevel];};
				
				FLOAT GetRadius(INT iSkillLevel) const {return m_fRadius[iSkillLevel];};
				INT GetCriticalRate(INT iSkillLevel) const {return m_nCriticalRate[iSkillLevel];};
				
				ConDepTerm_T const* GetConDepTermByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&CONDITION_AND_DEPLETE_TERM_NUMBER>nIdx)
						return &m_aConDepTerms[nIdx];
					return NULL;
				}
				INT GetSkillLevel(VOID) const {return m_nSkillLevel;};
				Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&DESCRIPTOR_NUMBER>nIdx)
						return &m_aDescriptors[nIdx];
					return NULL;
				}
			protected:
				VOID ReadSplitStringToTable( SplitStringLevelOne* pSplitL1, const CHAR* pSplitString, INT iMaxSkillLevel, INT* pArry );
				VOID ReadSplitStringToTable( SplitStringLevelOne* pSplitL1, const CHAR* pSplitString, INT iMaxSkillLevel, FLOAT* pArry );				
			private:
				BOOL m_bInited;
				INT m_nInstanceID;
				//INT m_nGUID;
				CHAR const* m_szDesc;
				ID_t m_nLogicID;
				Time_t m_nCooldownTime[MAX_SKIL_LEVEL_LIMIT];
				Time_t m_nChargeTime[MAX_SKIL_LEVEL_LIMIT];
				Time_t m_nChannelTime[MAX_SKIL_LEVEL_LIMIT];
				ConDepTerm_T m_aConDepTerms[CONDITION_AND_DEPLETE_TERM_NUMBER];
				INT			m_nSkillLevel;
				INT			m_nAccuracy[MAX_SKIL_LEVEL_LIMIT];				//������
				INT			m_nCriticalRate[MAX_SKIL_LEVEL_LIMIT];			//������,���߽б�����
				FLOAT		m_fRadius[MAX_SKIL_LEVEL_LIMIT];				//���ð뾶
				INT			m_nMaxTargetNumber[MAX_SKIL_LEVEL_LIMIT];		//�������Ŀ����				
				Descriptor_T m_aDescriptors[DESCRIPTOR_NUMBER];
				CHAR const* m_szDescDetail;
		};
		//ImpactSEData_T
		//FILE_IMPACT_SEDATA					"../Public/Config/ImpactSEData_V1.txt"
		//��������״̬
		//���粻ͬID��buff����ID��ͬ,˵����Щbuff����ͬһ�������飬���ܲ���
		//�����Ƿ���
		//�Ƿ���Ա�������ɢ
		//�Ƿ��Ǹ���Ч��
		//�Ƿ���Ա��ֶ�ȡ��
		//��g_ImpactSEDataMgr����
		class ImpactSEData_T
		{
			public:
				enum
				{
					MAX_ID = SkillTemplateData_T::MAX_ID,
				};
				ImpactSEData_T()
					:m_bInited(FALSE),m_nID(-1),m_nMutexID(-1)
					,m_nPriParam(0), m_bCanBeDispeled(FALSE)
					,m_bHostileFlag(FALSE), m_bCanBeManualCancel(FALSE)
					,m_szDescription(NULL)
				{};
				~ImpactSEData_T() {};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				ID_t GetID(VOID) const{return m_nID;};
				ID_t GetMutexID(VOID) const {return m_nMutexID;};
				INT GetPriParam(VOID) const {return m_nPriParam;};
				BOOL IsStillOnWhenOwnerDead(VOID) const {return m_bStillOnWhenOwnerDead;};
				BOOL CanBeDispeled(VOID) const {return m_bCanBeDispeled;};
				BOOL IsHosttileImpact(VOID) const {return m_bHostileFlag;};
				BOOL CanBeManualCancel(VOID) const {return m_bCanBeManualCancel;};
				CHAR const* GetDescription(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL m_bInited;
				ID_t m_nID;
				ID_t m_nMutexID;
				INT m_nPriParam;
				BOOL m_bStillOnWhenOwnerDead;
				BOOL m_bCanBeDispeled;
				BOOL m_bHostileFlag;
				BOOL m_bCanBeManualCancel;
				CHAR const* m_szDescription;
		};		
		//SkillObjData //��ʱ����
		class SkillObjData_T
		{
			public:
				enum
				{
					MAX_ID = 255,
				};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				INT DoLogic(VOID) const {return m_nDoLogic;};
				INT Action(VOID) const {return m_nAction;};
				INT SpecialEffect(VOID) const {return m_nSpecialEffect;};
				INT BindPosition(VOID) const{return m_nSEBind;};
				INT Model(VOID) const {return m_nModel;};
				INT Icon(VOID) const {return m_nIcon;};
				INT Friendliness(VOID) const {return m_nFriendliness;};
				BOOL IsPartyOnly(VOID) const {return m_bPartyOnly;};
				CHAR const* Description(VOID) const {return m_szDescription;};
				SkillObjData_T(VOID) {};
				~SkillObjData_T(VOID) {};
			protected:
			private:
				BOOL m_bInited;
				INT m_nDoLogic;
				INT m_nAction;
				INT m_nSpecialEffect;
				INT m_nSEBind;
				INT m_nModel;
				INT m_nIcon;
				INT m_nFriendliness;
				INT m_nTargetTypeRequirement;
				BOOL m_bPartyOnly;
				CHAR const* m_szDescription;
		};		
	};
	namespace Impact_Module
	{
		//ImpactData_T
		//FILE_STANDARD_IMPACT_DATA			"./Config/StandardImpact.txt"
		//�������ܵ�ԭ��Ч����
		//�ɱ�ĳ�������ʹ�ã�����һ�����ܿ��ܰ�������Ч��ԭ��
		//�ж�Ӧ���߼�ID,��ӦImpactLogic_T
		//֮��Ĳ�����������Ӧ��ImpactLogic_T����Ĳ���
		//��g_StandardImpactMgr����
		class ImpactData_T  //��׼����Ч������
		{
			public:
				enum
				{
					//10/03/16 �¾� �ߴ��16���ӵ�18�����磺���Ĺ�+�����ԣ�*����+������Ҫ18��������
					NUMBER_OF_PARAMS = 18,
					MAX_ID = Skill_Module::SkillTemplateData_T::MAX_ID,
				};
				ImpactData_T() :
					m_bInited(FALSE),
					m_nDataIndex(INVALID_ID),
					m_szName(NULL),
					m_nLogicID(INVALID_ID),
					m_szLogicDesc(NULL),
					m_bIsOverTimed(FALSE),
					m_nImpactID(INVALID_ID),
					m_nMutexID(INVALID_ID),
					m_nLevel(0),
					m_nStandFlag(INVALID_ID),
					m_bRemainOnCorpse(FALSE),
					m_bCanBeDispeled(FALSE),
					m_bCanBeCanceled(FALSE),
					m_bNeedChannelSupport(FALSE),
					m_bIgnoreFliter(FALSE),
					m_bFadeOutWhenUnitOnDamage(FALSE),
					m_bFadeOutWhenUnitStartActions(FALSE),
					m_bFadeOutWhenUnitOffline(FALSE),
					m_bStillTimingWhenUnitOffline(FALSE),
					m_nContinuance(0),
					m_nInterval(0),
					m_bMutexByCaster(FALSE),
					m_szDescription(NULL),
					m_uBallCount(0),
					m_uBallLife(0),
					m_nReserveParam1(INVALID_ID),
					m_nReserveParam2(INVALID_ID)
				{};
				~ImpactData_T() {};
				UINT GetBallCount() const { return m_uBallCount; }
				UINT GetBallLife() const { return m_uBallLife; }

				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBC::DBCFile* pDB, INT const nRow);
				ID_t GetDataIndex(VOID) const {return m_nDataIndex;};
				CHAR const* GetName(VOID) const {return m_szName;};
				ID_t GetLogicID(VOID) const {return m_nLogicID;};
				CHAR const* GetLogicDescription(VOID) const {return m_szLogicDesc;};
				BOOL IsOverTimed(VOID) const {return m_bIsOverTimed;};
				ID_t GetImpactID(VOID) const {return m_nImpactID;};
				ID_t GetMutexID(VOID) const {return m_nMutexID;};
				ID_t GetLevel(VOID) const {return m_nLevel;};
				ID_t GetStandFlag(VOID) const {return m_nStandFlag;};
				BOOL IsRemainOnCorpse(VOID) const {return m_bRemainOnCorpse;};
				BOOL CanBeDispeled(VOID) const {return m_bCanBeDispeled;};
				BOOL CanBeCanceled(VOID) const {return m_bCanBeCanceled;};
				BOOL NeedChannelSupport(VOID) const {return m_bNeedChannelSupport;};
				BOOL IsFadeOutWhenUnitOnDamage(VOID) const {return m_bFadeOutWhenUnitOnDamage;};
				BOOL IsFadeOutWhenUnitStartActions(VOID) const {return m_bFadeOutWhenUnitStartActions;};
				BOOL IsFadeOutWhenUnitOffline(VOID) const {return m_bFadeOutWhenUnitOffline;};
				BOOL IsStillTimingWhenUnitOffline(VOID) const {return m_bStillTimingWhenUnitOffline;};
				BOOL IgnoreFliter(VOID) const {return m_bIgnoreFliter;};
				Time_t GetContinuance(VOID) const {return m_nContinuance;};
				Time_t GetInterval(VOID) const {return m_nInterval;};
				BOOL GetMutexByCasterFlag(VOID) const {return m_bMutexByCaster;};
				Descriptor_T const* GetDescriptorByIndex(INT nIdx) const
				{
					if(0<=nIdx&&NUMBER_OF_PARAMS>nIdx)
						return &m_aParams[nIdx];
					return NULL;
				};
				CHAR const* GetDescription(VOID) const {return m_szDescription;};
				
				VOID SetReserveParam1( INT value1)  { m_nReserveParam1 = value1; }
				VOID SetReserveParam2( INT value2)  { m_nReserveParam2 = value2; }

				INT GetReserveParam1() const { return m_nReserveParam1; }
				INT GetReserveParam2() const { return m_nReserveParam2; }
			protected:
			private:
				//�Ƿ��ʼ��
				BOOL m_bInited;
				//�ڱ���ж�Ӧ�ļ�¼������
				ID_t m_nDataIndex;
				//����
				CHAR const* m_szName;
				//��Ӧ�ļ���ID
				ID_t m_nLogicID;
				//����
				CHAR const* m_szLogicDesc;
				//�Ƿ�Ϊ������Ч��
				BOOL m_bIsOverTimed;
				//˲����פ��Ч��ID
				ID_t m_nImpactID;
				//������ID
				ID_t m_nMutexID;
				//�������ȼ�
				ID_t m_nLevel;
				//Ӱ������
				ID_t m_nStandFlag;
				//�����Ƿ���
				BOOL m_bRemainOnCorpse;
				//�Ƿ���Ա�������ɢ
				BOOL m_bCanBeDispeled;
				//�Ƿ���Ա��ֶ�ȡ��
				BOOL m_bCanBeCanceled;
				//�Ƿ���Ҫ����֧��
				BOOL m_bNeedChannelSupport;
				//�Ƿ���ֱͨЧ��,����������Ӱ��
				BOOL m_bIgnoreFliter;
				//�ܵ��˺�ʱ�Ƿ���ʧ
				BOOL m_bFadeOutWhenUnitOnDamage;
				//��ʼ����ʱ�Ƿ���ʧ
				BOOL m_bFadeOutWhenUnitStartActions;
				//���߾���ʧ��
				BOOL m_bFadeOutWhenUnitOffline;
				//�����Ƿ���Ȼ��ʱ
				BOOL m_bStillTimingWhenUnitOffline;
				//�ܳ���ʱ��(����)
				Time_t m_nContinuance;
				//����ʱ����(����)
				Time_t m_nInterval;
				//��������Ƿ�ο�������ID
				BOOL m_bMutexByCaster;
				//��Ч����������
				Descriptor_T m_aParams[NUMBER_OF_PARAMS];
				CHAR const* m_szDescription;
				//��֪��ͬ�༼����Ҫ���ӵ�����
				UINT m_uBallCount;
				//�ӵ�����������
				UINT m_uBallLife;

				INT  m_nReserveParam1;//add by gh for SoulBead 2010/07/01
				INT  m_nReserveParam2;
		};
	};
	namespace Special_Obj_Module
	{
		using namespace DBC;
		
		//SpecialObjData_T
		//FILE_SPECIAL_OBJ_DATA				"../Public/Config/SpecialObjData.txt"
		//Ϊ���������ɣ���ң�ɣ����ļ����������ڵ��Ϸ����屻��ɫ���������Լ������
		//�ڱ���Ŀ�п��ܲ���
		//��Ӧ�߼�ΪSpecialObjLogic_T
		//��g_SpecialObjDataMgr����
		class SpecialObjData_T
		{
			public:
				enum
				{
					MAX_ID = 1023,
					DESCRIPTOR_NUMBER = 16,
				};
				SpecialObjData_T();
				~SpecialObjData_T(){};
				BOOL IsInited(VOID) const {return m_bInited;};
				BOOL InitFromDB(DBCFile* pDB, INT const nRow);
				SkillID_t GetID(VOID) const {return m_nDataID;};
				CHAR const* GetName(VOID) const {return m_szName;};
				ID_t GetClass(VOID) const {return m_nClass;};
				ID_t GetLogicID(VOID) const {return m_nLogicID;};
				ID_t GetStealthLevel(VOID) const {return m_nStealthLevel;}
				INT GetTrapUsedFlags(VOID) const {return m_nTrapUsedFlags;}
				INT GetActiveTimes(VOID) const {return m_nActiveTimes;}
				Time_t GetContinuance(VOID) const {return m_nContinuance;}
				Time_t GetInterval(VOID) const {return m_nInterval;}
				FLOAT GetTriggerRadius(VOID) const {return m_fTriggerRadius;}
				FLOAT GetEffectRadius(VOID) const {return m_fEffectRadius;}
				INT	GetEffectedObjNumber(VOID) const {return m_nEffectedObjNumber;}
				Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const
				{
					if(0<=nIdx&&DESCRIPTOR_NUMBER>nIdx)
						return &m_aDescriptors[nIdx];
					return NULL;
				}
				CHAR const* Description(VOID) const {return m_szDescription;};
			protected:
			private:
				BOOL 	m_bInited; //�ü�¼�Ƿ��Ѿ���ʼ��
				INT 	m_nDataID; //����ID
				CHAR const* m_szName; //����
				//����Skip Tooltips(�ͻ�����ʾ�õ�˵��������)
				ID_t 	m_nClass; //���
				ID_t 	m_nLogicID; //�߼�ID
				INT		m_nStealthLevel; //������
				INT		m_nTrapUsedFlags; //����ר�õ�һЩ��ǵļ���
				INT		m_nActiveTimes; //���Լ����Ĵ���
				//����Skip �ͻ�����ʾ�õ��������
				Time_t 	m_nContinuance; //����ʱ��
				Time_t 	m_nInterval; //����ʱ����
				FLOAT	m_fTriggerRadius;//�����뾶
				FLOAT 	m_fEffectRadius; //Ӱ��뾶
				INT	  	m_nEffectedObjNumber; //Ӱ��������Ŀ
				Descriptor_T m_aDescriptors[DESCRIPTOR_NUMBER];//��������
				CHAR const* m_szDescription;//�ڲ�ʹ�õ�����
		};
	}
};
#endif //DATARECORDS_H
