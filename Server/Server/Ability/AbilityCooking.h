// AbilityCooking.h

#ifndef __ABILITYCOOKING_H__
#define __ABILITYCOOKING_H__

#include "Ability.h"
#include "AbilityCompound.h"

class AbilityCooking : public AbilityCompound
{
	
public:
	AbilityCooking(const Ability& a);

protected:
	virtual INT CalcVigorWastage(Obj_Human* pHuman, INT nPrescrData  , INT nAbilityID);
	virtual INT CalcEnergyWastage(Obj_Human* pHuman, INT nPrescrData, INT nAbilityID);

private:
};

#endif // __ABILITYCOOKING_H__