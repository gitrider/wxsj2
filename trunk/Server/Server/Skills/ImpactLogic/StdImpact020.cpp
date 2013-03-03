///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact020.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�Ч�����������������ֱ�Ӹ���
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact020.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact020_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact020_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact020_T::OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			INT nRate = GetRefixRate(rImp);
			nRate +=100;
			if(0>nRate)
			{
				nRate=0;
			}
			INT nValue = 0;
			Obj_Character::_RELIVE_INFO ReliveInfo;
			ReliveInfo.Reset();
			ReliveInfo.m_bHPUsePercent = TRUE;
			ReliveInfo.m_bMPUsePercent = TRUE;
			ReliveInfo.m_bSPUsePercent = TRUE;
			nValue = GetHpRecoverRate(rImp);
			nValue = Float2Int((nValue*nRate)/100.0f);
			ReliveInfo.m_nHPPercent = nValue;
			nValue = GetMpRecoverRate(rImp);
			nValue = Float2Int((nValue*nRate)/100.0f);
			ReliveInfo.m_nMPPercent = nValue;
			nValue = GetRageRecoverRate(rImp);
			nValue = Float2Int((nValue*nRate)/100.0f);
			ReliveInfo.m_nSPPercent = nValue;
			ReliveInfo.m_SceneID = rMe.__GetSceneID();
			WORLD_POS const* pPos = rMe.getWorldPos();
			if(pPos)
			{
				ReliveInfo.m_Pos.m_fX = pPos->m_fX;
				ReliveInfo.m_Pos.m_fZ = pPos->m_fZ;				
			}
			rMe.SetReliveInfo(TRUE, &ReliveInfo);
			__LEAVE_FUNCTION
		}
	};
};

