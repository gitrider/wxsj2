///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact042.cpp
// ����Ա������
// ����˵����Ч��--���������䳤��Ч��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact042.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		INT StdImpact042_T::OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const
		{
			__ENTER_FUNCTION
			if(INVALID_ID != GetImmunoImpactCollectionOne(rImp))
			{
				if(TRUE==Impact_IsImpactInCollection(rImpactNeedCheck, GetImmunoImpactCollectionOne(rImp)))
				{
					return MissFlag_T::FLAG_IMMU;
				}
			}
			if(INVALID_ID != GetImmunoImpactCollectionTwo(rImp))
			{
				if(TRUE==Impact_IsImpactInCollection(rImpactNeedCheck, GetImmunoImpactCollectionTwo(rImp)))
				{
					return MissFlag_T::FLAG_IMMU;
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact042_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetAttackSpeedRefix(rImp))
			{
				rMe.MarkAttackSpeedRefixDirtyFlag();
			}
			if(0!=GetCriticalRefix(rImp))
			{
				rMe.MarkCriticalRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact042_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_ATTACK_SPEED:
					if(0!=GetAttackSpeedRefix(rImp))
					{
						rIntAttrRefix += GetAttackSpeedRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_CRITICAL:
					if(0!=GetCriticalRefix(rImp))
					{
						rIntAttrRefix += GetCriticalRefix(rImp);
						return TRUE;
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

