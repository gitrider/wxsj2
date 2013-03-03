///////////////////////////////////////////////////////////////////////////////
// �ļ�����SkillInfo.h
// ����Ա������
// ����˵������ɫ���ϼ���ʵ��
//
// �޸ļ�¼��
// ף����  ����ע�� Ŀ�����ڽ����ĵ���ͬʱ����
//		   ��׼ȷ�������๦�ܣ��˴��޸�ֻ��������ע��
//		   �����κδ����޸ġ�2007-10-29 
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILLINFO_H
#define SKILLINFO_H

#include "Type.h"
#include "DataRecords.h"
#include "GameDefine2.h"
#include "OResultDef.h"
#include "GameStruct.h"
#include "DB_Struct.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;
	class SkillInfo_T
	{
	public:
		//���캯��
		SkillInfo_T();
		SkillInfo_T(SkillInfo_T const& rSrc);
		~SkillInfo_T(){};

		//ִ�г�ʼ��
		VOID Init(VOID);
		//��ü���ID
		SkillID_t GetSkillID(VOID) const {return m_nSkillID;};
		//���ü���ID
		VOID SetSkillID(SkillID_t const nID);;

		//���������
		INT GetAccuracy(VOID) const {return m_nAccuracy;};
		//����������
		VOID SetAccuracy(INT const nID) {m_nAccuracy = nID;};

		//��û�����
		INT GetCriticalRate(VOID) const {return m_nCriticalRate;};
		//���û�����
		VOID SetCriticalRate(INT const nRate) {m_nCriticalRate = nRate;};

		//��ȡЧ������
		INT GetDepleteRefixByValue(VOID) const {return m_nDepleteRefixByValue;}
		//����Ч������
		VOID SetDepleteRefixByValue(INT nValue) {m_nDepleteRefixByValue = nValue;}

		INT GetDepleteRefixByRate(VOID) const {return m_nDepleteRefixByRate;}
		VOID SetDepleteRefixByRate(INT nRate) {m_nDepleteRefixByRate = nRate;}
		INT GetPowerRefixByValue(VOID) const {return m_nPowerRefixByValue;}
		VOID SetPowerRefixByValue(INT nValue) {m_nPowerRefixByValue = nValue;}
		INT GetPowerRefixByRate(VOID) const {return m_nPowerRefixByRate;}
		VOID SetPowerRefixByRate(INT nRate) {m_nPowerRefixByRate = nRate;}
		INT GetTimeRefixByValue(VOID) const {return m_nTimeRefixByValue;}
		VOID SetTimeRefixByValue(INT nValue) {m_nTimeRefixByValue = nValue;}
		INT GetTimeRefixByRate(VOID) const {return m_nTimeRefixByRate;}
		VOID SetTimeRefixByRate(INT nRate) {m_nTimeRefixByRate = nRate;}
		
		Time_t GetCooldownTime(VOID) const {return m_nCooldownTime;};
		VOID SetCooldownTime(Time_t const nTime) {m_nCooldownTime = nTime;};
		Time_t GetChargeTime(VOID) const {return m_nChargeTime;};
		VOID SetChargeTime(Time_t const nTime) {m_nChargeTime = nTime;};
		Time_t GetChannelTime(VOID) const {return m_nChannelTime;};
		VOID SetChannelTime(Time_t const nTime) {m_nChannelTime = nTime;};
		//�Լ����������ֵ�Ĵ�ȡ
		FLOAT GetMinSkillDistance() const { return m_fMinSkillDistance; }
		VOID SetMinSkillDistance(FLOAT val) { m_fMinSkillDistance = val; }
		//�Լ�����Զ����ֵ�Ĵ�ȡ
		FLOAT GetMaxSkillDistance() const { return m_fMaxSkillDistance; }
		VOID SetMaxSkillDistance(FLOAT val) { m_fMaxSkillDistance = val; }
		////������������������ʱ�������
		//INT GetChargesOrInterval() const { return m_ChargesOrInterval; }
		VOID SetChargesOrInterval(INT val) { m_ChargesOrInterval = val; }
		//�������ID
		MenPaiID_t GetMenPai(VOID) const;
		//��ȡ��������
		CHAR const* GetName(VOID) const;
		//��ȡ���ܵ��ķ��ȼ�Ҫ��
		INT  GetXinFaLevelRequirement(VOID) const;
		//��ȡ�˼����Ƿ����ʹ������
		BOOL GetMustUseWeaponFlag(VOID) const;
		//��ȡ�Ƿ������ڱ��1
		BOOL GetDisableByFlag1(VOID) const;
		//��ȡ�Ƿ������ڱ��2
		BOOL GetDisableByFlag2(VOID) const;
		//��ȡ�Ƿ������ڱ��3
		BOOL GetDisableByFlag3(VOID) const;
		//��ȡ�ķ�ID
		XinFaID_t GetXinFa(VOID) const;
		//��ȡ�ķ���������
		INT  GetXinFaParam(VOID) const;
		//��ȡ�Ƿ�ΪԶ�̼���
		INT  GetRangedSkillFlag(VOID) const;
		//�Ƿ�ǿ���ж���һ������ִ�еļ���
		BOOL IsForceBreakPreSkill(VOID) const;
		//���Ŀ������
		INT  GetTargetMustInSpecialState(VOID) const;
		//Charge, channel or instant shot skill
		ID_t GetSkillType(VOID) const;
		//��ȡ��ȴʱ���ID
		CooldownID_t GetCooldownID(VOID) const;
		//����ѡ����
		ID_t GetSelectType(VOID) const;
		//���＼�ܷ������� 0:�����ֶ���ѡ,1:AI�Զ�ִ��,2:��ǿ�������Եı�������
		ID_t GetOperateModeForPetSkill(VOID) const;
		//���ܷ�������,ֻ�Գ��＼����Ч
		ID_t GetPetRateOfSkill(VOID) const;
		//���＼������,0:�﹦,1:����,2:����,3:����,4:����
		ID_t GetTypeOfPetSkill(VOID) const;
		//���＼�ܲ�����Ч��ID
		ID_t GetImpactID(VOID) const;
		//Ŀ��ѡȡ�߼�
		ID_t GetTargetingLogic(VOID) const;
		//���ܶ������ŵ�ʱ��
		Time_t GetPlayActionTime(VOID) const;
		//����ʹ�÷�Χ�½�
		FLOAT GetOptimalRangeMin(VOID) const;
		//����ʹ�÷�Χ�Ͻ�
		FLOAT GetOptimalRangeMax(VOID) const;
		//����Ǽ��ܵ�������ǣ�-1����������Ŀ�꣬0�����ԣ�1�Ǽ�����ǿĿ��
		ID_t GetStandFlag(VOID) const;
		//����������ʲô��Ӫ��Ŀ�꣬�жԣ��Ѻã�������ȫ��������
		ID_t GetTargetLogicByStand(VOID) const;
		//�ӵ������ٶ�
		INT  GetSpeedOfBullet(VOID) const;
		//�˺�������ʱ
		INT  GetHurtDelayTime(VOID) const;
		//���ܵ����ļ���ؼ��
		ID_t GetTargetCheckByObjType(VOID) const;
		//�����Ƿ�ֻ�������ڶ���
		BOOL IsPartyOnly(VOID) const;
		//������Ч�����������е���Ч����
		INT GetChargesOrInterval(VOID) const;
		//�Ƿ��Զ������ͷż���
		BOOL IsAutoShotSkill(VOID) const;
		//���ܵ���ȴ�Ƿ������﹥���ٶ�Ӱ��
		BOOL UseNormalAttackRate(VOID) const;
		//���ð뾶
		FLOAT GetRadius(VOID) const
		{
			return m_fRadius;
		}
		VOID SetRadius(FLOAT fRadius)
		{
			m_fRadius = fRadius;
		}
		//���ýǶ�
		FLOAT GetAngle(VOID) const;
		//�������Ŀ����
		INT GetMaxTargetNumber(VOID) const;
		VOID SetMaxTargetNumber( INT val );
		//�������Ƿ���ж��Զ�������ܵ������ͷ�
		BOOL CanInterruptAutoShot(VOID) const;
		//�ӳ�ʱ��
		Time_t GetDelayTime(VOID) const;
		ID_t GetLogicID(VOID) const;
			
		ConDepTerm_T const* GetConDepTermByIndex(INT const nIdx) const;
		INT GetSkillLevel(VOID) const;
		Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const;

		INT GetCharacterSkillLevel() const { return m_iSkillLevel; }
		VOID SetCharacterSkillLevel(INT val) { m_iSkillLevel = val; }

		
		SkillInfo_T& operator=(SkillTemplateData_T const& rSrc);
		SkillInfo_T& operator=(SkillInstanceData_T const& rSrc);
		SkillInfo_T& operator=(SkillInfo_T const& rSrc);

		INT GetSkillLogicCount() const { return m_iSkillLogicCount; }
		VOID SetSkillLogicCount(INT val) { m_iSkillLogicCount = val; }

		INT GetSendSkillImpactID() const {return m_iSendSkillImpactID;}
		VOID SetSendSkillImpactID(INT val) {m_iSendSkillImpactID = val;}

	protected:
		SkillID_t GetInstanceID(VOID) const {return m_nInstanceID;};
		VOID SetInstanceID(SkillID_t const nID) {m_nInstanceID = nID;};
		VOID InitOptimalRangeMin();
		VOID InitOptimalRangeMax();
		VOID InitChargesOrInterval();
		//VOID InitMaxTargetNumber();
	private:
		//���ܱ��
		SkillID_t m_nSkillID; 
		//ʵ�����
		INT m_nInstanceID;
		//������
		INT m_nAccuracy;
		//������,���߽б����ʣ�
		//�������������еĹ����б�ŭ�����ӹ������ĸ���
		INT m_nCriticalRate;
		//Ч��ֵ��������������ĳ���Զ�����������Ե�Ӱ��ϵ��
		INT m_nDepleteRefixByValue; 
		//����%����
		INT m_nDepleteRefixByRate; 
		//����ֵ����
		INT m_nPowerRefixByValue; 
		//����%����
		INT m_nPowerRefixByRate;
		//Ч���������ʱ��ֵ����
		INT m_nTimeRefixByValue; 
		//Ч���������ʱ��%����
		INT m_nTimeRefixByRate;
		//ʹ����Ҫ��ȴ�����ٴη��еļ��ܺ��������ȴʱ��
		Time_t m_nCooldownTime;
		//�����ݿ��ã���ͨ���ӿڸ�ֵ��δ�ҵ��κ�����㷨
		Time_t m_nChargeTime;
		//�����ݿ��ã���ͨ���ӿڸ�ֵ��δ�ҵ��κ�����㷨
		Time_t m_nChannelTime;
		//������Ч��Χ������
		FLOAT m_fMinSkillDistance;
		FLOAT m_fMaxSkillDistance;
		//��������������ʱ�������
		INT m_ChargesOrInterval;
		//���ð뾶
		FLOAT m_fRadius;
		//���Ŀ����������
		INT m_iMaxTargetNumber;
		INT	m_iSkillLevel;
		INT m_iSkillLogicCount;

		//��Ӧ��ImpactID ���ID ֻ��ʹ��sendimpacttounitʱ���Ż�ʵ������
		INT	m_iSendSkillImpactID;
	};

	//���ݼ���ID��ü�������ģ��
	Skill_Module::SkillTemplateData_T const* GetSkillTemplateByID(INT nSkill);
	//���ݼ���ʵ���Ż�ü�������ģ��
	Skill_Module::SkillInstanceData_T const* GetSkillInstanceByID(INT nInstance);

	//����δʹ��
	class DamageList_T
	{
		public:
			DamageList_T()
			{
				memset((VOID*)m_aDamages, 0, sizeof(m_aDamages));
			}
			~DamageList_T(){};
			INT m_aDamages[DAMAGE_TYPE_NUMBER];
		protected:
		private:
	};
}

#endif //SKILLINFO_H
