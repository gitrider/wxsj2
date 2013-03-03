///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact048.cpp
// ����Ա������
// ����˵����Ч��--��һ��ʱ���ڣ�����Ч�������������������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdImpact048.h"
#include "Combat_Core.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact048_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj* pObj=Impact_GetCaster(rMe, rImp);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rAttacker = (Obj_Character&)*pObj;
					OWN_IMPACT impact;
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					g_ImpactCore.InitImpactFromData(GetSubImpact(rImp), impact, rMe);
					myCombatCore.GetResultImpact(rAttacker, rMe, impact);
					Scene* pScene = rMe.getScene();
					if(NULL!=pScene)
					{
						pScene->GetEventCore().RegisterImpactEvent(rMe.GetID(), rAttacker.GetID(), impact, 500);
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

