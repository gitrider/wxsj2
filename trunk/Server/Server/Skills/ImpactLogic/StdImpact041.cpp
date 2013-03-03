///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact041.cpp
// 程序员：高骐
// 功能说明：效果--明教的五星连珠的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact041.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact041_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetAttackNearRefix(rImp))
			{
				rMe.MarkAttackNearRefixDirtyFlag();
			}
			if(0!=GetDefenceNearRefix(rImp))
			{
				rMe.MarkDefenceNearRefixDirtyFlag();
			}
			if(0!=GetAttackMagicNearRefix(rImp))
			{
				rMe.MarkAttackMagicNearRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicNearRefix(rImp))
			{
				rMe.MarkDefenceMagicNearRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact041_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration *= GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact041_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_ATTACK_NEAR:
					if(0!=GetAttackNearRefix(rImp))
					{
						rIntAttrRefix += GetAttackNearRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_NEAR:
					if(0!=GetDefenceNearRefix(rImp))
					{
						rIntAttrRefix += GetDefenceNearRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC_NEAR:
					if(0!=GetAttackMagicNearRefix(rImp))
					{
						rIntAttrRefix += GetAttackMagicNearRefix(rImp);
						return TRUE;
					}

					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC_NEAR:
					if(0!=GetDefenceMagicNearRefix(rImp))
					{
						rIntAttrRefix += GetDefenceMagicNearRefix(rImp);
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

