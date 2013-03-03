/********************************************************************
	created:	2008/05/05
	created:	5:5:2008   11:10
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact063.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact063
	file ext:	h
	author:		Richard
	
	purpose:	DOT
*********************************************************************/

#ifndef StdImpact063_h__
#define StdImpact063_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact063_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_063,
				};

				class ImpactDataDescriptorIndex_T
				{
				public:
					enum
					{
						IDX_REFIX_TYPE=0,			//0为按点数修正，1为按百分比修正
						IDX_REFIX_DAMAGE_TYPE,		//伤害类型，
						IDX_REFIX_PARA,				//伤害数值
						IDX_REFIX_ATTACK_WEIGHT,	//自身攻击力计算比重
						IDX_REFIX_DEFENCE_WEIGHT,	//对方防御力计算比重
					};
				};
				StdImpact063_T() {};
				~StdImpact063_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetDamageType(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_DAMAGE_TYPE);
				}
				VOID SetDamageType(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_DAMAGE_TYPE, nDamage);
				}

				INT GetDamagePara(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_PARA);
				}
				VOID SetDamagePara(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_PARA, nDamage);
				}

				INT GetAttackWeight(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_ATTACK_WEIGHT);
				}
				VOID SetAttackWeight(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_ATTACK_WEIGHT, nDamage);
				}

				INT GetDefenceWeight(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_DEFENCE_WEIGHT);
				}
				VOID SetDefenceWeight(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_REFIX_DEFENCE_WEIGHT, nDamage);
				}
		public: //message handler
			VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
		protected:
			VOID InitDamages( OWN_IMPACT& rImp ) const;
			INT GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const;
		private:
		};
	};
};

#endif // StdImpact063_h__