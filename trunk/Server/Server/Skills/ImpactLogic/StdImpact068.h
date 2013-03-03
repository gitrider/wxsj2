/********************************************************************
	created:	2008/05/08
	created:	8:5:2008   14:44
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact068.h
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact068
	file ext:	h
	author:		Richard
	
	purpose:	坐骑专用分类型的一次性伤害
*********************************************************************/

#ifndef StdImpact068_h__
#define StdImpact068_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact068_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_068,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_PARA_ID=0,	//对应4个公式的ID,顺序为
											//力量参数=等级*成长率/1000+力量*力量资质/1000
											//敏捷参数=等级*成长率/1000+敏捷*敏捷资质/1000
											//智力参数=等级*成长率/1000+智力*智力资质/1000
											//体质参数=等级*成长率/1000+体质*体质资质/1000
							IDX_REFIX_RATE,
							IDX_DAMAGE_NEAR,
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
							IDX_PARA_ID=0,		//对应4个公式的ID,顺序为
												//力量参数=等级*成长率/1000+力量*力量资质/1000
												//敏捷参数=等级*成长率/1000+敏捷*敏捷资质/1000
												//智力参数=等级*成长率/1000+智力*智力资质/1000
												//体质参数=等级*成长率/1000+体质*体质资质/1000
							IDX_REFIX_RATE,
							IDX_DAMAGE_NEAR,
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
				StdImpact068_T() {};
				~StdImpact068_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetParaID(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_PARA_ID);
				};
				VOID SetParaID(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_PARA_ID, nDamage);
				};

				INT GetRefixRate(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);
				};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nDamage);
				};

				INT GetDamageNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_NEAR);
				};

				VOID SetDamageNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_NEAR, nDamage);
				};

				INT GetDamageFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FAR);
				};
				VOID SetDamageFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FAR, nDamage);
				};

				INT GetDamageMagicNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_NEAR);
				};
				INT GetDamageMagicFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_FAR);
				};

				VOID SetDamageMagicNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_NEAR, nDamage);
				};
				VOID SetDamageMagicFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC_FAR, nDamage);
				};

				INT GetDamageGold(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_GOLD);
				};
				VOID SetDamageGold(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_GOLD, nDamage);
				};

				INT GetDamageWood(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WOOD);
				};
				VOID SetDamageWood(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WOOD, nDamage);
				};

				INT GetDamageWater(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WATER);
				};
				VOID SetDamageWater(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_WATER, nDamage);
				};

				INT GetDamageFire(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE);
				};
				VOID SetDamageFire(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE, nDamage);
				};

				INT GetDamageSoil(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_SOIL);
				};
				VOID SetDamageSoil(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_SOIL, nDamage);
				};

			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;

				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				VOID FinalDamage( INT * vDamage, Obj_Character &rMe ) const;
				VOID InitImpact( OWN_IMPACT &rImp ) const;
				INT GetRefixPara( OWN_IMPACT & rImp, Obj_Character& rMe ) const;
				INT GetResult( INT iPara1, FLOAT fPara2, INT iPara3, INT iPara4, INT iRate ) const;
			private:
		};
	};
};
#endif // StdImpact068_h__
