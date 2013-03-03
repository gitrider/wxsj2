///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact039.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ﱬ�������ӣ�ͬʱ�ܵ��˺�������ŭ��
//			���ּ����������õ�
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact039.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact039_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetCriticalRefix(rImp))
			{
				rMe.MarkCriticalRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact039_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_CRITICAL==nIdx)
			{
				if(0!=GetCriticalRefix(rImp))
				{
					rIntAttrRefix += GetCriticalRefix(rImp);
					return TRUE;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact039_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			INT nValue = Float2Int((rRageRegeneration * GetRageRegenerateRefix(rImp))/100.0f);
			rRageRegeneration += nValue;
			__LEAVE_FUNCTION
		}			
	};
};


