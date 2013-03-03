/********************************************************************
	created:	2010/07/01
	author:		耿浩
	purpose:	魂珠BUFF修改力量,身法,体质,(与16类似)
*********************************************************************/

#ifndef StdImpact083_h__
#define StdImpact083_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact083_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_083,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_PARAM1,//BagIndex
							IDX_POINT_STR,	//力道
							IDX_POINT_DEX,	//身法
							IDX_POINT_INT,	//内力
							IDX_POINT_CON,	//体质

							IDX_BASE_ATTACK_NEAR,		//基础远程内攻击
							IDX_BASE_ATTACK_FAR,		//基础远程外攻击
							IDX_BASE_ATTACK_MAGIC_NEAR,//基础近程内功攻击
							IDX_BASE_ATTACK_MAGIC_FAR, //基础近程外功攻击

							IDX_BASE_DEFENCE_MAGIC_NEAR,//基础近程内功防御
							IDX_BASE_DEFENCE_MAGIC_FAR,	//基础近程外功防御
							IDX_BASE_DEFENCE_NEAR,		//基础远程内功防御
							IDX_BASE_DEFENCE_FAR,		//基础远程外功防御
						};
				};

				StdImpact083_T(){}
				~StdImpact083_T() {}
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetParam1(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_PARAM1);
				}
				VOID SetParam1(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_PARAM1, nDamage);
				}
				//
				INT GetStr(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_STR);
				}
				VOID SetStr(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_STR, nDamage);
				}
				//
				INT GetDex(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_DEX);
				}
				VOID SetDex(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_DEX, nDamage);
				}
				//
				INT GetInt(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_INT);
				}
				VOID SetInt(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_INT, nDamage);
				}

				INT GetCon(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_CON);
				}
				VOID SetCon(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_POINT_CON, nDamage);
				}
				/********攻击属性*******************/
				INT GetAttackNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_NEAR);
				}
				VOID SetAttackNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_NEAR, nDamage);
				}

				INT GetAttackFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_FAR);
				}
				VOID SetAttackFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_FAR, nDamage);
				}
				//
				INT GetAttackMagicNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_MAGIC_NEAR);
				}
				VOID SetAttackMagicNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_MAGIC_NEAR, nDamage);
				}
				//
				INT GetAttackMagicFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_MAGIC_FAR);
				}
				VOID SetAttackMagicFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_ATTACK_MAGIC_FAR, nDamage);
				}
				/********防御属性*******************/
				INT GetDefenceNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_NEAR);
				}
				VOID SetDefenceNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_NEAR, nDamage);
				}

				INT GetDefenceFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_FAR);
				}
				VOID SetDefenceFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_FAR, nDamage);
				}
				//
				INT GetDefenceMagicNear(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_MAGIC_NEAR);
				}
				VOID SetDefenceMagicNear(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_MAGIC_NEAR, nDamage);
				}
				//
				INT GetDefenceMagicFar(OWN_IMPACT const& rImp) const 
				{
					return rImp.GetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_MAGIC_FAR);
				}
				VOID SetDefenceMagicFar(OWN_IMPACT& rImp, INT const nDamage) const 
				{
					rImp.SetParamByIndex(ImpactDataDescriptorIndex_T::IDX_BASE_DEFENCE_MAGIC_FAR, nDamage);
				}
				//////////////////////////////////////////////////////////////////////////
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;

			protected:

			private:	
				VOID  InitAttrData(Obj_Human& Human,OWN_IMPACT & rImp) const;//获取魂珠属性数据
		};
	};
};

#endif // StdImpact083_h__
