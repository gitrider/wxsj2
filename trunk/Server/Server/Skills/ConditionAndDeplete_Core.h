///////////////////////////////////////////////////////////////////////////////
// 文件名：ConditionAndDepleteCore.h
// 程序员：高骐
// 功能说明：技能的基础类，声明技能的公共接口和常用功能
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CONDITIONANDDEPLETECORE_H
#define CONDITIONANDDEPLETECORE_H

#include "Obj_Character.h"
#include "DataRecords.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	typedef enum _ConditionAndDepleteID
	{
		//C表示条件
		//D表示消耗
		//不检查
		CD_INVALID = INVALID_ID,
		//检查自身必须具备特定gather的impact
		C_UNIT_MUST_HAVE_IMPACT = 0,
		//检查自身的HP必须小于最大HP的百分比
		C_UNIT_HP_MUST_LESS_THAN_BY_RATE,
		//检查自身的HP必须大于最大HP的百分比
		C_UNIT_HP_MUST_MORE_THAN_BY_RATE,
		//检查目标身上必须有消耗表格参数1指定的gather的impact
		C_TARGET_MUST_HAVE_IMPACT,
		//检查目标的HP必须小于最大HP的百分比
		C_TARGET_HP_MUST_LESS_THAN_BY_RATE,
		//检查目标的HP必须大于最大HP的百分比
		C_TARGET_HP_MUST_MORE_THAN_BY_RATE,
		//检查并消耗当前HP
		CD_HP_BY_VALUE,
		//检查自身的HP必须大于最大HP的百分比并消耗
		CD_HP_BY_RATE,
		//检查并消耗6职业怒气
		CD_RAGE_BY_VALUE,
		CD_RAGE_BY_RATE,
		//检查并消耗MP
		CD_MP_BY_VALUE,
		CD_MP_BY_RATE,
		//检查并消耗元气
		CD_PNEUMA_BY_VALUE,
		CD_PNEUMA_BY_RATE,
		//消除指定gather的impact
		D_CANCEL_SPECIFIC_IMPACT,
		//检查并消耗指定gather
		CD_CANCEL_SPECIFIC_IMPACT,
		//必要是配偶
		C_TARGET_MUST_BE_MY_SPOUSE,
		//检查可使用神兽召唤次数大于0并消耗add by gh 
		CD_HAVE_PETZHAOHUAN_TIME,
		//检查可使用坐骑召唤次数大于0并消耗
		CD_HAVE_ZUOJIZHAOHUAN_TIME,
	} ConditionAndDepleteID_T;

	//消耗值通用公式：p1*level+p2
	//第一个，第二个消耗默认为以上项目消耗. p0是类型,p1是公式中的p1,p2是公式中的p2
	//第三个消耗：物品种类（类型ID）p1，数量p2
	//（只需要两个参数，即需要检测条件也需要消耗）


	typedef class CDCore_T
	{
		public:
			// public interface
			CDCore_T(){};
			~CDCore_T(){};
			//OnRefixItemPower();
			BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
			BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;
			BOOL ConditionCheckByItem(Obj_Character& rMe, ConDepTerm_T const& rData) const;
			BOOL DepleteByItem(Obj_Character& rMe, ConDepTerm_T const& rData) const;
		protected: //interface for interval using

		private:
			class ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_INVALID,
					};
					ConDepLogic_T(){};
					~ConDepLogic_T(){};
					virtual BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const{return TRUE;};
					virtual BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const{return TRUE;};					
				protected:
					INT		RefixValue( INT iValue, Obj_Character& rMe, ConDepTerm_T const& rData ) const;
					INT		GetCommonDeplete( ConDepTerm_T const &rData, SkillInfo_T &rSkillInfo ) const;
					BOOL	RageConditionCheckByValue(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL	RageDepleteByValue(Obj_Character& rMe, ConDepTerm_T const& rData) const;

					BOOL	RageConditionCheckByRate(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL	RageDepleteByRate(Obj_Character& rMe, ConDepTerm_T const& rData) const;


			private:
			};

			class C_UnitHaveImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_UNIT_MUST_HAVE_IMPACT,
					};
					C_UnitHaveImpact_T(){}
					~C_UnitHaveImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_UnitHpLessThanByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_UNIT_HP_MUST_LESS_THAN_BY_RATE,
					};
					C_UnitHpLessThanByRate_T(){}
					~C_UnitHpLessThanByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_UnitHpMoreThanByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = C_UNIT_HP_MUST_MORE_THAN_BY_RATE,
				};
				C_UnitHpMoreThanByRate_T(){}
				~C_UnitHpMoreThanByRate_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class CD_HpByValue_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_HP_BY_VALUE,
				};
				CD_HpByValue_T(){}
				~CD_HpByValue_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class C_TargetHpLessThanByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = C_TARGET_HP_MUST_LESS_THAN_BY_RATE,
				};
				C_TargetHpLessThanByRate_T(){}
				~C_TargetHpLessThanByRate_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class C_TargetHpMoreThanByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = C_TARGET_HP_MUST_MORE_THAN_BY_RATE,
				};
				C_TargetHpMoreThanByRate_T(){}
				~C_TargetHpMoreThanByRate_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
			
			class C_TargetMustHaveImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_TARGET_MUST_HAVE_IMPACT,
					};
					C_TargetMustHaveImpact_T(){}
					~C_TargetMustHaveImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class D_CancelSpecialImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = D_CANCEL_SPECIFIC_IMPACT,
					};
					D_CancelSpecialImpact_T(){}
					~D_CancelSpecialImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_TargetMustBeMySpouse_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_TARGET_MUST_BE_MY_SPOUSE,
					};
					C_TargetMustBeMySpouse_T(){}
					~C_TargetMustBeMySpouse_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};

			class CD_HpByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_HP_BY_RATE,
					};
					CD_HpByRate_T(){}
					~CD_HpByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};

			class CD_ManaByValue_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_MP_BY_VALUE,
				};
				CD_ManaByValue_T(){};
				~CD_ManaByValue_T(){};
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
			class CD_ManaByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_MP_BY_RATE,
				};
				CD_ManaByRate_T(){};
				~CD_ManaByRate_T(){};
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class CD_PneumaByValue_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_PNEUMA_BY_VALUE,
				};
				CD_PneumaByValue_T(){};
				~CD_PneumaByValue_T(){};
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
			class CD_PneumaByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_PNEUMA_BY_RATE,
				};
				CD_PneumaByRate_T(){};
				~CD_PneumaByRate_T(){};
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class CD_RageByValue_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_RAGE_BY_VALUE,
				};
				CD_RageByValue_T(){}
				~CD_RageByValue_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;

				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
			
			class CD_RageByRate_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_RAGE_BY_RATE,
				};
				CD_RageByRate_T(){}
				~CD_RageByRate_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};

			class CD_CancelSpecialImpact_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_CANCEL_SPECIFIC_IMPACT,
				};
				CD_CancelSpecialImpact_T(){}
				~CD_CancelSpecialImpact_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
			// add by gh for xouxia skill 2010/05/20
			class CD_HavePetZhaoHuanTime_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_HAVE_PETZHAOHUAN_TIME,
				};
				CD_HavePetZhaoHuanTime_T(){}
				~CD_HavePetZhaoHuanTime_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};


			class CD_HaveZuoJiZhaoHuanTime_T : public ConDepLogic_T
			{
			public:
				enum
				{
					ID = CD_HAVE_ZUOJIZHAOHUAN_TIME,
				};
				CD_HaveZuoJiZhaoHuanTime_T(){}
				~CD_HaveZuoJiZhaoHuanTime_T(){}
				BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
				BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
			protected:
			private:
			};
		private:

			C_UnitHaveImpact_T          m_C_UnitHaveImpact;
			C_UnitHpLessThanByRate_T   	m_C_UnitHpLessThanByRate;
			C_UnitHpMoreThanByRate_T	m_C_UnitHpMoreThanByRate;
			C_TargetMustHaveImpact_T    m_C_TargetMustHaveImpact;
			C_TargetHpLessThanByRate_T	m_C_TargetHpLessThanByRate;
			C_TargetHpMoreThanByRate_T  m_C_TargetHpMoreThanByRate;
			CD_HpByValue_T				m_CD_HpByValue;
			CD_HpByRate_T				m_CD_HpByRate;
			CD_ManaByValue_T            m_CD_ManaByValue;
			CD_ManaByRate_T             m_CD_ManaByRate;
			CD_PneumaByValue_T          m_CD_PneumaByValue;
			CD_PneumaByRate_T           m_CD_PneumaByRate;
			CD_RageByValue_T			m_CD_RageByValue;
			CD_RageByRate_T				m_CD_RageByRate;
			D_CancelSpecialImpact_T		m_D_CancelSpecialImpact;
			CD_CancelSpecialImpact_T	m_CD_CancelSpecialImpact;
			C_TargetMustBeMySpouse_T	m_C_TargetMustBeMySpouse;
			CD_HavePetZhaoHuanTime_T	m_CD_HavePetZhaoHuanTime;
			CD_HaveZuoJiZhaoHuanTime_T  m_CD_HaveZuoJiZhaoHuanTime;

	} ConditionAndDepleteCore_T;
	extern ConditionAndDepleteCore_T g_ConditionAndDepleteCore;
}
#endif //CONDITIONANDDEPLETECORE_H
