///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact028.cpp
// ����Ա��frankwu
// ����˵����
//
// Ч���������ֵĸ�ʽ��|Ч��ID|����ʱ��|����������|ľ��������|����������|����������
//
// �޸ļ�¼��
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
			// �����ı���<0���ʾ�������Ͷ�Ӧ�˺���һ�����ʵ���ֵת���ɱ����ߵ�HP
			// �����ı���>0���ʾ�������Ͷ�Ӧ�˺���һ�����ʵ���ֵ���ӵ�ʩ������
			
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

			// ��ת������HP�ӵ��Լ�����
			if(0<nIncreaceHP)
			{
				rMe.HealthIncrement(nIncreaceHP, rImp.GetCasterLogicCount(), &rMe);
			}
			__LEAVE_FUNCTION
		}
	};
};

