///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact028.cpp
// 程序员：frankwu
// 功能说明：
//
// 效果描述部分的格式：|效果ID|持续时间|金属性修正|木属性修正|火属性修正|毒属性修正
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact028.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact028_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nIncreaceHP = 0;
			INT nDamage = 0;
			INT nValue = 0;
			// 读出的比率<0则表示将该类型对应伤害的一定比率的数值转换成被击者的HP
			// 读出的比率>0则表示将该类型对应伤害的一定比率的数值叠加到施法者上
			
			if(0!=GetGoldRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_TYPE_GOLD] * GetGoldRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_TYPE_GOLD] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_TYPE_GOLD]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetWoodRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_TYPE_WOOD] * GetWoodRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_TYPE_WOOD] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_TYPE_WOOD]=0;
					nIncreaceHP += -nValue;
				}
			}

			if(0!=GetWaterRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_TYPE_WATER] * GetWaterRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_TYPE_WATER] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_TYPE_WATER]=0;
					nIncreaceHP += -nValue;
				}
			}

			if(0!=GetFireRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_TYPE_FIRE] * GetFireRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_TYPE_FIRE] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_TYPE_FIRE]=0;
					nIncreaceHP += -nValue;
				}
			}

			if(0!=GetSoilRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_TYPE_SOIL] * GetSoilRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_TYPE_SOIL] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_TYPE_SOIL]=0;
					nIncreaceHP += -nValue;
				}
			}

			// 将转换来的HP加到自己身上
			if(0<nIncreaceHP)
			{
				rMe.HealthIncrement(nIncreaceHP, rImp.GetCasterLogicCount(), &rMe);
			}
			__LEAVE_FUNCTION
		}
	};
};

