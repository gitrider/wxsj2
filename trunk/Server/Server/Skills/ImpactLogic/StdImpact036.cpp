///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact036.cpp
// ����Ա������
// ����˵����Ч��--̽������
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact036.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact036_T::CanViewSpecialObj(OWN_IMPACT & rImp, Obj_Special & rObj) const
		{
			__ENTER_FUNCTION
			if(rObj.GetStealthLevel() < GetDetectLevelRefix(rImp))
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

