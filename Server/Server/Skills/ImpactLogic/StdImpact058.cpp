/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   13:51
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact058.cpp
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact058
	file ext:	cpp
	author:		Richard
*********************************************************************/

#include "stdafx.h"
#include "StdImpact058.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{

		StdImpact058_T::StdImpact058_T()
		{
		}

		StdImpact058_T::~StdImpact058_T()
		{

		}

		VOID StdImpact058_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			//获得角色身上的先知类技能引发效果的全局UniqueID
			//rImp.SetBallUniqueID( rMe.Impact_GetImpactUniqueID() );
			__LEAVE_FUNCTION
		}

		VOID StdImpact058_T::OnIntervalOver( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION

			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(rMe,rImp);
			
			INT nHP = GetHpModification(rImp);
			rMe.HealthIncrement(nHP, rImp.GetCasterLogicCount(), pImpactSender);

			INT nMP = GetMpModification(rImp);
			rMe.ManaIncrement(nMP, rImp.GetCasterLogicCount(), pImpactSender);

			INT nRage = GetRageModification(rImp);
			rMe.RageIncrement(nRage, rImp.GetCasterLogicCount(), pImpactSender);

			__LEAVE_FUNCTION
		}

		VOID StdImpact058_T::OnFadeOut( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION

			__LEAVE_FUNCTION
		}

		BOOL StdImpact058_T::InitFromData( OWN_IMPACT& rImp, ImpactData_T const& rData ) const
		{
			SetHpModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				OwnImpactParamIndex_T::IDX_HP,
				rImp.GetSkillLevel())
			);

			SetMpModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				OwnImpactParamIndex_T::IDX_MP,
				rImp.GetSkillLevel())
			);

			SetRageModification
			(rImp, 
			Impact_GetImpactDataDescriptorValueByIndex
				(rImp.GetDataIndex(), 
				OwnImpactParamIndex_T::IDX_RAGE,
				rImp.GetSkillLevel())
			);

			return TRUE;
		}
	};
};

