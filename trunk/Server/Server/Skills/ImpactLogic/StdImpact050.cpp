///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact050.cpp
// ����Ա������
// ����˵����Ч��--��ңŭ����������
//			
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact050.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact050_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration *= GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION			
		}		
	};
};


