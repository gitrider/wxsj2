///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact044.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�����Ч�������ߵ��ƶ��ٶȣ�������һ�γɹ����Ŀ�����һ����������3��������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact044.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact044_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetMoveSpeedRefix(rImp))
			{
				rMe.MarkMoveSpeedRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact044_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_MOVE_SPEED==nIdx)
			{
				if(0!=GetMoveSpeedRefix(rImp))
				{
					rIntAttrRefix += Float2Int((rMe.GetBaseMoveSpeed()*GetMoveSpeedRefix(rImp))/100.0f);
					return TRUE;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact044_T::OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character & rTar) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				if(0>=GetActivateOdds(rImp))
				{
					return;
				}
				Scene* pScene = rMe.getScene();
				if(NULL==pScene)
				{
					return;
				}
				INT nRand = pScene->GetRand100();
				if(nRand<=GetActivateOdds(rImp))
				{
					rMe.StrikePointIncrement(GetModifyStrikePoint(rImp));
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

