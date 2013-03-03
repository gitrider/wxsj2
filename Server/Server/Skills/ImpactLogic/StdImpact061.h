/********************************************************************
	created:	2008/04/22
	created:	22:4:2008   10:36
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact061.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact061
	file ext:	h
	author:		Richard
	
	purpose:	效果--分类型的一次性伤害,按百分比增加伤害
*********************************************************************/

#ifndef StdImpact061_h__
#define StdImpact061_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact061_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_061,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_DAMAGE_NEAR=0,
							IDX_DAMAGE_FAR,
							IDX_DAMAGE_MAGIC_NEAR,
							IDX_DAMAGE_MAGIC_FAR,
							IDX_DAMAGE_GOLD,
							IDX_DAMAGE_WOOD,
							IDX_DAMAGE_WATER,
							IDX_DAMAGE_FIRE,
							IDX_DAMAGE_SOIL,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_DAMAGE_NEAR=0,
							IDX_DAMAGE_FAR,
							IDX_DAMAGE_MAGIC_NEAR,
							IDX_DAMAGE_MAGIC_FAR,
							IDX_DAMAGE_GOLD,
							IDX_DAMAGE_WOOD,
							IDX_DAMAGE_WATER,
							IDX_DAMAGE_FIRE,
							IDX_DAMAGE_SOIL,
						};
				};
				StdImpact061_T() {};
				~StdImpact061_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetDamageNear(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_NEAR);};
				VOID SetDamageNear(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_NEAR, nDamage);};

				INT GetDamageFar(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FAR);};
				VOID SetDamageFar(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FAR, nDamage);};

				INT GetDamageMagicNear(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_NEAR);};
				VOID SetDamageMagicNear(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_NEAR, nDamage);};

				INT GetDamageMagicFar(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_FAR);};
				VOID SetDamageMagicFar(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_FAR, nDamage);};

				INT GetDamageGold(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_GOLD);};
				VOID SetDamageGold(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_GOLD, nDamage);};

				INT GetDamageWood(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WOOD);};
				VOID SetDamageWood(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WOOD, nDamage);};

				INT GetDamageWater(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WATER);};
				VOID SetDamageWater(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WATER, nDamage);};

				INT GetDamageFire(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE);};
				VOID SetDamageFire(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE, nDamage);};

				INT GetDamageSoil(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_SOIL);};
				VOID SetDamageSoil(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_SOIL, nDamage);};

			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID InitDamages( OWN_IMPACT& rImp ) const;
			private:
		} DI_DamagesByRate_T;
	};
};

#endif // StdImpact061_h__