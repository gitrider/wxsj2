///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact025.cpp
// ����Ա��frankwu
// ����˵����Ч��--��������һ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact025.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact025_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		INT StdImpact025_T::OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return FALSE;
			}
			// ��Ч���� 
			INT nRet = rand() % 100;
			if (nRet > GetActivateOdds(rImp))
			{// û����Ч��ֱ�ӷ���
				return FALSE;
			}
			// ���ܵ�����һ����ʱ��������һ������
			if (TRUE == rImpactNeedCheck.IsCriticalHit())
			{
				return MissFlag_T::FLAG_ABSORB;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};



