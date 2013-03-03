///////////////////////////////////////////////////////////////////////////////
// 文件名：SkillInfo.h
// 程序员：高骐
// 功能说明：角色身上技能实例
//
// 修改记录：
// 祝方亮  增加注释 目的在于建立文档的同时更快
//		   更准确的掌握类功能，此次修改只限于增加注释
//		   不做任何代码修改。2007-10-29 
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
		//构造函数
		SkillInfo_T();
		SkillInfo_T(SkillInfo_T const& rSrc);
		~SkillInfo_T(){};

		//执行初始化
		VOID Init(VOID);
		//获得技能ID
		SkillID_t GetSkillID(VOID) const {return m_nSkillID;};
		//设置技能ID
		VOID SetSkillID(SkillID_t const nID);;

		//获得命中率
		INT GetAccuracy(VOID) const {return m_nAccuracy;};
		//设置命中率
		VOID SetAccuracy(INT const nID) {m_nAccuracy = nID;};

		//获得会心率
		INT GetCriticalRate(VOID) const {return m_nCriticalRate;};
		//设置会心率
		VOID SetCriticalRate(INT const nRate) {m_nCriticalRate = nRate;};

		//获取效果修正
		INT GetDepleteRefixByValue(VOID) const {return m_nDepleteRefixByValue;}
		//设置效果修正
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
		//对技能最近修正值的存取
		FLOAT GetMinSkillDistance() const { return m_fMinSkillDistance; }
		VOID SetMinSkillDistance(FLOAT val) { m_fMinSkillDistance = val; }
		//对技能最远修正值的存取
		FLOAT GetMaxSkillDistance() const { return m_fMaxSkillDistance; }
		VOID SetMaxSkillDistance(FLOAT val) { m_fMaxSkillDistance = val; }
		////连续攻击次数或引导时间的修正
		//INT GetChargesOrInterval() const { return m_ChargesOrInterval; }
		VOID SetChargesOrInterval(INT val) { m_ChargesOrInterval = val; }
		//获得门派ID
		MenPaiID_t GetMenPai(VOID) const;
		//获取技能名称
		CHAR const* GetName(VOID) const;
		//获取技能的心法等级要求
		INT  GetXinFaLevelRequirement(VOID) const;
		//获取此技能是否必须使用武器
		BOOL GetMustUseWeaponFlag(VOID) const;
		//获取是否受限于标记1
		BOOL GetDisableByFlag1(VOID) const;
		//获取是否受限于标记2
		BOOL GetDisableByFlag2(VOID) const;
		//获取是否受限于标记3
		BOOL GetDisableByFlag3(VOID) const;
		//获取心法ID
		XinFaID_t GetXinFa(VOID) const;
		//获取心法修正参数
		INT  GetXinFaParam(VOID) const;
		//获取是否为远程技能
		INT  GetRangedSkillFlag(VOID) const;
		//是否强制中断上一个正在执行的技能
		BOOL IsForceBreakPreSkill(VOID) const;
		//获得目标类型
		INT  GetTargetMustInSpecialState(VOID) const;
		//Charge, channel or instant shot skill
		ID_t GetSkillType(VOID) const;
		//获取冷却时间的ID
		CooldownID_t GetCooldownID(VOID) const;
		//鼠标点选类型
		ID_t GetSelectType(VOID) const;
		//宠物技能发动类型 0:主人手动点选,1:AI自动执行,2:增强自身属性的被动技能
		ID_t GetOperateModeForPetSkill(VOID) const;
		//技能发动几率,只对宠物技能有效
		ID_t GetPetRateOfSkill(VOID) const;
		//宠物技能类型,0:物功,1:法功,2:护主,3:防御,4:复仇
		ID_t GetTypeOfPetSkill(VOID) const;
		//宠物技能产生的效果ID
		ID_t GetImpactID(VOID) const;
		//目标选取逻辑
		ID_t GetTargetingLogic(VOID) const;
		//技能动作播放的时间
		Time_t GetPlayActionTime(VOID) const;
		//技能使用范围下界
		FLOAT GetOptimalRangeMin(VOID) const;
		//技能使用范围上界
		FLOAT GetOptimalRangeMax(VOID) const;
		//这个是技能的立场标记，-1代表技能消弱目标，0是中性，1是技能增强目标
		ID_t GetStandFlag(VOID) const;
		//技能作用于什么阵营的目标，敌对，友好，中立，全部。。。
		ID_t GetTargetLogicByStand(VOID) const;
		//子弹飞行速度
		INT  GetSpeedOfBullet(VOID) const;
		//伤害计算延时
		INT  GetHurtDelayTime(VOID) const;
		//技能的消耗及相关检查
		ID_t GetTargetCheckByObjType(VOID) const;
		//技能是否只能作用于队友
		BOOL IsPartyOnly(VOID) const;
		//连续生效次数和引导中的生效次数
		INT GetChargesOrInterval(VOID) const;
		//是否自动连续释放技能
		BOOL IsAutoShotSkill(VOID) const;
		//技能的冷却是否受人物攻击速度影响
		BOOL UseNormalAttackRate(VOID) const;
		//作用半径
		FLOAT GetRadius(VOID) const
		{
			return m_fRadius;
		}
		VOID SetRadius(FLOAT fRadius)
		{
			m_fRadius = fRadius;
		}
		//作用角度
		FLOAT GetAngle(VOID) const;
		//最大作用目标数
		INT GetMaxTargetNumber(VOID) const;
		VOID SetMaxTargetNumber( INT val );
		//本技能是否会中断自动射击技能得连续释放
		BOOL CanInterruptAutoShot(VOID) const;
		//延迟时间
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
		//技能编号
		SkillID_t m_nSkillID; 
		//实例编号
		INT m_nInstanceID;
		//命中率
		INT m_nAccuracy;
		//会心率,或者叫暴几率，
		//就是在连续发招的过程中暴怒并增加攻击力的概率
		INT m_nCriticalRate;
		//效果值修正，修正就是某属性对其它相关属性的影响系数
		INT m_nDepleteRefixByValue; 
		//消耗%修正
		INT m_nDepleteRefixByRate; 
		//消耗值修正
		INT m_nPowerRefixByValue; 
		//威力%修正
		INT m_nPowerRefixByRate;
		//效果或陷阱的时间值修正
		INT m_nTimeRefixByValue; 
		//效果或陷阱的时间%修正
		INT m_nTimeRefixByRate;
		//使用需要冷却才能再次发招的技能后所需的冷却时间
		Time_t m_nCooldownTime;
		//从数据库获得，或通过接口赋值，未找到任何相关算法
		Time_t m_nChargeTime;
		//从数据库获得，或通过接口赋值，未找到任何相关算法
		Time_t m_nChannelTime;
		//攻击有效范围的修正
		FLOAT m_fMinSkillDistance;
		FLOAT m_fMaxSkillDistance;
		//攻击次数或引导时间的修正
		INT m_ChargesOrInterval;
		//作用半径
		FLOAT m_fRadius;
		//最大目标搜索个数
		INT m_iMaxTargetNumber;
		INT	m_iSkillLevel;
		INT m_iSkillLogicCount;

		//对应的ImpactID 这个ID 只有使用sendimpacttounit时，才会实际意义
		INT	m_iSendSkillImpactID;
	};

	//根据技能ID获得技能数据模板
	Skill_Module::SkillTemplateData_T const* GetSkillTemplateByID(INT nSkill);
	//根据技能实例号获得技能数据模板
	Skill_Module::SkillInstanceData_T const* GetSkillInstanceByID(INT nInstance);

	//此类未使用
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
