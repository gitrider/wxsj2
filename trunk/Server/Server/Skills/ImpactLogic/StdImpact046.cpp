///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact046.cpp
// 程序员：高骐
// 功能说明：效果--大理技能007的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact046.h"
#include "GameTable.h"
namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact046_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(NULL==pAttacker)
			{
				return;
			}
			if(INVALID_ID==nSkillID)
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE==pCollection->IsThisIDInCollection(nSkillID))
			{
				return;
			}
			if(pAttacker->GetUniqueID()!=rImp.GetCasterUniqueID())
			{
				return;
			}
			INT nRefixRate = GetPowerRefixRate(rImp);
			pDamageList[DAMAGE_TYPE_NEAR]	= pDamageList[DAMAGE_TYPE_NEAR] + Float2Int((pDamageList[DAMAGE_TYPE_NEAR]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_FAR]	= pDamageList[DAMAGE_TYPE_FAR] + Float2Int((pDamageList[DAMAGE_TYPE_FAR]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_MAGIC_NEAR]	= pDamageList[DAMAGE_TYPE_MAGIC_NEAR] + Float2Int((pDamageList[DAMAGE_TYPE_MAGIC_NEAR]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_MAGIC_FAR]	= pDamageList[DAMAGE_TYPE_MAGIC_FAR] + Float2Int((pDamageList[DAMAGE_TYPE_MAGIC_FAR]* nRefixRate)/100.0f);

			pDamageList[DAMAGE_TYPE_GOLD]	= pDamageList[DAMAGE_TYPE_GOLD] + Float2Int((pDamageList[DAMAGE_TYPE_GOLD]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_WOOD]	= pDamageList[DAMAGE_TYPE_WOOD] + Float2Int((pDamageList[DAMAGE_TYPE_WOOD]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_WATER]  = pDamageList[DAMAGE_TYPE_WATER] + Float2Int((pDamageList[DAMAGE_TYPE_WATER]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_FIRE]	= pDamageList[DAMAGE_TYPE_FIRE] + Float2Int((pDamageList[DAMAGE_TYPE_FIRE]* nRefixRate)/100.0f);
			pDamageList[DAMAGE_TYPE_SOIL]	= pDamageList[DAMAGE_TYPE_SOIL] + Float2Int((pDamageList[DAMAGE_TYPE_SOIL]* nRefixRate)/100.0f);
			
			INT nDamage = pDamageList[DAMAGE_TYPE_NEAR]
						+ pDamageList[DAMAGE_TYPE_FAR]
						+ pDamageList[DAMAGE_TYPE_MAGIC_NEAR]
						+ pDamageList[DAMAGE_TYPE_MAGIC_FAR]

						+ pDamageList[DAMAGE_TYPE_GOLD]
						+ pDamageList[DAMAGE_TYPE_WOOD]
						+ pDamageList[DAMAGE_TYPE_WATER]
						+ pDamageList[DAMAGE_TYPE_FIRE]
						+ pDamageList[DAMAGE_TYPE_SOIL];
			INT nRage = Float2Int((nDamage * GetRageConvertRate(rImp))/100.0f);
			pAttacker->RageIncrement(nRage, rImp.GetCasterLogicCount(), pAttacker);
			__LEAVE_FUNCTION
		}
	};
};

