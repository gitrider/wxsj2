///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact045.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ﱬ�������ӣ�ͬʱ�ܵ��˺�������ŭ��
//			���ּ����������õ�
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact045.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact045_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			if(0!=GetRageRegenerationAdditional(rImp))
			{
				rRageRegeneration += GetRageRegenerationAdditional(rImp);
			}
			__LEAVE_FUNCTION
		}
	};
};


