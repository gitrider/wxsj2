///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact044.h
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�����Ч�������ߵ��ƶ��ٶȣ�������һ�γɹ����Ŀ�����һ����������3��������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT044_H
#define STDIMPACT044_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact044_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_044,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_MOVE_SPEED_REFIX =0,
							IDX_ACTIVATE_ODDS,
							IDX_MODIFY_STRIKE_POINT,
						};
				};
				StdImpact044_T() {};
				~StdImpact044_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetMoveSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MOVE_SPEED_REFIX,
						rImp.GetSkillLevel()
						);
				};
				INT GetActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS,
						rImp.GetSkillLevel()
						);
				};
				INT GetModifyStrikePoint(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex
						(rImp.GetDataIndex(), 
						ImpactDataDescriptorIndex_T::IDX_MODIFY_STRIKE_POINT,
						rImp.GetSkillLevel()
						);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const;
			protected:
			private:
		} SOT_GaiBang007_T;
	};
};
#endif //STDIMPACT044_H
