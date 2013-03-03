///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact022.cpp
// ����Ա��frankwu
// ����˵�������︴����ͳһЧ��
//
// Ч���������ֵĸ�ʽ��|Ч��ID|��Ч����|�˺�����|�����˺��ٷ���|��ȡ�˺��ٷ���|
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact022.h"
#include "StdImpact004.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact022_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact022_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if (!pAttacker)
			{
				return;
			}
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			// ��Ч���� 
			INT nActivateOdds = GetActivateOdds(rImp);
			INT nRet = pScene->GetRand100();
			if (nRet > nActivateOdds)
			{// û����Ч��ֱ�ӷ���
				return;
			}
			// ��������˺������������ж���
            INT nDamageCount = GetDamageCount(rImp);
			for (INT i = 0; i < nDamageCount; ++i)
			{
				OWN_IMPACT impact;

				CombatCore_T myCombatCore;
				myCombatCore.Reset();			
				myCombatCore.GetResultImpact(rMe, *pAttacker, impact);

				pScene->GetEventCore().RegisterImpactEvent(pAttacker->GetID(), rMe.GetID(), impact, 500);
			}
			// �����˺��ٷ���
			INT nReflectRate = GetReflectRate(rImp);
			if(0<nReflectRate)
			{
				OWN_IMPACT impact;
				INT nReflectDamage = (rDamage * nReflectRate) / 100;
				g_ImpactCore.InitImpactFromData(IMP_NOTYPE_DAMAGE, impact, *pAttacker);
				ModifyHpMpRageStrikePointByValue_T logic;
				if(logic.ID != Impact_GetLogicID(impact))
				{
					AssertEx(FALSE, "[StdImpact022_T::OnDamage]: Error Data found!");
					return;	
				}
				logic.SetHpModification(impact, -nReflectDamage);
				pScene->GetEventCore().RegisterImpactEvent(pAttacker->GetID(), rMe.GetID(), impact, 500);
			}
		
			__LEAVE_FUNCTION
		}

		VOID StdImpact022_T::OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			// ��ȡ�˺��ٷ���
			INT nAbsorbRate = GetAbsorbRate(rImp);
			INT nAbsorbHP = (rDamage * nAbsorbRate) / 100;
			// ��Ŀ���ܵ��˺�ʱ���˺���һ������ת�����Լ���HP			
			rMe.HealthIncrement(nAbsorbHP, rImp.GetCasterLogicCount(), &rMe);
			__LEAVE_FUNCTION
		}
	};
};

