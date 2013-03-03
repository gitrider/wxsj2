///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact008.cpp
// 程序员：高骐
// 功能说明：效果--强制怪物改变攻击目标，这对非玩家有效
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "StdImpact008.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact008_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			ObjID_t nTargetID = rImp.GetCasterObjID();
			if(Obj::OBJ_TYPE_MONSTER==rMe.GetObjType()&& TRUE==rMe.IsAlive())
			{
				Obj_Monster& rMonster = (Obj_Monster&)rMe;
				AI_Monster* pAI = (AI_Monster*)(rMonster.GetMonsterAI());
				if(NULL!=pAI)
				{
					Scene* pScene = rMe.getScene();
					if( pScene == NULL )
					{
						return;
					}
					INT iSkillLevel = 0;
					Obj *pObj = (Obj_Human*)(pScene->GetObjManager()->GetObj( nTargetID ));
					Obj_Human* pHuman = NULL;
					if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN )
					{
						pHuman = (Obj_Human* )pObj;
					}

					if( pHuman != NULL )
					{
						const _OWN_SKILL* pOwnSkill = pHuman->Skill_GetSkill( rImp.GetSkillID() );
						if( pOwnSkill != NULL )
						{
							iSkillLevel = pOwnSkill->m_nLevel;
						}
					}
					iSkillLevel -= 1;
					if( 0 > iSkillLevel )
					{
						iSkillLevel = 0;
					}
					if(TRUE==GetTargetMeFlag(rImp, iSkillLevel))
					{
						// Set impact caster as the primary target
						pAI->AddPrimaryEnemy(nTargetID);
					}
					else
					{
						// Set impact caster as the secondary target
						pAI->ChangePrimaryEnemy();
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

