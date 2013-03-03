///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact043.cpp
// 程序员：高骐
// 功能说明：效果--击中目标时增加自己的怒气，但在这一段时间内防御力降低
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact043.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact043_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetResistGoldRefix(rImp))
			{
				rMe.MarkResistGoldRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistWoodRefix(rImp))
			{
				rMe.MarkResistWoodRefixDirtyFlag();
			}
			if(0!=GetResistWaterRefix(rImp))
			{
				rMe.MarkResistWaterRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistSoilRefix(rImp))
			{
				rMe.MarkResistSoilRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact043_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration += GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact043_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_RESIST_GOLD:
					if(0!=GetResistGoldRefix(rImp))
					{
						rIntAttrRefix += GetResistGoldRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_WOOD:
					if(0!=GetResistWoodRefix(rImp))
					{
						rIntAttrRefix += GetResistWoodRefix(rImp);
						return TRUE;
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_WATER:
					if(0!=GetResistWaterRefix(rImp))
					{
						rIntAttrRefix += GetResistWaterRefix(rImp);
						return TRUE;
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
					if(0!=GetResistFireRefix(rImp))
					{
						rIntAttrRefix += GetResistFireRefix(rImp);
						return TRUE;
					}
					break;

				case CharIntAttrRefixs_T::REFIX_RESIST_SOIL:
					if(0!=GetResistSoilRefix(rImp))
					{
						rIntAttrRefix += GetResistSoilRefix(rImp);
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
