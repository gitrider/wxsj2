// AbilityCooking.cpp
#include "stdafx.h"
#include "Config.h"
#include "AbilityCooking.h"
#include "AbilityManager.h"

AbilityCooking::AbilityCooking(const Ability& a) : AbilityCompound(a)
{
}

INT AbilityCooking::CalcVigorWastage(Obj_Human* pHuman, INT nPrescrData, INT nAbilityID)
{
__ENTER_FUNCTION
	if(NULL == pHuman || nPrescrData == 0)
	{
		return 0;
	}

	INT nDemandVigor = nPrescrData;

	//AbilityID_t nAssistantAbilityID = AbilityManager::ABILITY_CLASS_REGIMEN;
	//if(nAssistantAbilityID != INVALID_ID && pHuman->__IsAbilityHaveLearned(nAssistantAbilityID))
	//{
	//	INT nAssistantAbilityLevel = pHuman->__GetAbilityLevel(nAssistantAbilityID);
	//	nDemandVigor;// = CalcAbilityAssistantDemand(nDemandVigor, nAssistantAbilityLevel);
	//}

	return nDemandVigor;
__LEAVE_FUNCTION
	return 0;
}

INT AbilityCooking::CalcEnergyWastage(Obj_Human* pHuman, INT nPrescrData, INT nAbilityID)
{
__ENTER_FUNCTION
	if(NULL == pHuman || nPrescrData == 0)
	{
		return 0;
	}

	INT nDemandEnergy = nPrescrData;

	//AbilityID_t nAssistantAbilityID = AbilityManager::ABILITY_CLASS_REGIMEN;
	//if(nAssistantAbilityID != INVALID_ID && pHuman->__IsAbilityHaveLearned(nAssistantAbilityID))
	//{
	//	INT nAssistantAbilityLevel = pHuman->__GetAbilityLevel(nAssistantAbilityID);
	//	nDemandEnergy ;//= CalcAbilityAssistantDemand(nDemandEnergy, nAssistantAbilityLevel);
	//}

	return nDemandEnergy;
__LEAVE_FUNCTION
	return 0;
}
