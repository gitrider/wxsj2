/********************************************************************
	created:	2008/05/16
	created:	16:5:2008   20:14
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact076.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact076
	file ext:	cpp
	author:		Richard
	
	purpose:	ÕÙ»½ÊÞ×¨ÓÃ
*********************************************************************/

#include "stdafx.h"
#include "StdImpact076.h"
#include "PetManager.h"
#include "GameStruct_Pet.h"
#include "Obj_Human.h"
#include "Scene.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact076_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		VOID StdImpact076_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			
			if( rMe.GetObjType() != Obj::OBJ_TYPE_HUMAN )
			{
				return;
			}
			
			Obj_Human& oHuman = static_cast<Obj_Human&>(rMe);

			INT iMonsterID = GetMonsterID( rImp );
			oHuman.SetMonsterPetDataID( iMonsterID );
			
			Scene* pScene = rMe.getScene();
			if( NULL == pScene )
			{
				return;
			}

			_MONSTER_PET_ATTR oInit;
			InitAttr( &oInit, rImp );

			PetManager* pPetManager = pScene->GetPetManager();
			if( NULL == pPetManager )
			{
				return;
			}
			pPetManager->SetInitData( oInit );
			
			__LEAVE_FUNCTION
		}

		VOID StdImpact076_T::CriticalRefix(OWN_IMPACT& rImp) const
		{
			__ENTER_FUNCTION

			__LEAVE_FUNCTION
		}

		BOOL StdImpact076_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION

			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact076_T::InitAttr( _MONSTER_PET_ATTR* pInit, OWN_IMPACT& rImp )  const
		{
			pInit->iLifeTime		= GetLifeTime( rImp );
			pInit->iImpactToSelf	= GetImpactSelf( rImp );
			pInit->iImpactToOnwer	= GetImpactOnwer( rImp );

			for( INT i=0; i<MONSTER_PET_MAX_SKILL; ++i )
			{
				pInit->aSkillID[i] = GetSkillIDByIndex( rImp, i+ImpactDataDescriptorIndex_T::IDX_SKILL1_ID );
			}
		}
	};
};
