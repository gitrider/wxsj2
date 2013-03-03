/********************************************************************
	created:	2010/05/13
	author:		陈军
	purpose:	带有一定条件的触发impact(设计思路参考65的处理，有了这个impact，65可以更简化)
*********************************************************************/

#ifndef StdImpact081_h__
#define StdImpact081_h__

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact081_T : public ImpactLogic_T
		{
		public:
			enum
			{
				ID = STD_IMPACT_081,
			};

			enum 
			{
				TargetTypeMe	= 0,
				TargetTypeOther
			};

			enum
			{
				MAX_IMPACT_NUM_AT_SAME_TIME = 5,		//同时可以激发的impact数量
			};

			class ImpactDataDescriptorIndex_T
			{
			public:
				enum
				{
					IDX_EFFECT_RATE = 0,
					IDX_TARGET_TYPE,				//0：自己 1：对方
					IDX_IMPACT_ID,
				};
			};

			StdImpact081_T() {};
			~StdImpact081_T() {};
			BOOL IsOverTimed(VOID) const {return FALSE;};
			BOOL IsIntervaled(VOID) const {return FALSE;};

			INT GetEffectRate(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_EFFECT_RATE,
					rImp.GetSkillLevel()
					);
			}

			INT GetTargetType(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_TARGET_TYPE,
					rImp.GetSkillLevel()
					);
			}

			INT GetImpactID(OWN_IMPACT const& rImp, INT nIndex) const
			{
				return Impact_GetImpactDataDescriptorValueByIndex
					(rImp.GetDataIndex(), 
					nIndex * 3 + ImpactDataDescriptorIndex_T::IDX_IMPACT_ID,
					rImp.GetSkillLevel()
					);
			}

		public: //message handler
			virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
		protected:
			BOOL IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp, INT nIndex ) const;
			VOID SendImpactToMe( OWN_IMPACT& rImp, Obj_Character& rMe ) const;
			//目标是被窝攻击的角色
			Obj_Character* GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const;
		private:
		} ;
	};
};

#endif // StdImpact065_h__
