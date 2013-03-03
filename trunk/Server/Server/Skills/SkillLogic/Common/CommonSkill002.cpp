///////////////////////////////////////////////////////////////////////////////
// �ļ�����CommonSkill002.cpp
// ����Ա������
// ����˵�����ٻ�����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Type.h"
#include "CommonSkill002.h"
#include "Obj_Pet.h"
#include "Obj_Human.h"
#include "LogicManager.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "Player.h"
#include "Impact_Core.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class CommonSkill002_T : public SkillLogic_T
		BOOL CommonSkill002_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				INT nValue = 0;
				OWN_IMPACT impact;
				TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
				//��ý�ɫ���ϵļ���ʵ��
				SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();

				nValue = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
				//�������ֵ�Ϸ�
				if(INVALID_ID != nValue)
				{
					g_ImpactCore.InitImpactFromData(nValue, impact, rMe);
					//ʹЧ����Ч
					if( !RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag) )
					{
						return FALSE;
					}
				}

				GCManipulatePetRet msg;
				ORESULT oResult = ((Obj_Human*)&rMe)->CallUpPet();
				if(OR_FAILED(oResult))
				{
					Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
					rHuman.SendOperateResultMsg(oResult);
					
					msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID);
				}
				else if (OR_SUCCEEDED(oResult))
				{
					msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPSUCC);
				}
				((Obj_Human*)&rMe)->GetPlayer()->SendPacket(&msg);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

