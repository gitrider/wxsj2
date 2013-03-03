///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact040.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�����Ч�������ߵ��ƶ��ٶȣ�������һ�δ����ɢĿ��ŭ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact040.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact040_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateTimes(rImp, GetActivateTimesInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact040_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetMoveSpeedRefix(rImp))
			{
				rMe.MarkMoveSpeedRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact040_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_MOVE_SPEED==nIdx)
			{
				if(0!=GetMoveSpeedRefix(rImp))
				{
					rIntAttrRefix += rMe.GetBaseMoveSpeed()*GetMoveSpeedRefix(rImp)/100;
					return TRUE;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact040_T::OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character & rTar) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
			{
				INT nActivateTimes = GetActivateTimes(rImp);
				if(0==nActivateTimes)
				{
					return;
				}
				INT nImpact = GetSubImpact(rImp);
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rMe.GetID(), 500);
				if(0<nActivateTimes)
				{
					--nActivateTimes;
					SetActivateTimes(rImp, nActivateTimes);
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

