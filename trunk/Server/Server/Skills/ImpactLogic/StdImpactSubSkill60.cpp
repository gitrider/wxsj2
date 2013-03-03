/********************************************************************
	created:	2008/02/28
	created:	28:2:2008   11:09
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpactSubSkill60.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpactSubSkill60
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "StdImpactSubSkill60.h"
#include "Skill_Core.h"
#include "GameTable.h"
#include "Scene.h"


BOOL StdImpact060_T::InitFromData( OWN_IMPACT& rImp, ImpactData_T const& rData ) const
{
	__ENTER_FUNCTION

	//永久持续
	rImp.SetContinuance( -1 );
	
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID StdImpact060_T::OnUseSkillSuccessfully( OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill ) const
{
	__ENTER_FUNCTION
	SkillID_t iSkillID =  rSkill.GetSkillID();
	Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(iSkillID);
	INT iIndex = 0;
	for( INT i=0; i<Skill_Module::SkillTemplateData_T::MAX_SUB_SKILL; ++i )
	{
		INT iSubSkillID = pSkillTemplate->GetSubSkill( i );
		if( INVALID_ID ==  iSubSkillID )
		{
			break;
		}
		INT iRateForSubSkill = pSkillTemplate->GetSubSkillSuccessRate( i );
		if( !IsSubSkillCanSuccess(iRateForSubSkill, rMe) )
		{
			continue;
		}
		//向玩家待发技能列表添加此技能
		rMe.SetSubSkillID( iIndex, iSubSkillID );
		++iIndex;
	}

	__LEAVE_FUNCTION
}

INT StdImpact060_T::IsSubSkillCanSuccess( INT iRate, Obj_Character& rMe ) const
{
	__ENTER_FUNCTION
	Scene* pScene = rMe.getScene();
	if(NULL==pScene)
	{
		return FALSE;
	}
	INT nRet = pScene->GetRand100();
	if( nRet <= iRate )
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

	return FALSE;
}

