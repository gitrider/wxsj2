/********************************************************************
	created:	2010/05/13
	author:		�¾�
	purpose:	����һ�������Ĵ���impact(���˼·�ο�65�Ĵ����������impact��65���Ը���)
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
				MAX_IMPACT_NUM_AT_SAME_TIME = 5,		//ͬʱ���Լ�����impact����
			};

			class ImpactDataDescriptorIndex_T
			{
			public:
				enum
				{
					IDX_EFFECT_RATE = 0,
					IDX_TARGET_TYPE,				//0���Լ� 1���Է�
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
			//Ŀ���Ǳ��ѹ����Ľ�ɫ
			Obj_Character* GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const;
		private:
		} ;
	};
};

#endif // StdImpact065_h__
