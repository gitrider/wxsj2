/********************************************************************
	created:	2008/05/09
	created:	9:5:2008   14:28
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact071.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact071
	file ext:	h
	author:		Richard
	
	purpose:	HOT
*********************************************************************/

#ifndef StdImpact071_h__
#define StdImpact071_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact071_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_071,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_IMPACT_HP = 0,
							IDX_IMPACT_RAGE0,
							IDX_IMPACT_RAGE1,
							IDX_IMPACT_RAGE2,
							IDX_IMPACT_RAGE3,
							IDX_IMPACT_RAGE4,
							IDX_IMPACT_RAGE5,
							IDX_REFIX_RATE
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_TYPE=0,		//0为按点数修正，1为按百分比修正
							IDX_REFIX_PARA_HORSE,
							IDX_REFIX_RATE_HORSE,
							IDX_REFIX_HP,
							IDX_REFIX_RAGE0,
							IDX_REFIX_RAGE1,
							IDX_REFIX_RAGE2,
							IDX_REFIX_RAGE3,
							IDX_REFIX_RAGE4,
							IDX_REFIX_RAGE5,
							IDX_REFIX_RATE
						};
				};
				StdImpact071_T() {};
				~StdImpact071_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetHP(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_HP);
				}
				VOID SetHP(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_HP, nDamage);
				}

				INT GetRage0(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE0);
				}
				VOID SetRage0(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE0, nDamage);
				}

				INT GetRage1(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE1);
				}
				VOID SetRage1(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE1, nDamage);
				}

				INT GetRage2(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE2);
				}
				VOID SetRage2(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE2, nDamage);
				}

				INT GetRage3(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE3);
				}
				VOID SetRage3(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE3, nDamage);
				}

				INT GetRage4(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE4);
				}
				VOID SetRage4(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE4, nDamage);
				}

				INT GetRage5(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE5);
				}
				VOID SetRage5(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_IMPACT_RAGE5, nDamage);
				}
				INT GetRefixRate(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_RATE,
						rImp.GetSkillLevel()
						);
				}

				INT GetParaID(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_PARA_HORSE,
						rImp.GetSkillLevel()
						);
				}

				INT GetRefixRateHorse(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_REFIX_RATE_HORSE,
						rImp.GetSkillLevel()
						);
				}

			public: //message handler
				VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID InitDamages( OWN_IMPACT& rImp ) const;
				INT GetRifixResult( INT iBase, INT iRefix, INT iRefixType ) const;
				INT GetRageByZhiYe( OWN_IMPACT& rImp, INT iZhiYe ) const;
				INT GetDamageRefix( Obj_Character * pImpactSender, OWN_IMPACT& rImp ) const;

				INT GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const;
				INT GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const;
			private:
		} ;
	};
};

#endif // StdImpact062_h__