///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact023.cpp
// ����Ա��frankwu
// ����˵�������������DS
//
// Ч���������ֵĸ�ʽ��|Ч��ID|��Ч����|�ܻ�ʱ��Ч|����ʱ��Ч|�ܻ�ʱ�˺�����
//					   |����ʱ��ȡ����MP�ٷ���|����ʱ��ȡ����ŭ���ٷ���|
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact023.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact023_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact023_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(0>=GetDamageIgnoreRate(rImp))
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
			// ���˺���������
			rDamage = Float2Int((rDamage*GetDamageIgnoreRate(rImp))/100.0f);
			__LEAVE_FUNCTION
		}

		VOID StdImpact023_T::OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			INT nMpDamageRate = GetMpDamageRate(rImp);
			INT nRageDamageRate = GetRageDamageRate(rImp);
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(0>=nMpDamageRate && 0>= nRageDamageRate)
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
			// ����ʱ��ȡ����MP�ٷ���
			if(0<nMpDamageRate)
			{
				INT nMP = Float2Int((nMpDamageRate*rDamage)/100.0f);
				rTar.ManaIncrement(-nMP, rImp.GetCasterLogicCount(), &rMe);
			}
			// ����ʱ��ȡ����ŭ���ٷ���
			if(0<nRageDamageRate)
			{
				INT nRage = rTar.GetRage();
				nRage = Float2Int((nRageDamageRate*nRage)/100.0f);
				rTar.RageIncrement(-nRage, rImp.GetCasterLogicCount(), &rMe); 
			}
			__LEAVE_FUNCTION
		}
	};
};

